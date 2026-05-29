#pragma once

#include <string>

namespace Util {

class Screenshot {
   public:
    // 截圖並儲存為檔案，回傳檔名
    static std::string Capture();

    // 依檔名刪除截圖檔案（檔名為空或檔案不存在則忽略）
    static void Remove(const std::string& filename);
};

}  // namespace Util
