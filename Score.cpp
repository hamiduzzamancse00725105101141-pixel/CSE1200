#pragma warning(disable:4996)
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;
#include "GameGlobals.h"
#include "Score.h"


static const char* SCORE_FILE = "scores.txt";

void saveScoreToFile()
{
	if (scoreSaved) return;
	ofstream file(SCORE_FILE, ios::out | ios::app);
	if (file.is_open())
	{
		file << score << "\n";
		file.close();
	}
	scoreSaved = true;
}

vector<int> loadScores()
{
	vector<int> scores;
	ifstream file(SCORE_FILE);
	int value;
	while (file >> value) scores.push_back(value);
	file.close();
	return scores;
}

int getHighestScore()
{
	vector<int> scores = loadScores();
	if (scores.empty()) return 0;
	return *max_element(scores.begin(), scores.end());
}
