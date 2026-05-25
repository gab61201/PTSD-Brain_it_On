#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>

inline constexpr float RESOLUTION_X = 1280.0F;
inline constexpr float RESOLUTION_Y = 720.0F;
inline constexpr int FPS = 60;
inline constexpr float STROKE_WIDTH = 10.0F;
inline constexpr float MIN_STROKE_LENGTH = 2.0F;

inline constexpr const char* kCsvHeader = "level,timestamp,passed,within_time,within_stroke,remaining_time,used_strokes";
inline constexpr const char* kBestRecordSavePath = "Resources/Save/BestRecord.csv";
inline constexpr float BASIC_SHAPE_IMAGE_SIZE = 417.0F;
inline constexpr float LEVEL_BACKGROUND_IMAGE_SIZE = 886.0F;

namespace Path {

inline const std::string Background = "Resources/Images/background.png";
inline const std::string LevelFrame = "Resources/Images/level_frame.png";
inline const std::string Alarm = "Resources/Images/alarm.png";
inline const std::string StrokeLimit = "Resources/Images/stroke_limit.png";
inline const std::string BlueSquare = "Resources/Images/BasicShapes/blue_square.png";
inline const std::string LightBlueSquare = "Resources/Images/BasicShapes/light_blue_square.png";
inline const std::string OrangeSquare = "Resources/Images/BasicShapes/orange_square.png";
inline const std::string WhiteSquare = "Resources/Images/BasicShapes/white_square.png";
inline const std::string WhiteCircle = "Resources/Images/BasicShapes/white_circle.png";
inline const std::string RedSquareTrans = "Resources/Images/BasicShapes/red_square_trans.png";
inline const std::string RedCircleTrans = "Resources/Images/BasicShapes/red_circle_trans.png";
inline const std::string StarBright = "Resources/Images/star_bright.png";
inline const std::string StarDark = "Resources/Images/star_dark.png";
inline const std::string LevelBackground = "Resources/Images/level_background.png";
inline const std::string BtnBack = "Resources/Images/Btn_Back.png";
inline const std::string BtnRetry = "Resources/Images/Btn_Retry.png";
inline const std::string BtnNext = "Resources/Images/Btn_Next.png";
inline const std::string BtnMainButtonGray = "Resources/Images/Btn_MainButton_Gray.png";

}  // namespace Path

#endif  // CONSTANTS_HPP
