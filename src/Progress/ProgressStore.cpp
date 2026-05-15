#include "Progress/ProgressStore.hpp"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "Util/Logger.hpp"

namespace {

constexpr const char* kProgressSavePath = "Resources/Save/progress.csv";
constexpr const char* kCsvHeader = "level,passed,within_time,within_stroke,remaining_time,used_strokes";

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

float CalculateRemainingTime(const LevelResultData& resultData) {
    return std::max(0.0f, resultData.goalTime - resultData.solvedTime);
}

ProgressRecord BuildRecord(const LevelResultData& resultData) {
    return ProgressRecord{
        CalculateConditions(resultData),
        CalculateRemainingTime(resultData),
        resultData.usedStroke,
    };
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

bool ParseBool01(const std::string& token, bool& out) {
    if (token == "1") {
        out = true;
        return true;
    }
    if (token == "0") {
        out = false;
        return true;
    }
    return false;
}

}  // namespace

std::filesystem::path ProgressStore::s_SavePath(kProgressSavePath);
std::unordered_map<LevelId, ProgressRecord> ProgressStore::s_Records;

void ProgressStore::LoadOrCreateDefault() {
    s_Records.clear();

    std::error_code ec;
    std::filesystem::create_directories(s_SavePath.parent_path(), ec);
    if (ec) {
        LOG_WARN("Failed to create save directory '{}': {}", s_SavePath.parent_path().string(), ec.message());
    }

    if (!std::filesystem::exists(s_SavePath)) {
        if (!Save()) {
            LOG_WARN("Failed to initialize progress file: '{}'", s_SavePath.string());
        }
        return;
    }

    std::ifstream ifs(s_SavePath);
    if (!ifs.is_open()) {
        LOG_WARN("Failed to open progress file for reading: '{}'", s_SavePath.string());
        if (!Save()) {
            LOG_WARN("Failed to rewrite fallback progress file: '{}'", s_SavePath.string());
        }
        return;
    }

    std::string line;
    bool headerSkipped = false;

    while (std::getline(ifs, line)) {
        if (!headerSkipped) {
            headerSkipped = true;
            continue;
        }
        if (line.empty()) {
            continue;
        }

        const auto tokens = SplitCsvLine(line);
        if (tokens.size() != 6) {
            LOG_WARN("Skipping malformed progress row (expected 6 columns): '{}'", line);
            continue;
        }

        try {
            const int levelNumber = std::stoi(tokens[0]);
            if (levelNumber <= 0) {
                LOG_WARN("Skipping progress row with non-positive level: '{}'", line);
                continue;
            }

            ProgressRecord record;
            if (!ParseBool01(tokens[1], record.conditions[0]) ||
                !ParseBool01(tokens[2], record.conditions[1]) ||
                !ParseBool01(tokens[3], record.conditions[2])) {
                LOG_WARN("Skipping progress row with invalid boolean: '{}'", line);
                continue;
            }
            record.remainingTime = std::stof(tokens[4]);
            record.usedStrokes = std::stoi(tokens[5]);

            const LevelId levelId = static_cast<LevelId>(levelNumber - 1);
            s_Records[levelId] = record;
        } catch (const std::exception& e) {
            LOG_WARN("Skipping unparsable progress row '{}': {}", line, e.what());
        }
    }
}

bool ProgressStore::Save() {
    std::error_code ec;
    std::filesystem::create_directories(s_SavePath.parent_path(), ec);
    if (ec) {
        LOG_WARN("Failed to create save directory '{}': {}", s_SavePath.parent_path().string(), ec.message());
    }

    std::ofstream ofs(s_SavePath);
    if (!ofs.is_open()) {
        LOG_WARN("Failed to open progress file for writing: '{}'", s_SavePath.string());
        return false;
    }

    ofs << kCsvHeader << '\n';

    std::vector<LevelId> sortedKeys;
    sortedKeys.reserve(s_Records.size());
    for (const auto& [levelId, _] : s_Records) {
        sortedKeys.push_back(levelId);
    }
    std::sort(sortedKeys.begin(), sortedKeys.end(), [](LevelId a, LevelId b) {
        return static_cast<int>(a) < static_cast<int>(b);
    });

    for (const LevelId levelId : sortedKeys) {
        const ProgressRecord& record = s_Records[levelId];
        ofs << (static_cast<int>(levelId) + 1) << ','
            << (record.conditions[0] ? 1 : 0) << ','
            << (record.conditions[1] ? 1 : 0) << ','
            << (record.conditions[2] ? 1 : 0) << ','
            << std::fixed << std::setprecision(2) << record.remainingTime
            << std::defaultfloat << ','
            << record.usedStrokes << '\n';
    }

    return true;
}

StarConditions ProgressStore::GetConditions(LevelId levelId) {
    const auto it = s_Records.find(levelId);
    if (it == s_Records.end()) {
        return {false, false, false};
    }
    return it->second.conditions;
}

int ProgressStore::GetTotalStars() {
    int total = 0;
    for (const auto& [_, record] : s_Records) {
        total += CountStars(record.conditions);
    }
    return total;
}

bool ProgressStore::ApplyResultAndSave(const LevelResultData& resultData) {
    const ProgressRecord candidate = BuildRecord(resultData);
    if (!UpdateBestRecord(resultData.levelId, candidate)) {
        return true;
    }

    if (Save()) {
        return true;
    }
    return false;
}

bool ProgressStore::UpdateBestRecord(LevelId levelId, const ProgressRecord& candidate) {
    const auto it = s_Records.find(levelId);
    if (it == s_Records.end()) {
        s_Records[levelId] = candidate;
        LOG_INFO("Progress record created: level={} stars={} remaining_time={:.2f} used_strokes={}",
                 static_cast<int>(levelId) + 1,
                 CountStars(candidate.conditions),
                 candidate.remainingTime,
                 candidate.usedStrokes);
        return true;
    }

    const int newStars = CountStars(candidate.conditions);
    const int currentStars = CountStars(it->second.conditions);

    if (newStars < currentStars) {
        return false;
    }

    if (newStars > currentStars) {
        s_Records[levelId] = candidate;
        LOG_INFO("Progress record upgraded (stars): level={} stars={}->{} remaining_time={:.2f} used_strokes={}",
                 static_cast<int>(levelId) + 1,
                 currentStars,
                 newStars,
                 candidate.remainingTime,
                 candidate.usedStrokes);
        return true;
    }

    const bool improvedTime = candidate.remainingTime > it->second.remainingTime;
    const bool improvedStrokes = candidate.usedStrokes < it->second.usedStrokes;

    if (improvedTime || improvedStrokes) {
        s_Records[levelId] = candidate;
        LOG_INFO("Progress record upgraded (tiebreak): level={} stars={} remaining_time={:.2f} used_strokes={}",
                 static_cast<int>(levelId) + 1,
                 newStars,
                 candidate.remainingTime,
                 candidate.usedStrokes);
        return true;
    }

    return false;
}
