

class PlayerBehaviourComponent : public Component
{
	bool is_jumping;
	bool * on_river;
	bool * on_trunk;
	bool * trunk_move_left;
	// unsigned int current_lane; // TODO: Deprecated? 
	Dir dir;
	unsigned int start_position;

public:
	virtual ~PlayerBehaviourComponent() {}

	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects, bool * onrvr, bool * ontrnk, bool * trnkmvlft)
	{
		Component::Create(system, go, game_objects);
		// ohmygawd this is so ugly
		on_river = onrvr;
		on_trunk = ontrnk;
		trunk_move_left = trnkmvlft;
	}

	virtual void Init()
	{
		is_jumping = false;
		dir = UP;
		start_position = 0;
	}

	virtual void Update(float dt)
	{
		AvancezLib::KeyStatus keys;
		system->getKeyStatus(keys);
		

		if (*on_river)
			SDL_Log("frogger is on the river");
		if (*on_trunk)
			SDL_Log("frogger is sitting on a trunk");
		// TODO: Check if frogger is on a log or in the river
		if (*on_river && !(*on_trunk) && !is_jumping)
		{
			go->Init();
			go->Receive(REMOVE_LIFE);
		}		

		if (keys.up && !is_jumping)
		{
				is_jumping = true;
				dir = UP;
				start_position = go->verticalPosition;
				*on_trunk = false;
		}
		else if (keys.left && !is_jumping)
		{
				is_jumping = true;
				dir = LEFT;
				start_position = go->horizontalPosition;
				*on_trunk = false;
		}
		else if (keys.down && !is_jumping)
		{
				is_jumping = true;
				dir = DOWN;
				start_position = go->verticalPosition;
				*on_trunk = false;
		}
		else if (keys.right && !is_jumping)
		{
				is_jumping = true;
				dir = RIGHT;
				start_position = go->horizontalPosition;
				*on_trunk = false;
		}
		if (is_jumping)
		{
			if (*on_trunk)
				Move(dt * TRUNK_SPEED, (*trunk_move_left ? LEFT : RIGHT));
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
		else if (*on_trunk)
		{
			if (*trunk_move_left)
				Move(dt*TRUNK_SPEED*2, LEFT); // TODO: make frogger move in the right direction
			else
				Move(dt*TRUNK_SPEED*2, RIGHT);
		}

		*on_trunk = false;
		*on_river = false;
		*trunk_move_left = false;
	}


	// move the player left or right
	// param move depends on the time, so the player moves always at the same speed on any computer
	void Move(float move, int dir)
	{
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
	bool * trunk_move_left;


	virtual ~Player() { SDL_Log("Player::~Player"); }

	virtual void Create(float hSize, float vSize)
	{
		lives = NUM_LIVES;
		// allocate two new booleans then set them to false, only god knows why
		this->horizontalSize = hSize;
		this->verticalSize = vSize;
		on_river = new bool;
		on_trunk = new bool;
		trunk_move_left = new bool;
		*on_river = false;
		*on_trunk = false;
		*trunk_move_left = false;
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
		if (m == ONRIVER)
			*on_river = true;
		else if (m == ONTRUNK)
			*on_trunk = true;
		else if (m == MOVE_LEFT)
			*trunk_move_left = true;
		else if (m == HIT)
		{
			SDL_Log("Player::Hit!");
			RemoveLife();
			if (lives < 0)
				Send(GAME_OVER);
			this->Init();
			std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_AFTER_DEATH));
		}
		else if (m == REMOVE_LIFE)
			RemoveLife();
		else if (m == AT_HOME)
		{
			this->Init();
			std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_AFTER_DEATH));
		}
			
	}

	void RemoveLife()
	{
		lives--;
		SDL_Log("remaining lives %d", lives);
	}

};
