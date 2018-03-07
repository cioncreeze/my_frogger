#include "component.h"
#include "game_object.h"
#include "avancezlib.h"

void Component::Create(AvancezLib * system, GameObject * go, std::set<GameObject*>* game_objects)
{
	this->go = go;
	this->system = system;
	this->game_objects = game_objects;
}

void RenderComponent::Create(AvancezLib * system, GameObject * go, std::set<GameObject*>* game_objects, const char * sprite_name)
{
	Component::Create(system, go, game_objects);

	sprite = system->createSprite(sprite_name);
}

void RenderComponent::Update(float dt)
{
	if (!go->enabled)
		return;

	if (sprite)
		sprite->draw(int(go->horizontalPosition), int(go->verticalPosition));
}

void RenderComponent::Destroy()
{
	if (sprite != NULL)
		sprite->destroy();
	sprite = NULL;
}


void CollideComponent::Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects)
{
	Component::Create(system, go, game_objects);
	this->coll_objects = coll_objects;
}




void CollideComponent::Update(float dt)
{
	for (auto i = 0; i < coll_objects->pool.size(); i++)
	{
		GameObject * go0 = coll_objects->pool[i];
		if (go0->enabled)
		{
			/*if ((go0->horizontalPosition > go->horizontalPosition - 10) &&
				(go0->horizontalPosition < go->horizontalPosition + 10) &&
				(go0->verticalPosition   > go->verticalPosition - 10) &&
				(go0->verticalPosition   < go->verticalPosition + 10))*/
			if ((go->horizontalPosition + go->horizontalSize - 5 >= go0->horizontalPosition) && 
				(go->verticalPosition + go->verticalSize - 5 >= go0->verticalPosition) ||
				(go0->horizontalPosition + go0->horizontalSize - 5 >= go->horizontalPosition) &&
				(go0->verticalPosition + go0->verticalSize - 5 >= go->verticalPosition) ||
				(go->horizontalPosition + go->horizontalSize - 5 >= go0->horizontalPosition) &&
				(go->verticalPosition - 5 >= go0->verticalPosition + go0->verticalSize ) ||
				(go->verticalPosition + go->verticalSize - 5 >= go0->verticalPosition) &&
				(go0->horizontalPosition + go0->horizontalSize - 5 >= go->horizontalPosition))
			{
				SDL_Log("Hit detected");
				go->Receive(COLLISION);
				go0->Receive(COLLISION);
				break;
			}
		}
	}
}
