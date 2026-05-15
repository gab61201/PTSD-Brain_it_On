#ifndef PROGRESS_STORE_HPP
#define PROGRESS_STORE_HPP

#include <array>
#include <filesystem>
#include <unordered_map>

#include "Level/Level.hpp"

using StarConditions = std::array<bool, 3>;

struct ProgressRecord {
    StarConditions conditions{false, false, false};
    float remainingTime = 0.0f;
    int usedStrokes = 0;
};

class ProgressStore {
public:
    static void LoadOrCreateDefault();
    static bool Save();

    static StarConditions GetConditions(LevelId levelId);
    static int GetTotalStars();
    static bool ApplyResultAndSave(const LevelResultData& resultData);

private:
    static bool UpdateBestRecord(LevelId levelId, const ProgressRecord& candidate);

    static std::filesystem::path s_SavePath;
    static std::unordered_map<LevelId, ProgressRecord> s_Records;
};

#endif
