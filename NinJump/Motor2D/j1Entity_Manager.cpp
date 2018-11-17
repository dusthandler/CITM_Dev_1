#include "j1Entity_Manager.h"
#include "j1Player.h"
#include "j1Enemy_Flying.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Enemy_Walker.h"
#include "j1Map.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Scene.h"

j1Entity_Manager::j1Entity_Manager() : j1Module()
{
	 
}

// Destructor
j1Entity_Manager::~j1Entity_Manager()
{

}

bool j1Entity_Manager::Start(){
	bool ret = true;
	
	//New: We will create the entyties here, that way is more easy to do the respawn. // meeec, it creates them only one time

	// j1Player* player = (j1Player*)CreateEntity(Type::PLAYER, Initialize_Player_Pos());
	return ret;
}
j1Entity* j1Entity_Manager::CreateEntity(Type type, iPoint pos) 
{
// 	static_assert(Type::UNKNOWN == (Type)3, "code needs update");
	j1Entity* ret = nullptr;
	switch (type) {
	case Type::ENEMY_FLYING: ret = new j1Enemy_Flying(pos, type); /*if (ret != nullptr) { flyers.add(ret); }*/ break;
	case Type::ENEMY_LAND: ret = new j1Enemy_Walker(pos, type); /*if (ret != nullptr) { walkers.add(ret); }*/ break; 
   // case Type::PLAYER: ret = new j1Player(pos, type); Player_Count++; break;

	}
	
	if (ret != nullptr) {
		entities.add(ret); // entities.push_back(ret);
	}

	LOG("There are a number of %i entities !!", entities.count()); 

	return ret;
}

iPoint j1Entity_Manager::Initialize_Player_Pos() {
	iPoint Pos;
	pugi::xml_node InitPos = App->map->map_file.child("map");

	// we need to load this from tiled 
	Pos.x = InitPos.child("tileset").child("terraintypes").child("terrain").child("properties").child("property").attribute("value").as_uint();
	Pos.y = InitPos.child("tileset").child("terraintypes").child("terrain").child("properties").child("property").next_sibling("property").attribute("value").as_uint();

	return Pos; 
}

j1Entity* j1Entity_Manager::Get_Player() {
	p2List_item<j1Entity*>* item;
	item = entities.start;
	j1Entity* ret = NULL;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data->type == Type::PLAYER){// && item->data != nullptr) {
			ret = item->data; 
		}
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

			entities.del(item);                      // first remove from list
			if (entity->tex != nullptr) {
				App->tex->UnLoad(item->data->tex);                      
			}
			if (entity->collider != nullptr) {
				entity->collider->to_delete = true;
			}
		                
			delete entity;                      // then delete                   
			entity = nullptr;                

			/*if (entity->type == Type::PLAYER) {
				Player_Count--; 
			}*/
		}
		
	}

}

void j1Entity_Manager::Draw() {

	p2List_item<j1Entity*>* item;
	item = entities.start;
	SDL_Rect Rect; 
	
	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data->animation != nullptr) {
			Rect = item->data->animation->GetCurrentFrame();
			App->render->Blit(item->data->tex, item->data->position.x, item->data->position.y, &Rect, 1);
		}
	}

}

bool j1Entity_Manager::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
		App->scene->MapSwap(1); 
		CleanUp();
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) { 
		App->scene->MapSwap(0); CleanUp(); 
	}


	accumulated_time += dt;
	if (accumulated_time >= update_ms_cycle)
		do_logic = true;
	UpdateAll(dt, do_logic);
	if (do_logic == true) {
		accumulated_time = 0.0f;
		do_logic = false;
	}


	if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN) {                                // testing spawn // change keys
		App->entity_manager->CreateEntity(Type::ENEMY_FLYING, iPoint(150, 200));
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
		entities.del(item);    // first remove from list

		if (item->data->tex != nullptr) {
			App->tex->UnLoad(item->data->tex);                       
		}
		if (item->data->collider != nullptr) {
			item->data->collider->to_delete = true;  
		}
		
		delete item->data;                                                  
		item->data = nullptr;

		item = item->prev; 
		
	}

	

	return ret;
}


bool j1Entity_Manager::Load(pugi::xml_node &) //New: Save and Load methods, now are not working 
{
	return false;
}

bool j1Entity_Manager::Save(pugi::xml_node &) const
{
	return false;
}


