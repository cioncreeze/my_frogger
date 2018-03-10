class TrunkBehaviourComponent : public Component
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
		Move(dt*TRUNK_SPEED);

		if (go->horizontalPosition > 640)
			go->enabled = false;
		if (go->horizontalPosition < -250)
			go->enabled = false;
	}

	void Move(float move)
	{
		SDL_Log("move of trunk is: %f ", move);
		if (move_left)
			go->horizontalPosition -= move;
		else
			go->horizontalPosition += move;
	}
	virtual ~TrunkBehaviourComponent() {}
};


class Trunk : public GameObject
{
	unsigned int lane;
public:
	bool move_left;
	virtual ~Trunk() { SDL_Log("Trunk::~Trunk"); }
	
	virtual void Create(float hSize, float vSize, unsigned int lane)
	{
		this->horizontalSize = hSize;
		this->verticalSize = vSize;
		this->lane = lane;
	}
	virtual void Init()
	{
		SDL_Log("Trunk::Init");
		GameObject::Init();
		switch (lane)
		{
		case 0:
			horizontalPosition = 0;
			verticalPosition = 325;
			move_left = false;
			break;
		case 1:
			horizontalPosition = 640;
			verticalPosition = 285;
			move_left = true;
			break;
		case 2:
			horizontalPosition = 0;
			verticalPosition = 245;
			move_left = false;
			break;
		case 3:
			horizontalPosition = 640;
			verticalPosition = 205;
			move_left = true;
			break;
		case 4:
			horizontalPosition = 0;
			verticalPosition = 165;
			move_left = false;
			break;
		default:
			return;
		}
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;
		if (m == COLLISION)
		{
			this->Send(ONTRUNK);
			if (move_left)
				this->Send(MOVE_LEFT);
		}
	}
};