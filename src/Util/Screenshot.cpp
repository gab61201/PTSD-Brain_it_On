#include "Util/Screenshot.hpp"

#include <cstdio>
#include <filesystem>
#include <vector>
#include <algorithm>

#include "pch.hpp"
#include "config.hpp"
#include "Util/Logger.hpp"

namespace Util {

void Screenshot::Capture(std::string filename) {
    const int screenshotWidth = 840;
    const int screenshotHeight = 640;
    const int xOff = (WINDOW_WIDTH - screenshotWidth) / 2;
    const int yOff = (WINDOW_HEIGHT - screenshotHeight) / 2;
    const int stride = screenshotWidth * 4;

    std::vector<Uint8> buffer(static_cast<size_t>(screenshotHeight) * stride);
    glReadPixels(xOff, yOff, screenshotWidth, screenshotHeight, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());

    for (int y1 = 0, y2 = screenshotHeight - 1; y1 < y2; ++y1, --y2) {
        Uint8 *row1 = buffer.data() + static_cast<size_t>(y1) * stride;
        Uint8 *row2 = buffer.data() + static_cast<size_t>(y2) * stride;
        std::swap_ranges(row1, row1 + stride, row2);
    }

    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(
        buffer.data(), screenshotWidth, screenshotHeight, 32, stride,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
        0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000
#else
        0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
#endif
    );

    if (surface == nullptr) {
        LOG_ERROR("Failed to create SDL surface for screenshot: {}", SDL_GetError());
        return;
    }

    std::filesystem::path filepath("Resources/Save/LevelScreenshots/" + filename + ".bmp");
    if (filepath.has_parent_path()) {
        std::filesystem::create_directories(filepath.parent_path());
    }

    if (SDL_SaveBMP(surface, filepath.c_str()) != 0) {
        LOG_ERROR("Failed to save screenshot: {}", SDL_GetError());
    } else {
        LOG_INFO("Screenshot saved: {}", filepath.c_str());
    }

    SDL_FreeSurface(surface);
}

} // namespace Util
