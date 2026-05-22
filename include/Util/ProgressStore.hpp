#ifndef UTIL_PROGRESS_STORE_HPP
#define UTIL_PROGRESS_STORE_HPP

#include <array>
#include <map>

#include "Level/Level.hpp"
#include "Level/LevelData.hpp"

namespace Util {

struct ProgressRecord {
    std::string screenshotFilename;
    std::array<bool, 3> stars{false, false, false};
    float remainingTime = 0.0f;
    int usedStrokes = 0;
};

class ProgressStore {
   public:
    // 從 CSV 載入進度；若檔案不存在則建立空檔
    static void LoadOrCreateDefault();
    // 回傳指定關卡的 3 顆星條件 [passed, within_time, within_stroke]
    static std::array<bool, 3> GetStars(LevelId levelId);
    // 統計所有關卡的總星數
    static int GetTotalStarCount();
    // 將 LevelResult 轉為 ProgressRecord，呼叫 UpdateBestRecord 更新最佳記錄，再存檔
    // 回傳是否為新紀錄
    static bool ApplyResultAndSave(const LevelResult& resultData);
    // 取得截圖檔路徑
    static std::string GetScreenshotPath(LevelId levelId);

   private:
    static std::map<LevelId, ProgressRecord> s_Records;
};

}  // namespace Util

#endif  // UTIL_PROGRESS_STORE_HPP
