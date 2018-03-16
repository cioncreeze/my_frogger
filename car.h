class CarBehaviourComponent : public Component
{
public:
	bool move_left;

	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects, bool mvlft)
	{
		Component::Create(system, go, game_objects);
		this->move_left = mvlft;
	}

	virtual void Update(float dt)
	{
		Move(dt*CAR_SPEED);

		if (go->horizontalPosition > 640)
			go->enabled = false;
		if (go->horizontalPosition < -100)
			go->enabled = false;
	}

	void Move(float move)
	{
		if (move_left)
			go->horizontalPosition -= move;
		else
			go->horizontalPosition += move;
	}

	virtual ~CarBehaviourComponent() {}
};


class Car : public GameObject
{
	unsigned int lane;
public:

	virtual ~Car() { SDL_Log("Car::~Car"); }

	virtual void Create(float hSize, float vSize, int lane)
	{
		this->lane = lane;
		this->horizontalSize = hSize;
		this->verticalSize = vSize;
	}
	virtual void Init() // lanes are hardcoded by initial position
	{
		SDL_Log("Car::Init");
		GameObject::Init();
		switch (lane)
		{
		case 0:
			horizontalPosition = -40;
			verticalPosition = 565;
			break;
		case 1:
			horizontalPosition = 640;
			verticalPosition = 525;
			break;
		case 2:
			horizontalPosition = -40;
			verticalPosition = 485;
			break;
		case 3:
			horizontalPosition = 640;
			verticalPosition = 445;
			break;
		case 4:
			horizontalPosition = 40;
			verticalPosition = 405;
			break;
		default:
			return;
		}
	}

	virtual void Receive(Message m) 
	{
		SDL_Log("car recieving message");
		if (!enabled)
			return;
		if (m == COLLISION)
			this->Send(HIT);
	}
};