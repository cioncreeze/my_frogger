

class TurtleBehaviourComponent : public Component
{
	// TODO: member variables?

public:
	virtual ~TurtleBehaviourComponent() {}

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


class Turtle : public GameObject
{
public:



	virtual ~Turtle() { SDL_Log("Turtle::~Turtle"); }

	virtual void Init()
	{
		SDL_Log("Turtle::Init");
		GameObject::Init();
	}

	virtual void Receive(Message m)
	{
		//stub
	}

};