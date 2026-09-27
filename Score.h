#ifndef SHADOW_SPRINT_SCORE_H
#define SHADOW_SPRINT_SCORE_H

#include <vector>
using std::vector;

void saveScoreToFile();
vector<int> loadScores();
int getHighestScore();

#endif
