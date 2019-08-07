#include "RandomHeightGenerator.h"
#include "GlobalDefs.h"

int RandomHeightGenerator::seed = 0;
int RandomHeightGenerator::rowRandomMultiplier = 0;
int RandomHeightGenerator::columnRandomMultiplier = 0;

int RandomHeightGenerator::GenerateNoise(int row, int col)
{
	int noise;

	int noiseSeed = row * rowRandomMultiplier + col * columnRandomMultiplier + seed;
	srand(noiseSeed);
	noise = rand() % (int)MAX_BYTE_VALUE;
	srand(seed);

	return noise;
}

int RandomHeightGenerator::GenerateSmoothNoise(int row, int col)
{
	int smoothNoise;

	int corners = (GenerateNoise(row - 1, col - 1) + GenerateNoise(row - 1, col + 1) +
		GenerateNoise(row + 1, col - 1) + GenerateNoise(row + 1, col + 1)) / 16;
	int sides = (GenerateNoise(row, col - 1) + GenerateNoise(row, col + 1) +
		GenerateNoise(row - 1, col) + GenerateNoise(row + 1, col)) / 8;
	int center = GenerateNoise(row, col) / 4;

	smoothNoise = corners + sides + center;

	return smoothNoise;
}

void RandomHeightGenerator::GenerateSeed()
{
	seed = (int)time(0);
	srand(seed);

	rowRandomMultiplier = rand();
	columnRandomMultiplier = rand();
}

int RandomHeightGenerator::GenerateHeight(int row, int col)
{
	return GenerateSmoothNoise(row, col);
}