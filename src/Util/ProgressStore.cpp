#include "Util/ProgressStore.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "Util/Logger.hpp"

namespace Util {

namespace {

constexpr const char* kCsvHeader = "level,timestamp,passed,within_time,within_stroke,remaining_time,used_strokes";
constexpr const char* kBestRecordSavePath = "Resources/Save/BestRecord.csv";

std::vector<std::string> SplitCsvLine(const std::string& line) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream iss(line);
    while (std::getline(iss, token, ',')) {
        tokens.push_back(token);
    }
    return tokens;
}

bool WriteCSV(std::map<LevelId, ProgressRecord>& record) {
    std::error_code ec;
    std::filesystem::path savePath(kBestRecordSavePath);
    std::filesystem::create_directories(savePath.parent_path(), ec);

    std::ofstream ofs(savePath);
    if (!ofs.is_open()) {
        LOG_WARN("Failed to open progress file for writing: '{}'", savePath.string());
        return false;
    }

    ofs << kCsvHeader << '\n';
    for (const auto& [levelId, r] : record) {
        ofs << (static_cast<int>(levelId) + 1) << ','
            << r.screenshotFilename << ','
            << (r.stars[0] ? 1 : 0) << ','
            << (r.stars[1] ? 1 : 0) << ','
            << (r.stars[2] ? 1 : 0) << ','
            << std::fixed << std::setprecision(2) << r.remainingTime << std::defaultfloat << ','
            << r.usedStrokes << '\n';
    }
    return true;
}

}  // namespace

LevelResultData ProgressStore::s_LastPlayedLevelData;
std::map<LevelId, ProgressRecord> ProgressStore::s_Records;

void ProgressStore::LoadOrCreateDefault() {
    s_Records.clear();

    std::error_code ec;
    std::filesystem::path savePath(kBestRecordSavePath);
    std::filesystem::create_directories(savePath.parent_path(), ec);

    if (!std::filesystem::exists(savePath)) {
        WriteCSV(s_Records);
        return;
    }

    std::ifstream ifs(savePath);
    if (!ifs.is_open()) {
        LOG_WARN("Failed to open progress file for reading: '{}'", savePath.string());
        return;
    }

    std::string line;
    std::getline(ifs, line);  // skip header

    while (std::getline(ifs, line)) {
        if (line.empty()) continue;

        const auto tokens = SplitCsvLine(line);
        if (tokens.size() != 7) {
            LOG_WARN("Skipping malformed progress row: '{}'", line);
            continue;
        }

        try {
            const int levelNumber = std::stoi(tokens[0]);
            if (levelNumber <= 0) continue;

            s_Records[static_cast<LevelId>(levelNumber - 1)] = ProgressRecord{
                tokens[1],
                {tokens[2] == "1", tokens[3] == "1", tokens[4] == "1"},
                std::stof(tokens[5]),
                std::stoi(tokens[6]),
            };
        } catch (const std::exception& e) {
            LOG_WARN("Skipping unparsable progress row '{}': {}", line, e.what());
        }
    }
}

std::array<bool, 3> ProgressStore::GetStars(LevelId levelId) {
    if (auto it = s_Records.find(levelId); it != s_Records.end()) {
        return it->second.stars;
    }
    return std::array<bool, 3>{false, false, false};
}

std::string ProgressStore::GetScreenshotPath(LevelId levelId) {
    if (auto it = s_Records.find(levelId); it != s_Records.end() && !it->second.screenshotFilename.empty()) {
        return "Resources/Save/Screenshots/" + it->second.screenshotFilename;
    }
    return "Resources/Images/level_frame.png";
}

int ProgressStore::GetTotalStarCount() {
    int total = 0;
    for (const auto& [_, record] : s_Records) {
        total += record.stars[0] + record.stars[1] + record.stars[2];
    }
    return total;
}

void ProgressStore::ApplyResultAndSave(const LevelResultData& data) {
    s_LastPlayedLevelData = data;

    std::array<bool, 3> newStars = {
        data.passed,
        IsWithinTimeLimit(data),
        IsWithinStrokeLimit(data)
    };
    int newStarCount = newStars[0] + newStars[1] + newStars[2];
    float newRemainingTime = std::max(0.0f, data.goalTime - data.solvedTime);

    bool isNewRecord = false;
    if (auto it = s_Records.find(data.levelId); it != s_Records.end()) {
        const auto& oldRecord = it->second;
        int oldStarCount = oldRecord.stars[0] + oldRecord.stars[1] + oldRecord.stars[2];

        if (newStarCount > oldStarCount) {
            isNewRecord = true;
        } else if (newStarCount == oldStarCount) {
            if (data.usedStroke < oldRecord.usedStrokes) {
                isNewRecord = true;
            } else if (data.usedStroke == oldRecord.usedStrokes) {
                if (newRemainingTime > oldRecord.remainingTime) {
                    isNewRecord = true;
                }
            }
        }
    } else {
        isNewRecord = true;
    }

    if (isNewRecord) {
        s_Records[data.levelId] = ProgressRecord{
            data.screenshotFilename,
            newStars,
            newRemainingTime,
            data.usedStroke
        };
        WriteCSV(s_Records);
    }
}

void ProgressStore::CleanUpUnusedScreenshots() {
    std::vector<std::string> usedScreenshots;
    for (const auto& [_, record] : s_Records) {
        if (!record.screenshotFilename.empty()) {
            usedScreenshots.push_back(record.screenshotFilename);
        }
    }

    std::filesystem::path screenshotDir("Resources/Save/Screenshots");
    if (!std::filesystem::exists(screenshotDir)) {
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(screenshotDir)) {
        if (!entry.is_regular_file()) continue;

        std::string filename = entry.path().filename().string();
        if (std::find(usedScreenshots.begin(), usedScreenshots.end(), filename) == usedScreenshots.end()) {
            std::error_code ec;
            std::filesystem::remove(entry.path(), ec);
            if (ec) {
                LOG_WARN("Failed to delete unused screenshot: {}", entry.path().string());
            } else {
                LOG_INFO("Deleted unused screenshot: {}", entry.path().string());
            }
        }
    }
}

}  // namespace Util
