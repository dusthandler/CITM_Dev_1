#include "j1Entity_Manager.h"
#include "j1Player.h"
#include "j1Enemy_Flying.h"
#include "j1Textures.h"
#include "j1App.h"

j1Entity_Manager::j1Entity_Manager() : j1Module()
{

}

// Destructor
j1Entity_Manager::~j1Entity_Manager()
{

}

j1Entity* j1Entity_Manager::CreateEntity(Type type, iPoint pos)
{
// 	static_assert(Type::UNKNOWN == (Type)3, "code needs update");
	j1Entity* ret = nullptr;
	switch (type) {
	case Type::ENEMY_FLYING: ret = new j1Enemy_Flying(); break;
	// case Type::PLAYER: ret = new j1Player(); break;
	}
	if (ret != nullptr)
		entities.add(ret); // entities.push_back(ret);
	return ret;


	
}

void j1Entity_Manager::DestroyEntity(j1Entity* entity) {

	p2List_item<j1Entity*>* item;
	item = entities.start;
	j1Entity* pEntity = NULL;
	
	
	// clean collider and texture first


	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity && entity != nullptr && entity->to_delete) {
			App->tex->UnLoad(item->data->tex);
			delete entity;                                                  // check this out
			entity = nullptr;                          
		}
		
	}

}



bool j1Entity_Manager::Update(float dt)
{
	accumulated_time += dt;
	if (accumulated_time >= update_ms_cycle)
		do_logic = true;
	UpdateAll(dt, do_logic);
	if (do_logic == true) {
		accumulated_time = 0.0f;
		do_logic = false;
	}
	return true;
}

bool j1Entity_Manager::UpdateAll(float dt, bool do_logic) {       // this function is like DoUpdate() in App
	bool ret = true;                                              // TODO: add "do_logic" condition
	p2List_item<j1Entity*>* item;
	item = entities.start;
	j1Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		/*if (pEntity->active == false) {
			continue;
		}*/

		/* TODO 5: send dt as an argument to all updates
		 you will need to update module parent class
		 and all modules that use update */
		 ret = item->data->Update(dt);
	}

	return ret;
}


bool j1Entity_Manager::CleanUp()      // as in App
{
	bool ret = true;
	p2List_item<j1Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}