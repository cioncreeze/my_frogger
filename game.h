
class Game : public GameObject
{
	std::set<GameObject*> game_objects;	// http://www.cplusplus.com/reference/set/set/
	
	AvancezLib* system;
	
	float time_trunk_launched;
	float time_car_launched;
	float time_turtle_launched;

	ObjectPool<Car> car_pool;
	ObjectPool<Trunk> trunk_pool;
	ObjectPool<River> river_pool; // Needed for the computation of collision between frog and river
	ObjectPool<Home> home_pool;
	ObjectPool<Turtle> turtle_pool;

	Player * player;

	River * river; 
	Background * bg;

	Sprite * life_sprite;

	bool game_over;
	bool game_won;

	unsigned int score;
	unsigned int homes_reached;

public:

	virtual void Create(AvancezLib* system)
	{
		SDL_Log("Game::Create");

		this->system = system;

		// create background
		bg = new Background();
		RenderComponent * bg_render = new RenderComponent();
		bg_render->Create(system, bg, &game_objects, "data/background.bmp"); // background.bmp is a low-fi bitmap image
		bg->Create();
		bg->AddComponent(bg_render);

		// player creation, component creation
		player = new Player();
		player->Create(FROGGER_H_SIZE, FROGGER_V_SIZE); // make sure the player object has the proper size
		PlayerBehaviourComponent * player_behaviour = new PlayerBehaviourComponent();
		player_behaviour->Create(system, player, &game_objects, player->on_river, player->on_trunk, player->float_move_left, player->on_turtle); // make sure the behavious component has access to the shared booleans
		RenderComponent * player_render = new RenderComponent();
		player_render->Create(system, player, &game_objects, "data/frogger.bmp");
		player->AddComponent(player_behaviour);
		player->AddComponent(player_render);
		player->AddReceiver(this);
		game_objects.insert(player);

		// creating the river, there is only one river inside the river_pool because only one is needed, however the pool exists so the collide component can be properly used
		this->river_pool.Create(1);
		auto rvr = river_pool.pool.begin();
		(*rvr)->Create(RIVER_H_SIZE, RIVER_V_SIZE);
		(*rvr)->Init();
		game_objects.insert(*rvr);
		(*rvr)->AddReceiver(player);

		// creation of the colliders with the player - collision of the player object with river, trunks, cars, TODO: turtles, has to be accounted for
		CollideComponent * player_river_collider = new CollideComponent();
		player_river_collider->Create(system, player, &game_objects, (ObjectPool<GameObject>*)&river_pool);
		player->AddComponent(player_river_collider);

		CollideComponent * player_car_collider = new CollideComponent();
		player_car_collider->Create(system, player, &game_objects, (ObjectPool<GameObject>*)&car_pool);
		player->AddComponent(player_car_collider);

		CollideComponent * player_trunk_collider = new CollideComponent();
		player_trunk_collider->Create(system, player, &game_objects, (ObjectPool<GameObject>*)&trunk_pool);
		player->AddComponent(player_trunk_collider);

		CollideComponent * player_home_collider = new CollideComponent();
		player_home_collider->Create(system, player, &game_objects, (ObjectPool<GameObject>*)&home_pool);
		player->AddComponent(player_home_collider);

		CollideComponent * player_turtle_collider = new CollideComponent();
		player_turtle_collider->Create(system, player, &game_objects, (ObjectPool<GameObject>*)&turtle_pool);
		player->AddComponent(player_turtle_collider);


		// creates trunks in the river
		initialiseTrunks();
		// creates cars on the street
		initialiseCars();
		// create the frogger homes
		initialiseHomes();
		// creates the turtles
		initialiseTurtles();

		life_sprite = system->createSprite("data/frogger.bmp");
		score = 0;
	}
	
	/*
		create the 5 homes on the top side of the river which the frog has to reach. each home is 40px wide and 80px away from the next one.
	*/
	void initialiseHomes()
	{
		home_pool.Create(5);
		float home_location_vertical = 120.f;
		float home_location_horizontal = 60.f;
		for (auto home = home_pool.pool.begin(); home != home_pool.pool.end(); home++)
		{
			(*home)->Create(home_location_horizontal, home_location_vertical, HOME_H_SIZE, HOME_V_SIZE);
			(*home)->AddReceiver(player);
			RenderComponent * home_occupied_render = new RenderComponent();
			home_occupied_render->Create(system, *home, &game_objects, "data/frogger.bmp");
			(*home)->InsertOccupiedRender(home_occupied_render);
			(*home)->AddReceiver(this);
			home_location_horizontal += 120.f;
		}
	}

	/*
		create 15 turtles (represented by 3 turtles in a row)
	*/
	void initialiseTurtles()
	{
		turtle_pool.Create(15);
		unsigned int counter = 0;
		for (auto turtle = turtle_pool.pool.begin(); turtle != turtle_pool.pool.end(); turtle++)
		{
			bool mvlft = false;
			if (counter % 5 == 1 || counter % 5 == 3) // if the turtle drives on lane 1 or lane 3 it has to move towards the left starting at the right
				mvlft = true;
			TurtleBehaviourComponent * behaviour = new TurtleBehaviourComponent();
			behaviour->Create(system, *turtle, &game_objects, mvlft);
			RenderComponent * render = new RenderComponent();
			render->Create(system, *turtle, &game_objects, "data/turtles.bmp");
			RenderComponent * submergeRender = new RenderComponent();
			submergeRender->Create(system, *turtle, &game_objects, "data/turtles_submerging.bmp");
			(*turtle)->addSubmergeRender(submergeRender);
			(*turtle)->addStandardRender(render);
			(*turtle)->Create(TURTLE_H_SIZE, TURTLE_V_SIZE, counter % 5);
			(*turtle)->AddComponent(behaviour);
			(*turtle)->AddComponent(render);
			(*turtle)->AddReceiver(this);
			(*turtle)->AddReceiver(player);
			counter++;
		}
	}

	/*
		cars are created here, in total there are 25 cars available, that means there are never more than 25 cars on the screen. 
		Each car has a lane property and a move left property.
		Assume that lanes are numbered 0 to 4, thus lanes 1 and 3 move to the left.
	*/
	void initialiseCars()
	{
		car_pool.Create(25);
		unsigned int counter = 0;
		for (auto car = car_pool.pool.begin(); car != car_pool.pool.end(); car++)
		{
			bool mvlft = false;
			if (counter % 5 == 1 || counter % 5 == 3) // if the car drives on lane 1 or lane 3 it has to move towards the left starting at the right
				mvlft = true;
			CarBehaviourComponent * behaviour = new CarBehaviourComponent();
			behaviour->Create(system, *car, &game_objects, mvlft);
			RenderComponent * render = new RenderComponent();
			render->Create(system, *car, &game_objects, "data/car.bmp");
			(*car)->Create(CAR_H_SIZE, CAR_V_SIZE, counter % 5);
			(*car)->AddComponent(behaviour);
			(*car)->AddComponent(render);
			(*car)->AddReceiver(this);
			(*car)->AddReceiver(player);
			counter++;
		}
	}

	/*
		trunks behave similar to the cars as seen above
	*/
	void initialiseTrunks()
	{
		trunk_pool.Create(25);
		unsigned int counter = 0;
		for (auto trunk = trunk_pool.pool.begin(); trunk != trunk_pool.pool.end(); trunk++)
		{
			bool mvlft = false;
			if (counter % 5 == 1 || counter % 5 == 3) // same as above there are 5 lanes 2 of which go from right to left instead of left to right
				mvlft = true;
			TrunkBehaviourComponent * behaviour = new TrunkBehaviourComponent();
			behaviour->Create(system, *trunk, &game_objects, mvlft);
			RenderComponent * render = new RenderComponent();
			render->Create(system, *trunk, &game_objects, "data/trunk.bmp");
			(*trunk)->Create(TRUNK_H_SIZE, TRUNK_V_SIZE, counter % 5);
			(*trunk)->AddComponent(behaviour);
			(*trunk)->AddComponent(render);
			(*trunk)->AddReceiver(this);
			(*trunk)->AddReceiver(player);
			counter++;
		}
	}


	void randomTurtle()
	{
		if (fRand(0, 1) < 0.025 && canTurtle() && !canTrunk())
		{
			Turtle * turtle = turtle_pool.FirstAvailable();
			if (turtle != NULL)
			{
				turtle->Init();
			}
		}
	}
	/*
		initialise a random trunk - that means, look for an available trunk and then get it underway
	*/
	void randomTrunk()
	{
		if (fRand(0, 1) < 0.1 && canTrunk())
		{
			Trunk * trunk = trunk_pool.FirstAvailable();
			if (trunk != NULL)
			{
				trunk->Init();
			}
		}
	}

	/*
		initialise a random car - that means, look for an available car and then get it underway
	*/
	void randomCar()
	{
		if (fRand(0, 1) < 0.1 && canCar())
		{
			Car * car = car_pool.FirstAvailable();
			if (car != NULL)
			{
				car->Init();
			}
		}
	}

	/*
		initialise the game object, setting important variables and initialising 
	*/
	virtual void Init()
	{
		time_trunk_launched = -10000.f;
		time_car_launched = -10000.f;
		time_turtle_launched = -1000.f;
		homes_reached = 0;

		bg->Init();
		for (auto home = home_pool.pool.begin(); home != home_pool.pool.end(); home++)
		{
			(*home)->Init();
			(*home)->ClearComponents();
		}
		enabled = true;
		game_over = false;
		game_won = false;

		player->Init();
	}

	virtual void Update(float dt)
	{
		if (IsGameOver() || IsGameWon())
			dt = 0.f;

		// Updating bg before all other game objects to make sure it will always be drawn in the background
		bg->Update(dt); 
		// Updating all the object pools 
		for (auto trunk = trunk_pool.pool.begin(); trunk != trunk_pool.pool.end(); trunk++)
			(*trunk)->Update(dt);
		for (auto car = car_pool.pool.begin(); car != car_pool.pool.end(); car++)
			(*car)->Update(dt);
		for (auto home = home_pool.pool.begin(); home != home_pool.pool.end(); home++)
			(*home)->Update(dt);
		for (auto turtle = turtle_pool.pool.begin(); turtle != turtle_pool.pool.end(); turtle++)
			(*turtle)->Update(dt);
		// updating the player and the river last 
		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Update(dt);

		for (unsigned int lane = 0; lane < 5; lane++)
		{
			randomTrunk();
			randomCar();
			randomTurtle();
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

	bool canTurtle()
	{
		if ((system->getElapsedTime() - time_turtle_launched) < (TURTLE_TIME_INTERVAL / game_speed))
			return false;
		time_turtle_launched = system->getElapsedTime();
		return true;
	}

	
	double fRand(double fMin, double fMax)
	{
		double f = (double)rand() / RAND_MAX;
		return fMin + f * (fMax - fMin);
	}

	virtual void Draw()
	{

		char msg[1024];
		sprintf(msg, "%07d", Score());
		system->drawText(300, 4, msg);
		sprintf(msg, "bonus: %.1fX", game_speed);
		system->drawText(510, 4, msg);

		for (int i = 0; i < player->lives; i++)
			life_sprite->draw(i*36+4, 4);

		if (IsGameOver())
		{
			sprintf(msg, "*** G A M E  O V E R ***");
			system->drawText(250, 250, msg);
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

		if (m == AT_HOME)
		{
			score += 200;
			homes_reached++;
			if (homes_reached > 4)
			{
				game_speed += .1f;
				this->Init();
			}	
		}

		
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
		river_pool.Destroy();
		turtle_pool.Destroy();

		player->Destroy();
		bg->Destroy();
		delete bg;
		delete player;
	}
};