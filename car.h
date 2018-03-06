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

	virtual ~CarBehaviourComponent() {}
};


class Car : public GameObject
{
public:
	virtual ~Car() { SDL_Log("Car::~Car"); }

	virtual void Init(double xPos, double yPos)
	{
		SDL_Log("Car::Init");
		GameObject::Init();
		horizontalPosition = xPos;
		verticalPosition = yPos;
	}

};