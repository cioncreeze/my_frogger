

class BackGroundBehaviourComponent : public Component
{
public:
	virtual ~BackGroundBehaviourComponent() {}

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


class Background : public GameObject
{
public:
	virtual ~Background() { SDL_Log("Background::~Background"); }

	virtual void Init()
	{
		SDL_Log("Background::Init");
		GameObject::Init();
	}
};

