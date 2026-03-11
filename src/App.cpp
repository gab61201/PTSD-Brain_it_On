#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;
}

void App::Update() {
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
        if (new_drawing == nullptr){
            new_drawing = new DrawnObject(Util::Input::GetCursorPosition());
        }
        else{
            new_drawing->next_stroke(Util::Input::GetCursorPosition());
        }
    }
    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
        if (new_drawing != nullptr){
            drawn_objects.push_back(new_drawing);
            new_drawing = nullptr;
        }
    }
    if (new_drawing != nullptr){
        new_drawing->render();
    }
    for (auto& object : drawn_objects){
        object->render();
    }
    //TODO: do your things here and delete this line <3
    
    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
