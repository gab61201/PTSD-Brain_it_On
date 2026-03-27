#include "Level/LevelData.hpp"
#include <stdexcept>

std::unordered_map<LevelId, LevelFunction>& GetLevelRegistry() {
    static std::unordered_map<LevelId, LevelFunction> registry;
    return registry;
}

void RegisterLevel(LevelId id, LevelFunction func) {
    GetLevelRegistry()[id] = func;
}

LevelData GetLevelData(LevelId id) {
    auto& registry = GetLevelRegistry();
    auto it = registry.find(id);

    if (it == registry.end()) {
        throw std::runtime_error("Level not found");
    }

    return it->second();
}
