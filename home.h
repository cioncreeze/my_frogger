class HomeBehaciourComponent : public Component
{
public:

	virtual void Create(AvancezLib * system, GameObject * go, std::set<GameObject*> game_objects) {}

	virtual void Update(float dt) {}

	void Move(float move) {}
};

class Home : public GameObject
{
	bool occupied;
	RenderComponent * occupied_render;
public:
	
	virtual void Create(float hPos, float vPos, float hSize, float vSize)
	{
		this->horizontalPosition = hPos;
		this->verticalPosition = vPos;
		this->horizontalSize = hSize;
		this->verticalSize = vSize;
	}

	virtual void Init()
	{
		this->enabled = true;
		this->occupied = false;
		this->ClearComponents();
	}
	
	void InsertOccupiedRender(RenderComponent * ocprnd)
	{
		this->occupied_render = ocprnd;
	}

	void ClearComponents()
	{
		this->components.clear();
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;
		/*
			if there is a collision between frogger and the home pond and the home pond is not yet occupied,
			then send a message to the player object that a goal state is reached.
		*/
		if (m == COLLISION && !occupied)
		{	
			this->Send(AT_HOME);
			occupied = true; // make sure the home pond is now registered as occupied
			this->AddComponent(occupied_render);
		}
		else
			this->Send(HIT); // if the home pond is already occupied the player object recieves a hit -> resulting in deduction of one life etc.
	}
};
