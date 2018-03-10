

class RiverBehaviourComponent : public Component
{
	// TODO: member variables?

public:
	virtual ~RiverBehaviourComponent() {}

	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects)
	{
		Component::Create(system, go, game_objects);
	}

	virtual void Init()
	{

	}

	virtual void Update(float dt)
	{
	}
};


class River : public GameObject
{
public:



	virtual ~River() { SDL_Log("River::~River"); }

	virtual void Create(float hSize, float vSize)
	{
		this->horizontalPosition = 0;
		this->verticalPosition = 160;
		this->horizontalSize = hSize;
		this->verticalSize = vSize;
	}

	virtual void Init()
	{
		this->enabled = true;
		SDL_Log("River::Init");
		GameObject::Init();
	}

	virtual void Receive(Message m)
	{
		SDL_Log("River recieving a message");
		if (m == COLLISION)
			this->Send(ONRIVER);
	}

};

