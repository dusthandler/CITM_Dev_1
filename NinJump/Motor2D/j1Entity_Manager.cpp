#include "j1Entity_Manager.h"
#include "j1Player.h"
#include "j1Enemy_Flying.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Render.h"

j1Entity_Manager::j1Entity_Manager() : j1Module()
{

}

// Destructor
j1Entity_Manager::~j1Entity_Manager()
{

}

bool j1Entity_Manager::Start(){
	bool ret = true;
	//New: We will create the entyties here, that way is more easy to do the respawn.
	j1Enemy_Flying* fly = (j1Enemy_Flying*)App->entity_manager->CreateEntity(Type::ENEMY_FLYING, iPoint(250, 50));  //New: You can create a entity both ways.
	App->entity_manager->CreateEntity(Type::ENEMY_FLYING, iPoint(350, 50));


	return ret;
}
j1Entity* j1Entity_Manager::CreateEntity(Type type, iPoint pos) 
{
// 	static_assert(Type::UNKNOWN == (Type)3, "code needs update");
	j1Entity* ret = nullptr;
	switch (type) {
	case Type::ENEMY_FLYING: ret = new j1Enemy_Flying(pos,type); break; //New: Now we pass to paremeters to constructor
	// case Type::PLAYER: ret = new j1Player(); break;
	}
	
	if (ret != nullptr) {
		entities.add(ret); // entities.push_back(ret);
	}

	return ret;
}

void j1Entity_Manager::DestroyEntity(j1Entity* entity) {

	p2List_item<j1Entity*>* item;
	item = entities.start;
	j1Entity* pEntity = NULL;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity && entity != nullptr && entity->to_delete) {

			if (entity->tex != nullptr) {
				App->tex->UnLoad(item->data->tex);                      
			}
			if (entity->collider != nullptr) {
				entity->collider->to_delete = true;
			}
		
			delete entity;                                                 
			entity = nullptr;                          
		}
		
	}

}

void j1Entity_Manager::Draw() {

	p2List_item<j1Entity*>* item;
	item = entities.start;
	SDL_Rect Rect; 
	
	for (item = entities.start; item != NULL; item = item->next)
	{

		Rect = item->data->animation->GetCurrentFrame();
		App->render->Blit(item->data->tex, item->data->position.x, item->data->position.y, &Rect, 1);
		
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
		if (item->data->tex != nullptr) {
			App->tex->UnLoad(item->data->tex);                       
		}
		if (item->data->collider != nullptr) {
			item->data->collider->to_delete = true;  // TODO: check order, collider hsould be deleted first // Yes, always first collider (checked)
		}
		delete item->data;                                                  
		item->data = nullptr;

		item = item->prev; 
		
	}

	return ret;
}