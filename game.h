
class Game : public GameObject
{
	std::set<GameObject*> game_objects;	// http://www.cplusplus.com/reference/set/set/
	
	AvancezLib* system;
	
	float time_trunk_launched;
	float time_car_launched;

	ObjectPool<Car> car_pool;
	ObjectPool<Trunk> trunk_pool;
	ObjectPool<River> river_pool; // I need this because then I can use the collide component without changing to much shit 

	Player * player;

	River * river; 
	Background * bg;

	Sprite * life_sprite;

	// TODO: Deprecated?
	/* keep track of froggers position, is he on the river & or a trunk
	bool on_river;  
	bool on_trunk; */

	bool game_over; // TODO: implement proper game over calls
	bool game_won;

	unsigned int score = 0;

public:

	virtual void Create(AvancezLib* system)
	{
		SDL_Log("Game::Create");

		this->system = system;

		// create background
		bg = new Background();
		// TODO: background behaviour component necessary?
		BackGroundBehaviourComponent * bg_behaviour = new BackGroundBehaviourComponent();
		bg_behaviour->Create(system, bg, &game_objects);
		RenderComponent * bg_render = new RenderComponent();
		bg_render->Create(system, bg, &game_objects, "data/background.bmp"); // TODO: find gud background
		bg->Create();
		bg->AddComponent(bg_behaviour);
		bg->AddComponent(bg_render);
		bg->AddReceiver(this);

		// player creation, component creation
		player = new Player();
		player->Create(FROGGER_H_SIZE, FROGGER_V_SIZE);
		PlayerBehaviourComponent * player_behaviour = new PlayerBehaviourComponent();
		player_behaviour->Create(system, player, &game_objects, player->on_river, player->on_trunk);
		RenderComponent * player_render = new RenderComponent();
		player_render->Create(system, player, &game_objects, "data/frogger.bmp");
		// player setup, components added
		//player->Create(FROGGER_H_SIZE, FROGGER_V_SIZE);
		player->AddComponent(player_behaviour);
		player->AddComponent(player_render);
		player->AddReceiver(this);
		game_objects.insert(player);

		// TODO: create this river component for convenience

		this->river_pool.Create(1);
		auto river = river_pool.pool.begin();
		(*river)->Create(RIVER_H_SIZE, RIVER_V_SIZE);

		CollideComponent * player_river_collider = new CollideComponent();
		player_river_collider->Create(system, player, &game_objects, (ObjectPool<GameObject>*)&river_pool);
		player->AddComponent(player_river_collider);

		CollideComponent * player_car_collider = new CollideComponent();
		player_car_collider->Create(system, player, &game_objects, (ObjectPool<GameObject>*)&car_pool);
		player->AddComponent(player_car_collider);

		CollideComponent * player_trunk_collider = new CollideComponent();
		player_trunk_collider->Create(system, player, &game_objects, (ObjectPool<GameObject>*)&trunk_pool);
		player->AddComponent(player_trunk_collider);
		// TODO: DO NOT FORGET TO PROPERLY FREE MEMORY
		// creates trunks in the river
		initialiseTrunks();
		// creates cars on the street
		initialiseCars();

		// TODO: create 5 lanes of  gators, turtles, snakes

		life_sprite = system->createSprite("data/frogger.bmp");
		score = 0;
	}
	
	void initialiseCars()
	{
		car_pool.Create(25);
		unsigned int counter = 0;
		for (auto car = car_pool.pool.begin(); car != car_pool.pool.end(); car++)
		{
			bool mvlft = false;
			if (counter % 5 == 1 || counter % 5 == 3)
				mvlft = true;
			CarBehaviourComponent * behaviour = new CarBehaviourComponent();
			behaviour->Create(system, *car, &game_objects, mvlft);
			RenderComponent * render = new RenderComponent();
			render->Create(system, *car, &game_objects, "data/car.bmp");
			(*car)->Create(CAR_H_SIZE, CAR_V_SIZE, counter % 5);
			(*car)->AddComponent(behaviour);
			(*car)->AddComponent(render);
			(*car)->AddReceiver(this);
			counter++;
		}
	}

	void initialiseTrunks()
	{
		trunk_pool.Create(25);
		unsigned int counter = 0;
		for (auto trunk = trunk_pool.pool.begin(); trunk != trunk_pool.pool.end(); trunk++)
		{
			bool mvlft = false;
			if (counter % 5 == 1 || counter % 5 == 3)
				mvlft = true;
			CarBehaviourComponent * behaviour = new CarBehaviourComponent();
			behaviour->Create(system, *trunk, &game_objects, mvlft);
			RenderComponent * render = new RenderComponent();
			render->Create(system, *trunk, &game_objects, "data/trunk.bmp");
			(*trunk)->Create(TRUNK_H_SIZE, TRUNK_V_SIZE, counter % 5);
			(*trunk)->AddComponent(behaviour);
			(*trunk)->AddComponent(render);
			(*trunk)->AddReceiver(this);
			counter++;
		}
	}

	void randomTrunk(unsigned int lane)
	{
		if (fRand(0, 1) < 0.1 && canTrunk())
		{
			Trunk * trunk = trunk_pool.FirstAvailable();
			if (trunk != NULL)
			{
				trunk->Init();
				game_objects.insert(trunk);
			}

		}
	}

	virtual void Init()
	{
		time_trunk_launched = -10000.f;
		time_car_launched = -10000.f;


		bg->Init();
		
		enabled = true;
		game_over = false;
		game_won = false;
		/* TODO: Deprecated
		on_river = false;
		on_trunk = false;*/

		player->Init();
	}

	virtual void Update(float dt)
	{
		if (IsGameOver() || IsGameWon())
			dt = 0.f;

		// Updating bg before all other game objects to make sure it will always be drawn in the background
		bg->Update(dt); 
		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Update(dt);

		for (unsigned int lane = 0; lane < 5; lane++)
		{
			randomTrunk(lane);
			//randomCar(lane);
			randomCar();
		}
	}

	bool canTrunk()
	{
		if ((system->getElapsedTime() - time_trunk_launched) < (TRUNK_TIME_INTERVAL / game_speed))
			return false;
		time_trunk_launched = system->getElapsedTime();
		return true;
	}

	bool canCar()
	{
		if ((system->getElapsedTime() - time_car_launched) < (CAR_TIME_INTERVAL / game_speed))
			return false;
		time_car_launched = system->getElapsedTime();
		return true;
	}

	// TODO: Car initialisation random car modularize by array

	void randomCar() 
	{
		if (fRand(0, 1) < 0.1 && canCar())
		{
			Car * car = car_pool.FirstAvailable();
			if (car != NULL)
			{
				car->Init();
				game_objects.insert(car);
			}	
		}
	}

	double fRand(double fMin, double fMax)
	{
		double f = (double)rand() / RAND_MAX;
		return fMin + f * (fMax - fMin);
	}

	virtual void Draw()
	{

		char msg[1024];
		//sprintf(msg, "%07d", Score());
		//system->drawText(300, 4, msg);
		sprintf(msg, "bonus: %.1fX", game_speed);
		system->drawText(510, 4, msg);

		for (int i = 0; i < player->lives; i++)
			life_sprite->draw(i*36+4, 4);

		if (IsGameOver())
		{
			sprintf(msg, "*** G A M E  O V E R ***");
			system->drawText(250, 8, msg);
		}
		if (IsGameWon())
		{
			sprintf(msg, "*** FROGGER SAVED ***");
			system->drawText(250, 250, msg);
		}
	}

	virtual void Receive(Message m)
	{
		if (m == GAME_OVER)
			game_over = true;

		if (m == FROGGER_SAVED)
			game_won = true;
			
	}


	bool IsGameOver()
	{
		return game_over;
	}

	bool IsGameWon()
	{
		return game_won;
	}

	unsigned int Score()
	{
		return score;
	}

	virtual void Destroy()
	{
		SDL_Log("Game::Destroy");

		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Destroy();

		life_sprite->destroy();

		trunk_pool.Destroy();
		car_pool.Destroy();

		player->Destroy();
		bg->Destroy();
		delete bg;
		delete player;
	}
};