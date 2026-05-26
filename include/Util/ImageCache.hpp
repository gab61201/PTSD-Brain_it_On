#pragma once

#include <memory>

#include "Util/AssetStore.hpp"
#include "Util/Image.hpp"

namespace Util {

inline Util::AssetStore<std::shared_ptr<Util::Image>> ImageCache{
    [](const std::string& filepath) {
        return std::make_shared<Util::Image>(filepath);
    }};

}  // namespace Util
