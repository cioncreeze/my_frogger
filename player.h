

class PlayerBehaviourComponent : public Component
{
	bool is_jumping;
	bool * on_river;
	bool * on_trunk;
	bool * on_turtle;
	bool * float_move_left;
	Dir dir;
	unsigned int start_position;

public:
	virtual ~PlayerBehaviourComponent() {}

	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects, bool * onrvr, bool * ontrnk, bool * fltmvlft, bool * ontrtl)
	{
		Component::Create(system, go, game_objects);
		// pointer to shared variables with player object
		on_river = onrvr;
		on_trunk = ontrnk;
		on_turtle = ontrtl;
		float_move_left = fltmvlft;
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

		// if frogger is on the river but not on a trunk or mid jump, he dies
		if (*on_river && !(*on_trunk) && !is_jumping && !(*on_turtle))
		{
			go->Init();
			go->Receive(REMOVE_LIFE);
		}		

		// if frogger jumps outside of the game area or sits on a trunk for too long and thus leaves the game area he dies.
		if (go->horizontalPosition < -25 || go->horizontalPosition > 665 || go->verticalPosition > 640)
		{
			go->Init();
			go->Receive(REMOVE_LIFE);
		}
		// if frogger jumps onto the other side of the river but not into a home
		if (go->verticalPosition < 125)
		{
			go->Init();
			go->Receive(REMOVE_LIFE);
		}
		/* 
			handle jumping inputs if frogger is not jumping and player issues jumping command in any direction
			this implementation leads to the up button being preferred over the left over the down over the right
			button if keys are pressed simultaniously
		*/
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
			if (*on_trunk || *on_turtle)
				Move(dt * TRUNK_SPEED, (*float_move_left ? LEFT : RIGHT));
			Move(dt * FROGGER_SPEED, dir);
			if (dir == RIGHT && (go->horizontalPosition - start_position) > 40)
			{
					go->horizontalPosition = start_position + 40;
					is_jumping = false;
			}
			else if (dir == LEFT && (start_position - go->horizontalPosition) > 40)
			{
					go->horizontalPosition = start_position - 40;
					is_jumping = false;
			}
			else if (dir == UP && (start_position - go->verticalPosition) > 40)
			{
					go->verticalPosition = start_position - 40;
					is_jumping = false;
			}
			else if (dir == DOWN && (go->verticalPosition - start_position) > 40)
			{
				go->verticalPosition = start_position + 40;
				is_jumping = false;
			}
		}
		else if (*on_trunk || *on_turtle) //frog is just sitting on trunk or turtle and is thus constantly moving along it
		{
			if (*float_move_left)
				Move(dt*TRUNK_SPEED, LEFT);
			else
				Move(dt*TRUNK_SPEED, RIGHT);
		}

		*on_trunk = false;
		*on_river = false;
		*on_turtle = false; 
		*float_move_left = false;
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

	int lives;	// player starts with three lives
	bool * on_river;
	bool * on_trunk;
	bool * on_turtle;
	bool * float_move_left;


	virtual ~Player() { SDL_Log("Player::~Player"); }

	virtual void Create(float hSize, float vSize)
	{
		lives = NUM_LIVES;
		// allocate two new booleans then initialse them to false 
		this->horizontalSize = hSize;
		this->verticalSize = vSize;
		on_river = new bool;
		on_trunk = new bool;
		on_turtle = new bool;
		float_move_left = new bool;
		*on_river = false;
		*on_trunk = false;
		*on_turtle = false;
		*float_move_left = false;
	}
	virtual void Init()
	{
		SDL_Log("Player::Init");
		GameObject::Init();
		// player always starts at that position
		this->horizontalPosition = 320 - 18;
		this->verticalPosition = 640 - 36;

	}

	// the player object has to handle a large amount of messages 
	virtual void Receive(Message m) 
	{
		// set the onriver property to true if a collision between river and player has been detected 
		if (m == ONRIVER)
			*on_river = true;
		else if (m == ONTRUNK) // set the ontrunk property to true if a collision between a trunk and player has been detected
			*on_trunk = true;
		else if (m == ONTURTLE)
			*on_turtle = true;
		else if (m == MOVE_LEFT) // set the trunk_move_left property to indicate that the trunk on which the frog is riding is moving to the left
			*float_move_left = true;
		else if (m == HIT) // if the player is hit lose a life and reinitialise the player
		{
			SDL_Log("Player::Hit!");
			RemoveLife();
			this->Init();
			std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_AFTER_DEATH));
		}
		else if (m == REMOVE_LIFE) // the player behaviour component might have to trigger reinitialisation of the player object, and sometimes also make it lose a life
			RemoveLife();
		else if (m == AT_HOME) // if frogger reached one of the homes (that has not yet been occupied) reinitialise the game
		{
			this->Init();
			// make the game sleep for a short while after the frog died to keep the player from accidentally jumping right after the game has been newly initalised
			std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_AFTER_DEATH));
		}
			
	}

	void RemoveLife()
	{
		lives--;
		if (lives < 0)
			this->Send(GAME_OVER);
		SDL_Log("remaining lives %d", lives);
	}

};
