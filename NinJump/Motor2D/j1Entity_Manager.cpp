#include "j1Entity_Manager.h"

#include "j1Enemy_Flying.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Enemy_Walker.h"
#include "j1Player_Entity.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Entity_Coin.h"
#include "Brofiler/Brofiler.h"
#include "j1Audio.h"
#include "j1Shuriken.h"

j1Entity_Manager::j1Entity_Manager() : j1Module()
{
	name.create("Entity_Manager");
}

// Destructor
j1Entity_Manager::~j1Entity_Manager()
{

}

bool j1Entity_Manager::Start(){
	BROFILER_CATEGORY("Entity Manager Start", Profiler::Color::Brown);

	bool ret = true;
	// path texture
	path_tex = App->tex->Load("Maps/Textures/path_tex.png"); 
	
	// CreateEntity(Type::ENEMY_FLYING, iPoint(350, 200));
	/*CreateEntity(Type::ENEMY_FLYING, iPoint(1300, 400));
	CreateEntity(Type::ENEMY_FLYING, iPoint(1500, 400));
	CreateEntity(Type::ENEMY_FLYING, iPoint(1700, 400));
	CreateEntity(Type::ENEMY_FLYING, iPoint(1900, 400));
	CreateEntity(Type::ENEMY_FLYING, iPoint(2100, 400));*/

	// CreateEntity(Type::ENEMY_LAND, iPoint(350, 200));
		
	
  

	iPoint Pos;
	pugi::xml_node InitPos = App->map->map_file.child("map");

	// we need to load this from tiled 
	Pos.x = InitPos.child("tileset").child("terraintypes").child("terrain").child("properties").child("property").attribute("value").as_uint();
	Pos.y = InitPos.child("tileset").child("terraintypes").child("terrain").child("properties").child("property").next_sibling("property").attribute("value").as_uint();
	CreateEntity(Type::PLAYER, Pos);

	// coins and stuff
	App->audio->LoadFx("Sound/Fx/coin.wav");  // 5
	App->audio->LoadFx("Sound/Fx/shot.wav");  // 6

	CreateEntity(Type::SHURIKEN, iPoint(350, 870));        // after player !
	CreateEntity(Type::COIN, iPoint(450, 870));
	CreateEntity(Type::COIN, iPoint(550, 870));
	CreateEntity(Type::COIN, iPoint(650, 870));

	return ret;
}
j1Entity* j1Entity_Manager::CreateEntity(Type type, iPoint pos) 
{
// 	static_assert(Type::UNKNOWN == (Type)3, "code needs update");
	j1Entity* ret = nullptr;
	switch (type) {
	case Type::ENEMY_FLYING: ret = new j1Enemy_Flying(pos,type); break; //New: Now we pass to paremeters to constructor
	case Type::ENEMY_LAND: ret = new j1Enemy_Walker(pos, type); break; //New: Land enemie :D
	case Type::PLAYER: ret = new j1Player_Entity(pos, type); break;
	case Type::COIN: ret = new j1Entity_Coin(pos, type); break;
	case Type::SHURIKEN: ret = new j1Shuriken(pos, type); break;
	}
	
	if (ret != nullptr) {
		entities.add(ret); // entities.push_back(ret);
	}

	return ret;
}


void j1Entity_Manager::Draw() {
	BROFILER_CATEGORY("Entity Manager Draw", Profiler::Color::BurlyWood);

	p2List_item<j1Entity*>* item;
	item = entities.start;
	SDL_Rect Rect; 
	
	for (item = entities.start; item != NULL; item = item->next)
	{

		Rect = item->data->animation->GetCurrentFrame();
		if (item->data->type == Type::PLAYER && Get_Gravity_Reverse()) {
			App->render->Blit(item->data->tex, item->data->position.x, item->data->position.y, &Rect, 1, "player");
		}
		else {
			App->render->Blit(item->data->tex, item->data->position.x, item->data->position.y, &Rect, 1);
		}
		
		
	}

}

bool j1Entity_Manager::Update(float dt)
{
	BROFILER_CATEGORY("Entity Manager Update", Profiler::Color::CadetBlue);
	bool ret = true;                                              // TODO: add "do_logic" condition
	p2List_item<j1Entity*>* item;
	item = entities.start;
	j1Entity* pEntity = NULL;

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		CleanUp();
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN || restart) {
		Start();
		restart = false;
	}
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

bool j1Entity_Manager::PreUpdate() {
	BROFILER_CATEGORY("Entity Manager PreUpdate", Profiler::Color::CornflowerBlue);
	bool ret = true;                                              // TODO: add "do_logic" condition
	p2List_item<j1Entity*>* item;
	item = entities.start;
	


	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
	

		/*if (pEntity->active == false) {
		continue;
		}*/

		/* TODO 5: send dt as an argument to all updates
		you will need to update module parent class
		and all modules that use update */
		ret = item->data->PreUpdate();
	}
	
	return ret;
}

bool j1Entity_Manager::PostUpdate() {
	BROFILER_CATEGORY("Entity Manager PostUpdate", Profiler::Color::Cornsilk);
	bool ret = true;                                              // TODO: add "do_logic" condition
	p2List_item<j1Entity*>* item;
	item = entities.start;
	

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		

		/*if (pEntity->active == false) {
		continue;
		}*/

		/* TODO 5: send dt as an argument to all updates
		you will need to update module parent class
		and all modules that use update */
		ret = item->data->PostUpdate();
	}
	

	return ret;
}
bool j1Entity_Manager::UpdateAll(float dt, bool do_logic) {       // this function is like DoUpdate() in App
	return true;
}

void j1Entity_Manager::DestroyEntity(j1Entity* entity) {

	bool ret = true;                                                   // REMAKE THIS
	p2List_item<j1Entity*>* item;

	item = entities.start;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		if(item->data == entity){
		ret = item->data->CleanUp();
		delete item->data;
		item->data = nullptr;
		}
	}
	entities.del(item); 

}



bool j1Entity_Manager::CleanUp()      // as in App
{
	BROFILER_CATEGORY("Entity Manager Cleanup", Profiler::Color::Crimson);
	bool ret = true;
	p2List_item<j1Entity*>* item;  

	item = entities.start;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		ret = item->data->CleanUp();
		delete item->data;
		item->data = nullptr;
	}
	entities.clear();

	// unload coin and shot fxs at the end

	App->audio->UnloadFx(5);
	App->audio->UnloadFx(6);

	// path tex
	App->tex->UnLoad(path_tex); 


	return ret;
}

iPoint j1Entity_Manager::GetPlayerPos() {
	iPoint pos;
	bool done = false;
	p2List_item<j1Entity*>* item;
	item = entities.start;

	for (; item != NULL; item = item->next) 
	{

		if (item->data->type == Type::PLAYER) {
			pos = item->data->position;
		}

	}
	return pos;
}


int j1Entity_Manager::GetPlayerDir() {
	int dir = 0; 

	p2List_item<j1Entity*>* item;
	item = entities.start;

	for (; item != NULL; item = item->next)
	{

		if (item->data->type == Type::PLAYER) {
			
			if (item->data->Vel.x < 0) {
				dir = -1; 
			}
			else if (item->data->Vel.x > 0) {
				dir = 1; 
			}
		}

	}
	return dir;


}


bool j1Entity_Manager::Get_Gravity_Reverse() {
	bool ret = false;
	
	p2List_item<j1Entity*>* item;
	item = entities.start;

	for (; item != NULL; item = item->next)
	{
		if (item->data->type == Type::PLAYER) {
			if (item->data->gravity_reverse) {
				ret = true; 
			}
			else {
				ret = false; 
			}
		}

	}
	return ret;
}




bool j1Entity_Manager::Load(pugi::xml_node &node) //New: Save and Load methods, now are not working 
{
	p2List_item<j1Entity*>* item;
	
	item = entities.start;

	for (; item != NULL; item = item->next)
	{
		item->data->Load(node);
	}
	return true;
}

bool j1Entity_Manager::Save(pugi::xml_node &node) const
{
	// add brofiler category

	p2List_item<j1Entity*>* item;

	item = entities.start;

	for (; item != NULL; item = item->next)
	{
		item->data->Save(node);
	}
	return true;
}


