

class PlayerBehaviourComponent : public Component
{
	bool is_jumping;
	// unsigned int current_lane; // TODO: Deprecated? 
	Dir dir;
	unsigned int start_position;

public:
	virtual ~PlayerBehaviourComponent() {}

	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects)
	{
		Component::Create(system, go, game_objects);
	}

	virtual void Init()
	{
		//go->horizontalPosition = 320 -18;
		//go->verticalPosition = 640 - 36;
		is_jumping = false;
		//current_lane = 1; // TODO: Deprecated?
		//time_fire_pressed = -10000.f;
		dir = UP;
		start_position = 0;
	}

	virtual void Update(float dt)
	{
		//SDL_Log("player horizontal_position: %f \n", go->horizontalPosition);
		//SDL_Log("player vertical_position: %f \n", go->verticalPosition);
		AvancezLib::KeyStatus keys;
		system->getKeyStatus(keys);
		
		/* if (current_lane == 10) // TODO: Deprecated
		{
			go->Send(FROGGER_SAVED);
		} */

		if (keys.up)
		{
			//SDL_Log("up key is pressed");
			if (!is_jumping)
			{
				is_jumping = true;
				dir = UP;
				start_position = go->verticalPosition;
			}
		}
		if (keys.left)
		{
			if (!is_jumping)
			{
				is_jumping = true;
				dir = LEFT;
				start_position = go->horizontalPosition;
			}
		}
		if (keys.down)
		{
			if (!is_jumping)
			{
				is_jumping = true;
				dir = DOWN;
				start_position = go->verticalPosition;
			}
		}
		if (keys.right)
		{
			if (!is_jumping)
			{
				is_jumping = true;
				dir = RIGHT;
				start_position = go->horizontalPosition;
			}
		}
		if (is_jumping)
		{
			// something something Lane
			Move(dt * FROGGER_SPEED, dir);
			if (dir == RIGHT && (go->horizontalPosition - start_position) > 40)
			{
					go->horizontalPosition = start_position + 40;
					is_jumping = false;
			}
			if (dir == LEFT && (start_position - go->horizontalPosition) > 40)
			{
					go->horizontalPosition = start_position - 40;
					is_jumping = false;
			}
			if (dir == UP && (start_position - go->verticalPosition) > 40)
			{
					go->verticalPosition = start_position - 40;
					is_jumping = false;
			}
			if (dir == DOWN && (go->verticalPosition - start_position) > 40)
			{
				go->verticalPosition = start_position + 40;
				is_jumping = false;
			}
		}
		// TODO: Check if frogger is on a log or in the river

		if (go->verticalPosition < 325 && go->verticalPosition > 125)
		{
			go->Send(GAME_OVER);
		}
		if (go->verticalPosition < 125)
			go->Send(FROGGER_SAVED);
	}


	// move the player left or right
	// param move depends on the time, so the player moves always at the same speed on any computer
	void Move(float move, int dir)
	{
		// dir 1 = up; 
		switch (dir)
		{
		case UP:
			go->verticalPosition -= move;
			break;
		case DOWN:
			go->verticalPosition += move;
			break;
		case LEFT:
			go->horizontalPosition -= move;
			break;
		case RIGHT:
			go->horizontalPosition += move;
			break;
		}
	}
	/*int CurrentLane() // TODO: Deprecated?
	{
		return current_lane;
	}*/
};


// the main player
class Player : public GameObject
{
public:

	int lives;	// it's game over when goes below zero 
	// TODO: lives are actually frogs that have to go to the other side


	virtual ~Player() { SDL_Log("Player::~Player"); }

	virtual void Create()
	{
		lives = NUM_LIVES;
	}
	virtual void Init()
	{
		SDL_Log("Player::Init");
		GameObject::Init();
		horizontalPosition = 320 - 18;
		verticalPosition = 640 - 36;
	}

	virtual void Receive(Message m) 
	{
		if (m == HIT)
		{ 
			SDL_Log("Player::Hit!");
			RemoveLife();

			if (lives < 0)
				Send(GAME_OVER);
			this->Init();
		}
	}

	void RemoveLife()
	{
		lives--;
		SDL_Log("remaining lives %d", lives);
	}

};
