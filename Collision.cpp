#include <cstdlib>
#include <cmath>
#include "GameGlobals.h"
#include "Collision.h"

bool collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	if (x1 + w1 < x2) return false;
	if (x2 + w2 < x1) return false;
	if (y1 + h1 < y2) return false;
	if (y2 + h2 < y1) return false;

	return true;

}

bool isTooClose(int x1, int x2)
{
	int gap = abs(x1 - x2);

	if (gap < 250)
		return true;

	return false;
}
