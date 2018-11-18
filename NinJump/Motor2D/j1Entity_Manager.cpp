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
#include "Brofiler/Brofiler.h"


j1Entity_Manager::j1Entity_Manager() : j1Module()
{

}

// Destructor
j1Entity_Manager::~j1Entity_Manager()
{

}

bool j1Entity_Manager::Start(){
	BROFILER_CATEGORY("Entity Manager Start", Profiler::Color::Brown);

	bool ret = true; // Scene->Switch Para cambiar la posicion de los enemigos.
	//New: We will create the entyties here, that way is more easy to do the respawn.
	 CreateEntity(Type::ENEMY_FLYING, iPoint(250, 50));  //New: You can create a entity both ways.
	// App->entity_manager->CreateEntity(Type::ENEMY_LAND, iPoint(350, 50));
	

	iPoint Pos;
	pugi::xml_node InitPos = App->map->map_file.child("map");

	// we need to load this from tiled 
	Pos.x = InitPos.child("tileset").child("terraintypes").child("terrain").child("properties").child("property").attribute("value").as_uint();
	Pos.y = InitPos.child("tileset").child("terraintypes").child("terrain").child("properties").child("property").next_sibling("property").attribute("value").as_uint();
	CreateEntity(Type::PLAYER, Pos);


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
		App->render->Blit(item->data->tex, item->data->position.x, item->data->position.y, &Rect, 1);
		
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
	
}

void j1Entity_Manager::DestroyEntity(j1Entity* entity) {

	bool ret = true;
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

bool j1Entity_Manager::Load(pugi::xml_node &data) //New: Save and Load methods, now are not working 
{
	p2List_item<j1Entity*>* item;
	
	item = entities.start;

	for (; item != NULL; item = item->next)
	{
		item->data->Load(data);
	}
	return true;
}

bool j1Entity_Manager::Save(pugi::xml_node &data) const
{
	// add brofiler category

	p2List_item<j1Entity*>* item;

	item = entities.start;

	for (; item != NULL; item = item->next)
	{
		item->data->Save(data);
	}
	return true;
}


