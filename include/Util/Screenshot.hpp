#ifndef UTIL_SCREENSHOT_HPP
#define UTIL_SCREENSHOT_HPP

#include <string>

namespace Util {

class Screenshot {
   public:
    // 截圖並儲存為檔案，回傳檔名
    static std::string Capture();
};

}  // namespace Util

#endif  // UTIL_SCREENSHOT_HPP
