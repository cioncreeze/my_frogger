
class AlligatorBehaviourComponent : public Component
{
	// TODO: member variables?

public:
	virtual ~AlligatorBehaviourComponent() {}

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


class Alligator : public GameObject
{
public:



	virtual ~Alligator() { SDL_Log("Alligator::~Alligator"); }

	virtual void Init()
	{
		SDL_Log("Alligator::Init");
		GameObject::Init();
	}

	virtual void Receive(Message m)
	{
		//stub
	}

};