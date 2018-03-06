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
	//bool move_left;
	virtual ~Trunk() { SDL_Log("Trunk::~Trunk"); }
	
	virtual void Create(unsigned int lane)
	{
		this->lane = lane;
	}
	virtual void Init(/*double xPos, double yPos*/)
	{
		SDL_Log("Trunk::Init");
		GameObject::Init();
		/*horizontalPosition = xPos;
		verticalPosition = yPos;*/
		switch (lane)
		{
		case 0:
			horizontalPosition = 0;
			verticalPosition = 325;
			break;
		case 1:
			horizontalPosition = 640;
			verticalPosition = 285;
			break;
		case 2:
			horizontalPosition = 0;
			verticalPosition = 245;
			break;
		case 3:
			horizontalPosition = 640;
			verticalPosition = 205;
			break;
		case 4:
			horizontalPosition = 0;
			verticalPosition = 165;
			break;
		default:
			return;
		}
	}

};