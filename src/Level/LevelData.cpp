#include "Level/LevelData.hpp"

#include <stdexcept>

// 使用 function-local static 是為了規避 static initialization order fiasco：
// 各關卡的 .cpp 在 translation-unit 初始化時呼叫 RegisterLevel()，這個 getter
// 確保 registry 一定在第一次 Register 之前就構造完成。
// 注意：其他 static 物件不可在自己的建構子中依賴此 registry 的內容，因為
// Register 呼叫的時機仍取決於各 TU 的載入順序。
std::map<LevelId, LevelFunction>& GetLevelRegistry() {
    static std::map<LevelId, LevelFunction> registry;
    return registry;
}

void RegisterLevel(LevelId id, LevelFunction func) {
    GetLevelRegistry()[id] = func;
}

LevelConfig GetLevelConfig(LevelId id) {
    auto& registry = GetLevelRegistry();
    auto it = registry.find(id);

    if (it == registry.end()) {
        throw std::runtime_error("Level not found");
    }

    return it->second();
}
