#ifndef PROGRESS_STORE_HPP
#define PROGRESS_STORE_HPP

#include <array>
#include <filesystem>
#include <unordered_map>

#include "Level/Level.hpp"

// Star conditions: [passed, time, stroke]
using StarConditions = std::array<bool, 3>;

class ProgressStore {
   public:
    ProgressStore();

    void LoadOrCreateDefault();
    bool Save() const;

    // Query APIs
    StarConditions GetConditions(LevelId levelId) const;
    int GetTotalStars() const;

    // Update APIs - only upgrade strategy
    bool UpdateBestStars(LevelId levelId, const StarConditions& conditions);

    // Static helpers
    static StarConditions CalculateConditions(const LevelResultData& resultData);

   private:
    static std::string LevelKey(LevelId levelId);
    static int CountStars(const StarConditions& conditions);

    std::filesystem::path m_SavePath;
    // Stores per-level best conditions: [passed, time, stroke]
    std::unordered_map<LevelId, StarConditions> m_BestConditions;
};

#endif