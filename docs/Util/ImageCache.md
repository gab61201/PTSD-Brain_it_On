# ImageCache

**標頭檔**：[`include/Util/ImageCache.hpp`](../../include/Util/ImageCache.hpp)

基於 C++17 `inline` 全局變數設計的唯讀圖片快取機制，包裝了 PTSD 的 `Util::AssetStore` 系統。確保遊戲中相同的圖片素材在執行期間只會從硬碟載入並上傳至 GPU 一次，從而避免嚴重的記憶體累積與 I/O 卡頓。

#### 全局變數

```cpp
namespace Util {
    inline Util::AssetStore<std::shared_ptr<Util::Image>> ImageCache;
}
```

#### 使用方法

你可以直接透過 `Util::ImageCache` 物件進行操作，不需實例化或呼叫 `GetInstance()`：

```cpp
// 取得並快取圖片
std::shared_ptr<Util::Image> image = Util::ImageCache.Get(Path::WhiteCircle);

// 直接應用於 GameObject
m_Visual->SetDrawable(Util::ImageCache.Get(Path::WhiteCircle));
```

#### 運作特點

1. **自動生命週期管理**：快取內部以 `std::shared_ptr<Util::Image>` 儲存圖片物件，當遊戲關閉時會由 C++ 運行期自動安全地釋放，無需手動 `delete`。
2. **常駐資源（Resident Assets）**：已載入的素材會常駐於記憶體中，保證在關卡切換或重複劃線時能瞬間取得，避免二次 Disk I/O，是 2D 物理遊戲中防卡頓的關鍵優化。
