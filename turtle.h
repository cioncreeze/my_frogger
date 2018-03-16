class TurtleBehaviourComponent : public Component
{
public:
	bool move_left;
	float submerge_start;
	bool submerging;

	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects, bool mvlft)
	{
		Component::Create(system, go, game_objects);
		this->move_left = mvlft;
		this->submerge_start = -1000.f;
		this->submerging = false;
	}

	virtual void Update(float dt)
	{
		Move(dt*TRUNK_SPEED);

		if (fRand(0, 1) < 0.001 && !submerging)
		{
			go->Receive(SUBMERGE);
			submerge_start = system->getElapsedTime();
			submerging = true;
		}
		// after a while the turtles submerge completely, thus disappearing
		if (submerging && ((system->getElapsedTime() - submerge_start) > 2.85))
		{	
			go->enabled = false;
			this->submerging = false;
			this->submerge_start = -1000.f;
		}

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
	double fRand(double fMin, double fMax)
	{
		double f = (double)rand() / RAND_MAX;
		return fMin + f * (fMax - fMin);
	}

	virtual ~TurtleBehaviourComponent() {}
};


class Turtle : public GameObject
{
	unsigned int lane;
	RenderComponent * submergeRender;
	RenderComponent * turtleRender;
public:
	bool move_left;
	virtual ~Turtle() { SDL_Log("Turtle::~Turtle"); }

	virtual void Create(float hSize, float vSize, unsigned int lane)
	{
		this->horizontalSize = hSize;
		this->verticalSize = vSize;
		this->lane = lane;
	}

	virtual void Init()
	{
		SDL_Log("Turtle::Init");
		GameObject::Init();
		switch (lane)
		{
		case 0:
			horizontalPosition = -100;
			verticalPosition = 325;
			move_left = false;
			break;
		case 1:
			horizontalPosition = 640;
			verticalPosition = 285;
			move_left = true;
			break;
		case 2:
			horizontalPosition = -100;
			verticalPosition = 245;
			move_left = false;
			break;
		case 3:
			horizontalPosition = 640;
			verticalPosition = 205;
			move_left = true;
			break;
		case 4:
			horizontalPosition = -100;
			verticalPosition = 165;
			move_left = false;
			break;
		default:
			return;
		}
		if (this->components.size() == 2)
			this->components.pop_back();
		this->components.push_back(turtleRender);
		this->enabled = true;
	}
	void addSubmergeRender(RenderComponent * sbmrgrndr)
	{
		submergeRender = sbmrgrndr;
	}
	void addStandardRender(RenderComponent* stndrdrndr)
	{
		turtleRender = stndrdrndr;
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;
		if (m == COLLISION) // if a collision between the player and a trunk is detected then inform the receivers thusly
		{
			this->Send(ONTURTLE);
			if (move_left)
				this->Send(MOVE_LEFT);
		}
		if (m == SUBMERGE)
		{
			// as soon as  submerging begins the new renderer has to be added.
			// and the old one has to be removed, this is done by first clearing the components
			// and then adding the relevant components again.
			this->components.pop_back();
			this->components.push_back(submergeRender);
			//this->AddComponent(submergeRender);
		}
		
	}

};