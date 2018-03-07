

class PlayerBehaviourComponent : public Component
{
	bool is_jumping;
	bool * on_river;
	bool * on_trunk;
	// unsigned int current_lane; // TODO: Deprecated? 
	Dir dir;
	unsigned int start_position;

public:
	virtual ~PlayerBehaviourComponent() {}

	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects, bool * onrvr, bool * ontrnk)
	{
		Component::Create(system, go, game_objects);
		// ohmygawd this is so ugly
		on_river = onrvr;
		on_trunk = ontrnk;
	}

	virtual void Init()
	{
		//go->horizontalPosition = 320 -18;
		//go->verticalPosition = 640 - 36;
		is_jumping = false;
		dir = UP;
		start_position = 0;
	}

	virtual void Update(float dt)
	{
		//SDL_Log("player horizontal_position: %f \n", go->horizontalPosition);
		//SDL_Log("player vertical_position: %f \n", go->verticalPosition);
		AvancezLib::KeyStatus keys;
		system->getKeyStatus(keys);
		
		// TODO: Check if frogger is on a log or in the river
		//if (*on_river && !(*on_trunk))
			// frogger dies -> reset.

		if (keys.up)
		{
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

		*on_river = false;
		*on_trunk = false;
		/* TODO: Deprecated
		if (go->verticalPosition < 325 && go->verticalPosition > 125)
		{
			go->Send(GAME_OVER);
		}*/
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
};


// the main player
class Player : public GameObject
{
public:

	int lives;	// it's game over when goes below zero 
	// TODO: lives are actually frogs that have to go to the other side
	bool * on_river;
	bool * on_trunk;


	virtual ~Player() { SDL_Log("Player::~Player"); }

	virtual void Create(float hSize, float vSize)
	{
		lives = NUM_LIVES;
		// allocate two new booleans then set them to false, only god knows why
		on_river = new bool;
		on_trunk = new bool;
		*on_river = false;
		*on_trunk = false;
	}
	virtual void Init()
	{
		SDL_Log("Player::Init");
		GameObject::Init();
		this->horizontalPosition = 320 - 18;
		this->verticalPosition = 640 - 36;
	}

	virtual void Receive(Message m) 
	{
		if (!enabled)
			return;
		if (m == ONRIVER)
			*on_river = true;
		if (m == ONTRUNK)
			*on_trunk = true;

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
