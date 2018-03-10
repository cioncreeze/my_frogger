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
		this->components. <--
	}
	
	virtual void Receive(Message m)
	{
		if (!enabled)
			return;
		if (m == COLLISION && !occupied)
		{	
			this->Send(AT_HOME);
			occupied = true;
		}
		else
			this->Send(HIT);
	}
};
