#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "avancezlib.h"
#include "object_pool.h"

#include <set>



const unsigned int	MAX_NUM_GAME_OBJECTS = 10000;
const unsigned int	NUM_LIVES = 2;

// TODO: deprecated variables?
//const unsigned int	MAX_NUM_ROCKETS = 32;
//const unsigned int	MAX_NUM_BOMBS = 32;
//const unsigned int	POINTS_PER_ALIEN = 100;
const float			CAR_TIME_INTERVAL = 1.25f;
const float			TRUNK_TIME_INTERVAL = 1.f;
const float			FROGGER_SPEED = 120.0f;
const float			CAR_SPEED = 80.0f;
const float			TRUNK_SPEED = 40.0f;
//const float			BOMB_SPEED = 120.0f;


float game_speed = 1.f;		// speed of the game; it is increased each time all the aliens are hit
							// it is also the score multiplier




#include "component.h"
#include "game_object.h"
#include "object_pool.h"

#include "background.h"
//#include "river.h" // TODO: deprecated?
#include "car.h"
//#include "turtle.h"
//#include "aligator.h"


#include "trunk.h"
#include "player.h"
#include "game.h"



int main(int argc, char** argv)
{

	AvancezLib system;

	system.init(640, 640);

	Game game;
	game.Create(&system);
	game.Init();

	float lastTime = system.getElapsedTime();
	while (system.update())
	{
		float newTime = system.getElapsedTime();
		float dt = newTime - lastTime;
		dt = dt * game_speed;
		lastTime = newTime;

		game.Update(dt);

		game.Draw();
	}

	// clean up
	game.Destroy();
	system.destroy();

	return 0;
}



