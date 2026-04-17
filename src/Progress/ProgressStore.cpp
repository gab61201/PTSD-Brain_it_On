#include "Progress/ProgressStore.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

#include "Util/Logger.hpp"

namespace {

using json = nlohmann::json;

json MakeDefaultJson() {
    return json{{"levels", json::object()}};
}

}  // namespace

ProgressStore::ProgressStore(const std::filesystem::path& savePath) : m_SavePath(savePath) {}

std::filesystem::path ProgressStore::DefaultSavePath() {
    return std::filesystem::path(RESOURCE_DIR) / "Save" / "progress.json";
}

std::string ProgressStore::LevelKey(LevelId levelId) {
    return std::to_string(static_cast<int>(levelId) + 1);
}

int ProgressStore::ClampStars(int stars) {
    return std::max(0, std::min(3, stars));
}

void ProgressStore::LoadOrCreateDefault() {
    m_BestStars.clear();

    std::error_code ec;
    std::filesystem::create_directories(m_SavePath.parent_path(), ec);
    if (ec) {
        LOG_WARN("Failed to create save directory '{}': {}", m_SavePath.parent_path().string(), ec.message());
    }

    if (!std::filesystem::exists(m_SavePath)) {
        Save();
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
            if (!it.value().is_number_integer()) {
                continue;
            }
            const int levelNumber = std::stoi(it.key());
            if (levelNumber <= 0) {
                continue;
            }
            const LevelId levelId = static_cast<LevelId>(levelNumber - 1);
            m_BestStars[levelId] = ClampStars(it.value().get<int>());
        }
    } catch (const std::exception& e) {
        LOG_WARN("Progress file is invalid, fallback to empty progress. reason='{}'", e.what());
        m_BestStars.clear();
        Save();
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

    for (const auto& [levelId, stars] : m_BestStars) {
        levels[LevelKey(levelId)] = ClampStars(stars);
    }

    std::ofstream ofs(m_SavePath);
    if (!ofs.is_open()) {
        LOG_WARN("Failed to open progress file for writing: '{}'", m_SavePath.string());
        return false;
    }
    ofs << root.dump(2);
    return true;
}

int ProgressStore::GetBestStars(LevelId levelId) const {
    const auto it = m_BestStars.find(levelId);
    if (it == m_BestStars.end()) {
        return 0;
    }
    return ClampStars(it->second);
}

bool ProgressStore::UpdateBestStars(LevelId levelId, int stars) {
    const int clampedStars = ClampStars(stars);
    const int currentBest = GetBestStars(levelId);

    if (clampedStars <= currentBest) {
        return false;
    }

    m_BestStars[levelId] = clampedStars;
    return true;
}

int ProgressStore::GetTotalStars() const {
    int total = 0;
    for (const auto& [_, stars] : m_BestStars) {
        total += ClampStars(stars);
    }
    return total;
}

int ProgressStore::CalculateStars(const LevelResultData& resultData) {
    const int conditionPassed = resultData.passed ? 1 : 0;
    const int conditionTime = IsWithinTimeLimit(resultData) ? 1 : 0;
    const int conditionStroke = IsWithinStrokeLimit(resultData) ? 1 : 0;
    return conditionPassed + conditionTime + conditionStroke;
}
