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

int CountStars(const StarConditions& c) {
    return (c[0] ? 1 : 0) + (c[1] ? 1 : 0) + (c[2] ? 1 : 0);
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

}  // namespace

std::filesystem::path ProgressStore::s_SavePath(kProgressSavePath);
std::map<LevelId, ProgressRecord> ProgressStore::s_Records;

void ProgressStore::LoadOrCreateDefault() {
    s_Records.clear();

    std::error_code ec;
    std::filesystem::create_directories(s_SavePath.parent_path(), ec);

    if (!std::filesystem::exists(s_SavePath)) {
        Save();
        return;
    }

    std::ifstream ifs(s_SavePath);
    if (!ifs.is_open()) {
        LOG_WARN("Failed to open progress file for reading: '{}'", s_SavePath.string());
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

bool ProgressStore::Save() {
    std::error_code ec;
    std::filesystem::create_directories(s_SavePath.parent_path(), ec);

    std::ofstream ofs(s_SavePath);
    if (!ofs.is_open()) {
        LOG_WARN("Failed to open progress file for writing: '{}'", s_SavePath.string());
        return false;
    }

    ofs << kCsvHeader << '\n';
    for (const auto& [levelId, r] : s_Records) {
        ofs << (static_cast<int>(levelId) + 1) << ','
            << (r.conditions[0] ? 1 : 0) << ','
            << (r.conditions[1] ? 1 : 0) << ','
            << (r.conditions[2] ? 1 : 0) << ','
            << std::fixed << std::setprecision(2) << r.remainingTime << std::defaultfloat << ','
            << r.usedStrokes << '\n';
    }
    return true;
}

StarConditions ProgressStore::GetConditions(LevelId levelId) {
    const auto it = s_Records.find(levelId);
    return it == s_Records.end() ? StarConditions{false, false, false} : it->second.conditions;
}

int ProgressStore::GetTotalStars() {
    int total = 0;
    for (const auto& [_, record] : s_Records) {
        total += CountStars(record.conditions);
    }
    return total;
}

bool ProgressStore::ApplyResultAndSave(const LevelResultData& r) {
    const ProgressRecord candidate{
        {r.passed, IsWithinTimeLimit(r), IsWithinStrokeLimit(r)},
        std::max(0.0f, r.goalTime - r.solvedTime),
        r.usedStroke,
    };
    if (!UpdateBestRecord(r.levelId, candidate)) return true;
    return Save();
}

bool ProgressStore::UpdateBestRecord(LevelId levelId, const ProgressRecord& candidate) {
    const auto it = s_Records.find(levelId);
    const int newStars = CountStars(candidate.conditions);
    const int oldStars = it == s_Records.end() ? -1 : CountStars(it->second.conditions);

    bool shouldUpdate = newStars > oldStars;
    if (!shouldUpdate && newStars == oldStars) {
        shouldUpdate = candidate.remainingTime > it->second.remainingTime ||
                       candidate.usedStrokes < it->second.usedStrokes;
    }
    if (!shouldUpdate) return false;

    s_Records[levelId] = candidate;
    LOG_INFO("Progress updated: level={} stars={} remaining_time={:.2f} used_strokes={}",
             static_cast<int>(levelId) + 1, newStars,
             candidate.remainingTime, candidate.usedStrokes);
    return true;
}
