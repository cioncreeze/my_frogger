// GameObject represents objects which moves are drawn
#include <vector>

enum Message { SUBMERGE, HIT, ALIEN_HIT, GAME_OVER, LEVEL_WIN, NO_MSG, FROGGER_SAVED, COLLISION, ONRIVER, ONTRUNK, MOVE_LEFT, REMOVE_LIFE, AT_HOME, ONTURTLE };
enum Dir { UP, LEFT, DOWN, RIGHT };

class Component;

class GameObject
{
protected:
	std::vector<GameObject*> receivers;
	std::vector<Component*> components;

public:
	double horizontalPosition;
	double verticalPosition;
	double horizontalSize;
	double verticalSize;
	bool enabled;

	virtual ~GameObject();

	virtual void Create(float horizontalSize, float verticalSize);
	virtual void Create();
	virtual void AddComponent(Component * component);

	virtual void Init();
	virtual void Update(float dt);
	virtual void Destroy();
	virtual void AddReceiver(GameObject *go);
	virtual void Receive(Message m) {}
	void Send(Message m);
};