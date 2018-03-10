#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "avancezlib.h"
#include "object_pool.h"

#include <set>
#include <chrono>
#include <thread>





const unsigned int	MAX_NUM_GAME_OBJECTS = 10000;
const unsigned int	NUM_LIVES = 2;

// TODO: deprecated variables?
const float			CAR_TIME_INTERVAL = .75f;
const float			TRUNK_TIME_INTERVAL = .75f;
const float			FROGGER_SPEED = 120.0f;
const float			CAR_SPEED = 80.0f;
const float			TRUNK_SPEED = 40.0f;

// sizes for game objects go here
const float			CAR_H_SIZE = 55.f;
const float			CAR_V_SIZE = 30.f;
const float			TRUNK_H_SIZE = 115.f;
const float			TRUNK_V_SIZE = 30.f;
const float			FROGGER_H_SIZE = 32.f;
const float			FROGGER_V_SIZE = 32.f;
const float			RIVER_H_SIZE = 640.f;
const float			RIVER_V_SIZE = 200.f;
const float			HOME_H_SIZE = 40.f;
const float			HOME_V_SIZE = 40.f;


float game_speed = 1.f;		// speed of the game; it is increased each time all the aliens are hit
							// it is also the score multiplier

unsigned int SLEEP_AFTER_DEATH = 100; // sleep timer to let game sleep for a little while after frogger got resetted


#include "component.h"
#include "game_object.h"
#include "object_pool.h"

#include "background.h"
#include "car.h"
#include "river.h"
//#include "turtle.h"
//#include "aligator.h"


#include "trunk.h"
#include "player.h"
#include "home.h"

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



