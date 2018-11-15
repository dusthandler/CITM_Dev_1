#include "j1Entity_Manager.h"



j1Entity_Manager::j1Entity_Manager() : j1Module()
{

}

// Destructor
j1Entity_Manager::~j1Entity_Manager()
{

}

/*j1Entity* j1Entity_Manager::CreateEntity(Type type)
{
	static_assert(Type::UNKNOWN == (Type)3, "code needs update");
	j1Entity* ret = nullptr;
	switch (type) {
	// case j1Entity::Type::ENEMY: ret = new NPC(); break;
	// case j1Entity::Type::player: ret = new Player(); break;
	}
	if (ret != nullptr)
		entities.add(ret); // entities.push_back(ret);
	return ret;


	
}*/

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
	bool ret = true;
	p2List_item<j1Entity*>* item;
	item = entities.start;
	j1Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		/*if (pEntity->active == false) {
			continue;
		}*/

		// TODO 5: send dt as an argument to all updates
		// you will need to update module parent class
		// and all modules that use update
		// ret = item->data->Update(dt);
	}

	return ret;
}