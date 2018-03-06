

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
		go->verticalPosition = 0;
		go->horizontalPosition = 0;
	}

	virtual void Update(float dt)
	{
	}
};


class River : public GameObject
{
public:



	virtual ~River() { SDL_Log("River::~River"); }

	virtual void Init()
	{
		SDL_Log("River::Init");
		GameObject::Init();
	}

	virtual void Receive(Message m)
	{
		//stub
	}

};

