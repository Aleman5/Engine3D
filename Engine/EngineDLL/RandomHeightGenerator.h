#pragma once

#include <time.h>
#include <stdlib.h>
#include "Exports.h"

class ENGINEDLL_API RandomHeightGenerator
{
	static int seed;
	static int rowRandomMultiplier;
	static int columnRandomMultiplier;

	static int GenerateNoise(int row, int col);
	static int GenerateSmoothNoise(int row, int col);

public:
	static void GenerateSeed();
	static int GenerateHeight(int row, int col);
};

