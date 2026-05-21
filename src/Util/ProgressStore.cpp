#include "Util/ProgressStore.hpp"
#include "Util/Screenshot.hpp"
#include "App.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "Util/Logger.hpp"

namespace Util {

namespace {

constexpr const char* kCsvHeader = "level,screenshot,passed,within_time,within_stroke,remaining_time,used_strokes";
constexpr const char* kProgressSavePath = "Resources/Save/progress.csv";

int CountStars(const std::array<bool, 3>& c) {
    return c[0] + c[1] + c[2];
}

std::vector<std::string> SplitCsvLine(const std::string& line) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream iss(line);
    while (std::getline(iss, token, ',')) {
        tokens.push_back(token);
    }
    return tokens;
}

bool WriteCSV(std::map<LevelId, ProgressRecord> &record) {
    std::error_code ec;
    std::filesystem::path savePath(kProgressSavePath);
    std::filesystem::create_directories(savePath.parent_path(), ec);

    std::ofstream ofs(savePath);
    if (!ofs.is_open()) {
        LOG_WARN("Failed to open progress file for writing: '{}'", savePath.string());
        return false;
    }

    ofs << kCsvHeader << '\n';
    for (const auto& [levelId, r] : record) {
        ofs << (static_cast<int>(levelId) + 1) << ','
            << (r.stars[0] ? 1 : 0) << ','
            << (r.stars[1] ? 1 : 0) << ','
            << (r.stars[2] ? 1 : 0) << ','
            << std::fixed << std::setprecision(2) << r.remainingTime << std::defaultfloat << ','
            << r.usedStrokes << '\n';
    }
    return true;
}

}  // namespace

void ProgressStore::LoadOrCreateDefault() {
    s_Records.clear();

    std::error_code ec;
    std::filesystem::path savePath(kProgressSavePath);
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
        if (tokens.size() != 6) {
            LOG_WARN("Skipping malformed progress row: '{}'", line);
            continue;
        }

        try {
            const int levelNumber = std::stoi(tokens[0]);
            if (levelNumber <= 0) continue;

            s_Records[static_cast<LevelId>(levelNumber - 1)] = ProgressRecord{
                {tokens[1] == "1", tokens[2] == "1", tokens[3] == "1"},
                std::stof(tokens[4]),
                std::stoi(tokens[5]),
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

int ProgressStore::GetTotalStarCount() {
    int total = 0;
    for (const auto& [_, record] : s_Records) {
        total += CountStars(record.stars);
    }
    return total;
}

void ProgressStore::ApplyResultAndSave(const LevelResultData& data) {
    s_LastPlayedLevelData = data;
    std::string id = std::to_string(static_cast<int>(data.levelId) + 1);
    Screenshot::Capture(id);
    // todo: 更新 s_Records 

    WriteCSV(s_Records);
}

}  // namespace Util
