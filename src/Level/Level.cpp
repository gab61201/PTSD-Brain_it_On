#include "Level/Level.hpp"

#include <iomanip>
#include <sstream>

#include "Util/Color.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Text.hpp"
#include "Util/Time.hpp"

Level::Level(LevelId levelId) : m_LevelId(levelId) {
    LevelData data = GetLevelData(levelId);
    m_World = data.world;
    m_Timeout = data.timeout;

    // 1. 提示文字 (設為白色)
    m_TargetText = std::make_shared<Util::GameObject>();
    if (!data.targetText.empty()) {
        m_TargetText->SetDrawable(std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 30, data.targetText, Util::Color(255, 255, 255)));
        m_TargetText->m_Transform.translation = {0.0f, 250.0f};
    }

    // 2. 關卡編號文字 (設為白色)
    float leftUI_X = -520.0f;
    int levelNum = static_cast<int>(levelId) + 1;
    auto levelNumTextDrawable = std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 36, std::to_string(levelNum), Util::Color(255, 255, 255));
    m_LevelNumberText = std::make_shared<Util::GameObject>(levelNumTextDrawable, 0.5f);
    m_LevelNumberText->m_Transform.translation = {leftUI_X + 8, 290.0f};

    // 3. 計時器文字 (設為白色)
    m_TimerText = std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 26, "0.0", Util::Color(255, 255, 255));
    m_TimerObject = std::make_shared<Util::GameObject>(m_TimerText, 0.5f);
    m_TimerObject->m_Transform.translation = {leftUI_X + 7, 110.0f};
}

void Level::Waiting() {
    // 檢查使用者是否開始繪圖
    if (m_World && Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        m_state = state::DRAWING;
        m_World->Start();
        m_World->DrawObject(Util::Input::GetCursorPosition());
    }
}

void Level::Drawing() {
    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
        m_state = state::PLAYING;
        m_World->EndDrawing();
    } else {
        m_World->DrawObject(Util::Input::GetCursorPosition());
    }
}

void Level::Playing() {
    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        m_state = state::DRAWING;
        m_World->DrawObject(Util::Input::GetCursorPosition());
    }
    // 檢查通關條件
    if (m_World->IsPassed()) {
        m_state = state::FINISHED;
        m_World->Stop();
    }
}

void Level::Finished() {
    // 停止世界，渲染結算畫面
    auto title = std::make_shared<Util::GameObject>();
    title->SetDrawable(
        std::make_shared<Util::Text>("PTSD/assets/fonts/Inter.ttf", 48, "Finished"));
    title->Draw();
}

void Level::Reset() {
    m_state = state::WAITING;
    m_World = GetLevelData(m_LevelId).world;
    m_Time = 0.0F;
}

void Level::Update() {
    // 只有在繪圖或播放物理模擬時才計時 (使用 GetDeltaTimeMs() 並除以 1000)
    if (m_state == state::DRAWING || m_state == state::PLAYING) {
        m_Time += static_cast<float>(Util::Time::GetDeltaTimeMs()) / 1000.0f;
    }

    // 更新計時器文字內容 (直接使用存好的 Text 指標)
    if (m_TimerText) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << GetRemainingTime();
        m_TimerText->SetText(ss.str());
    }

    switch (m_state) {
        case state::WAITING:
            Waiting();
            // 玩家繪圖前（WAITING 狀態）顯示提示文字
            if (m_TargetText) {
                m_TargetText->Draw();
            }
            break;
        case state::DRAWING:
            Drawing();
            Playing();
            break;
        case state::PLAYING:
            Playing();
            break;
        case state::FINISHED:
            Finished();
            break;
    }

    // 繪製動態文字
    if (m_LevelNumberText) m_LevelNumberText->Draw();
    if (m_TimerObject) m_TimerObject->Draw();

    // 繪製物體
    if (m_World) {
        m_World->Update();
    }
}
