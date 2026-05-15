#ifndef PROGRESS_STORE_HPP
#define PROGRESS_STORE_HPP

#include <array>

#include "Level/Level.hpp"

using StarConditions = std::array<bool, 3>;

void LoadOrCreateDefault();
bool Save();

StarConditions GetConditions(LevelId levelId);
int GetTotalStars();
bool ApplyResultAndSave(const LevelResultData& resultData);

#endif
