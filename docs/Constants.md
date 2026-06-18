# Constants

**標頭檔**：[`include/Constants.hpp`](../include/Constants.hpp)

全域常數定義。

### 常數值

| 常數 | 型別 | 值 | 說明 |
|------|------|-----|------|
| `RESOLUTION_X` | `float` | `1280.0` | 視窗寬度（像素） |
| `RESOLUTION_Y` | `float` | `720.0` | 視窗高度（像素） |
| `FPS` | `int` | `60` | 目標幀率 |
| `STROKE_WIDTH` | `float` | `10.0` | 繪製線段寬度（像素） |
| `MIN_STROKE_LENGTH` | `float` | `2.0` | 最小筆畫長度（像素） |
| `kCsvHeader` | `const char*` | — | CSV 存檔標頭列 |
| `kBestRecordSavePath` | `const char*` | `"Resources/Save/BestRecord.csv"` | 最佳紀錄檔案路徑 |
| `BASIC_SHAPE_IMAGE_SIZE` | `float` | `417.0` | 基本形狀圖片尺寸（像素） |
| `LEVEL_BACKGROUND_IMAGE_SIZE` | `float` | `886.0` | 關卡背景圖片尺寸（像素） |

### `Path` 命名空間

提供所有資源圖片路徑的靜態字串常數：

| 常數 | 路徑 |
|------|------|
| `Path::Background` | `Resources/Images/background.png` |
| `Path::LevelFrame` | `Resources/Images/level_frame.png` |
| `Path::Alarm` | `Resources/Images/alarm.png` |
| `Path::StrokeLimit` | `Resources/Images/stroke_limit.png` |
| `Path::BlueSquare` | `Resources/Images/BasicShapes/blue_square.png` |
| `Path::LightBlueSquare` | `Resources/Images/BasicShapes/light_blue_square.png` |
| `Path::OrangeSquare` | `Resources/Images/BasicShapes/orange_square.png` |
| `Path::WhiteSquare` | `Resources/Images/BasicShapes/white_square.png` |
| `Path::WhiteCircle` | `Resources/Images/BasicShapes/white_circle.png` |
| `Path::RedSquareTrans` | `Resources/Images/BasicShapes/red_square_trans.png` |
| `Path::RedCircleTrans` | `Resources/Images/BasicShapes/red_circle_trans.png` |
| `Path::RedSquare` | `Resources/Images/BasicShapes/red_square.png` |
| `Path::RedCircle` | `Resources/Images/BasicShapes/red_circle.png` |
| `Path::OrangeCircle` | `Resources/Images/BasicShapes/orange_circle.png` |
| `Path::RedLine` | `Resources/Images/BasicShapes/red_line.png` |
| `Path::StarBright` | `Resources/Images/star_bright.png` |
| `Path::StarDark` | `Resources/Images/star_dark.png` |
| `Path::LevelBackground` | `Resources/Images/level_background.png` |
| `Path::BtnBack` | `Resources/Images/Btn_Back.png` |
| `Path::BtnRetry` | `Resources/Images/Btn_Retry.png` |
| `Path::BtnNext` | `Resources/Images/Btn_Next.png` |
| `Path::BtnMainButtonGray` | `Resources/Images/Btn_MainButton_Gray.png` |
