#include "Progress/ProgressStore.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "Util/Logger.hpp"

namespace {

using json = nlohmann::json;

constexpr const char* kProgressSavePath = "Resources/Save/progress.json";

std::filesystem::path g_SavePath(kProgressSavePath);
std::unordered_map<LevelId, StarConditions> g_BestConditions;

json MakeDefaultJson() {
    return json{{"levels", json::object()}};
}

StarConditions ConditionsFromLegacyStars(int stars) {
    return StarConditions{stars > 0, stars > 1, stars > 2};
}

bool ParseConditions(const json& value, StarConditions& outConditions) {
    if (!value.is_array() || value.size() != outConditions.size()) {
        return false;
    }

    for (std::size_t i = 0; i < outConditions.size(); ++i) {
        if (!value[i].is_boolean()) {
            return false;
        }
        outConditions[i] = value[i].get<bool>();
    }

    return true;
}

std::string LevelKey(LevelId levelId) {
    return std::to_string(static_cast<int>(levelId) + 1);
}

int CountStars(const StarConditions& conditions) {
    int stars = 0;
    for (const bool conditionMet : conditions) {
        stars += conditionMet ? 1 : 0;
    }
    return stars;
}

StarConditions CalculateConditions(const LevelResultData& resultData) {
    return StarConditions{
        resultData.passed,
        IsWithinTimeLimit(resultData),
        IsWithinStrokeLimit(resultData),
    };
}

bool UpdateBestStars(LevelId levelId, const StarConditions& conditions) {
    const auto it = g_BestConditions.find(levelId);
    if (it == g_BestConditions.end()) {
        g_BestConditions[levelId] = conditions;
        LOG_INFO("Best stars updated: level={} stars={}", static_cast<int>(levelId) + 1, CountStars(conditions));
        return true;
    }

    const int newStars = CountStars(conditions);
    const int currentStars = CountStars(it->second);

    if (newStars < currentStars) {
        return false;
    }

    if (newStars > currentStars) {
        g_BestConditions[levelId] = conditions;
        LOG_INFO("Best stars updated: level={} stars={}", static_cast<int>(levelId) + 1, newStars);
        return true;
    }

    bool hasNewCondition = false;
    for (std::size_t i = 0; i < conditions.size(); ++i) {
        if (conditions[i] && !it->second[i]) {
            hasNewCondition = true;
            break;
        }
    }

    if (hasNewCondition) {
        g_BestConditions[levelId] = conditions;
        return true;
    }

    return false;
}

}  // namespace

void LoadOrCreateDefault() {
    g_BestConditions.clear();

    std::error_code ec;
    std::filesystem::create_directories(g_SavePath.parent_path(), ec);
    if (ec) {
        LOG_WARN("Failed to create save directory '{}': {}", g_SavePath.parent_path().string(), ec.message());
    }

    if (!std::filesystem::exists(g_SavePath)) {
        if (!Save()) {
            LOG_WARN("Failed to initialize progress file: '{}'", g_SavePath.string());
        }
        return;
    }

    try {
        std::ifstream ifs(g_SavePath);
        json root;
        ifs >> root;

        if (!root.is_object() || !root.contains("levels") || !root["levels"].is_object()) {
            throw std::runtime_error("Invalid progress schema");
        }

        const auto& levels = root["levels"];
        for (auto it = levels.begin(); it != levels.end(); ++it) {
            const int levelNumber = std::stoi(it.key());
            if (levelNumber <= 0) {
                continue;
            }

            const LevelId levelId = static_cast<LevelId>(levelNumber - 1);
            StarConditions parsedConditions{false, false, false};

            if (ParseConditions(it.value(), parsedConditions)) {
                g_BestConditions[levelId] = parsedConditions;
                continue;
            }

            if (it.value().is_number_integer()) {
                const int legacyStars = it.value().get<int>();
                g_BestConditions[levelId] = ConditionsFromLegacyStars(legacyStars);
            }
        }
    } catch (const std::exception& e) {
        LOG_WARN("Progress file is invalid, fallback to empty progress. reason='{}'", e.what());
        g_BestConditions.clear();
        if (!Save()) {
            LOG_WARN("Failed to rewrite fallback progress file: '{}'", g_SavePath.string());
        }
    }
}

bool Save() {
    std::error_code ec;
    std::filesystem::create_directories(g_SavePath.parent_path(), ec);
    if (ec) {
        LOG_WARN("Failed to create save directory '{}': {}", g_SavePath.parent_path().string(), ec.message());
    }

    json root = MakeDefaultJson();
    auto& levels = root["levels"];

    for (const auto& [levelId, conditions] : g_BestConditions) {
        levels[LevelKey(levelId)] = json::array({conditions[0], conditions[1], conditions[2]});
    }

    std::ofstream ofs(g_SavePath);
    if (!ofs.is_open()) {
        LOG_WARN("Failed to open progress file for writing: '{}'", g_SavePath.string());
        return false;
    }
    ofs << root.dump(2);
    return true;
}

StarConditions GetConditions(LevelId levelId) {
    const auto it = g_BestConditions.find(levelId);
    if (it == g_BestConditions.end()) {
        return {false, false, false};
    }

    return it->second;
}

int GetTotalStars() {
    int total = 0;
    for (const auto& [_, conditions] : g_BestConditions) {
        total += CountStars(conditions);
    }
    return total;
}

bool ApplyResultAndSave(const LevelResultData& resultData) {
    const StarConditions conditions = CalculateConditions(resultData);
    if (!UpdateBestStars(resultData.levelId, conditions)) {
        return true;
    }

    if (Save()) {
        return true;
    }

    return false;
}
