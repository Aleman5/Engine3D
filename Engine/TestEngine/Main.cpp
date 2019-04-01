/*
	Abecasis Alejandro 

	Engine
*/

#include <iostream>
#include "Game.h"

using namespace std;

int main()
{
	Game* game = new Game();

	if (game->Start(1024, 768, "Abecasis Engine"))
	{
		game->Loop();
	}

	game->Stop();

	delete game;

	return 0;
}