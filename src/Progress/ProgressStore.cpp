#include "Progress/ProgressStore.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "Util/Logger.hpp"

namespace {

using json = nlohmann::json;

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

constexpr const char* kProgressSavePath = "Resources/Save/progress.json";

}  // namespace

ProgressStore::ProgressStore() : m_SavePath(kProgressSavePath) {}

std::string ProgressStore::LevelKey(LevelId levelId) {
    return std::to_string(static_cast<int>(levelId) + 1);
}

int ProgressStore::CountStars(const StarConditions& conditions) {
    int stars = 0;
    for (const bool conditionMet : conditions) {
        stars += conditionMet ? 1 : 0;
    }
    return stars;
}

void ProgressStore::LoadOrCreateDefault() {
    m_BestConditions.clear();

    std::error_code ec;
    std::filesystem::create_directories(m_SavePath.parent_path(), ec);
    if (ec) {
        LOG_WARN("Failed to create save directory '{}': {}", m_SavePath.parent_path().string(), ec.message());
    }

    if (!std::filesystem::exists(m_SavePath)) {
        if (!Save()) {
            LOG_WARN("Failed to initialize progress file: '{}'", m_SavePath.string());
        }
        return;
    }

    try {
        std::ifstream ifs(m_SavePath);
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
                m_BestConditions[levelId] = parsedConditions;
                continue;
            }

            if (it.value().is_number_integer()) {
                const int legacyStars = it.value().get<int>();
                m_BestConditions[levelId] = ConditionsFromLegacyStars(legacyStars);
            }
        }
    } catch (const std::exception& e) {
        LOG_WARN("Progress file is invalid, fallback to empty progress. reason='{}'", e.what());
        m_BestConditions.clear();
        if (!Save()) {
            LOG_WARN("Failed to rewrite fallback progress file: '{}'", m_SavePath.string());
        }
    }
}

bool ProgressStore::Save() const {
    std::error_code ec;
    std::filesystem::create_directories(m_SavePath.parent_path(), ec);
    if (ec) {
        LOG_WARN("Failed to create save directory '{}': {}", m_SavePath.parent_path().string(), ec.message());
    }

    json root = MakeDefaultJson();
    auto& levels = root["levels"];

    for (const auto& [levelId, conditions] : m_BestConditions) {
        levels[LevelKey(levelId)] = json::array({conditions[0], conditions[1], conditions[2]});
    }

    std::ofstream ofs(m_SavePath);
    if (!ofs.is_open()) {
        LOG_WARN("Failed to open progress file for writing: '{}'", m_SavePath.string());
        return false;
    }
    ofs << root.dump(2);
    return true;
}

StarConditions ProgressStore::GetConditions(LevelId levelId) const {
    const auto it = m_BestConditions.find(levelId);
    if (it == m_BestConditions.end()) {
        return {false, false, false};
    }

    return it->second;
}

bool ProgressStore::UpdateBestStars(LevelId levelId, const StarConditions& conditions) {
    const auto it = m_BestConditions.find(levelId);
    if (it == m_BestConditions.end()) {
        m_BestConditions[levelId] = conditions;
        LOG_INFO("Best stars updated: level={} stars={}", static_cast<int>(levelId) + 1, CountStars(conditions));
        return true;
    }

    const int newStars = CountStars(conditions);
    const int currentStars = CountStars(it->second);

    if (newStars < currentStars) {
        // Downgrade attempt - reject
        return false;
    }

    if (newStars > currentStars) {
        // Upgrade - accept
        m_BestConditions[levelId] = conditions;
        LOG_INFO("Best stars updated: level={} stars={}", static_cast<int>(levelId) + 1, newStars);
        return true;
    }

    // Equal stars: check if new is superset of old
    bool hasNewCondition = false;
    for (std::size_t i = 0; i < conditions.size(); ++i) {
        if (conditions[i] && !it->second[i]) {
            hasNewCondition = true;
            break;
        }
    }

    if (hasNewCondition) {
        m_BestConditions[levelId] = conditions;
        return true;
    }

    return false;
}

bool ProgressStore::ApplyResultAndSave(const LevelResultData& resultData) {
    const StarConditions conditions = CalculateConditions(resultData);
    if (!UpdateBestStars(resultData.levelId, conditions)) {
        // nothing to save
        return true;
    }

    if (Save()) {
        return true;
    }

    return false;
}

int ProgressStore::GetTotalStars() const {
    int total = 0;
    for (const auto& [_, conditions] : m_BestConditions) {
        total += CountStars(conditions);
    }
    return total;
}

StarConditions ProgressStore::CalculateConditions(const LevelResultData& resultData) {
    return StarConditions{
        resultData.passed,
        IsWithinTimeLimit(resultData),
        IsWithinStrokeLimit(resultData),
    };
}
