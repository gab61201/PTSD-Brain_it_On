#ifndef PROGRESS_STORE_HPP
#define PROGRESS_STORE_HPP

#include <filesystem>

#include "Level/Level.hpp"

class ProgressStore {
   public:
    explicit ProgressStore(const std::filesystem::path& savePath = DefaultSavePath());

    void LoadOrCreateDefault();
    bool Save() const;

    int GetBestStars(LevelId levelId) const;
    bool UpdateBestStars(LevelId levelId, int stars);
    int GetTotalStars() const;

    static int CalculateStars(const LevelResultData& resultData);

   private:
    static std::filesystem::path DefaultSavePath();
    static std::string LevelKey(LevelId levelId);
    static int ClampStars(int stars);

    std::filesystem::path m_SavePath;
    std::unordered_map<LevelId, int> m_BestStars;
};

#endif