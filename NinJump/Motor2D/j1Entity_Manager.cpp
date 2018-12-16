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
#include "p2Log.h"
#include <time.h>

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
	player_startup_time.Start();


	srand(time(NULL));

	bool ret = true;
	// textures
	path_tex = App->tex->Load("Maps/Textures/path_tex.png");                // in tiled
    enemy_fly_tex = App->tex->Load("Maps/Enemies/Flyer/Fly.png");
    enemy_walk_tex = App->tex->Load("Maps/Enemies/Floor/floor_enemy.png");
	coin_tex = App->tex->Load("Maps/Objects/coins.png");
	shuriken_tex = App->tex->Load("Maps/Objects/shuriken.png");
	player_tex = App->tex->Load("Maps/Ninja/Ninja.png");
	

	// fxs
	App->audio->LoadFx("Sound/Fx/jump.wav"); // 3
	App->audio->LoadFx("Sound/Fx/death.wav"); // 4
	App->audio->LoadFx("Sound/Fx/landing.wav"); // 5
	App->audio->LoadFx("Sound/Fx/gravity_reverse.wav"); // 6
	App->audio->LoadFx("Sound/Fx/coin.wav");  // 7
	App->audio->LoadFx("Sound/Fx/shot.wav");  // 8


	iPoint Pos;
	pugi::xml_node InitPos = App->map->map_file.child("map");

	Pos.x = InitPos.child("tileset").child("terraintypes").child("terrain").child("properties").child("property").attribute("value").as_uint();
	Pos.y = InitPos.child("tileset").child("terraintypes").child("terrain").child("properties").child("property").next_sibling("property").attribute("value").as_uint();

	CreateEntity(Type::PLAYER, Pos); // , 0);




	
	if (App->scene->map_active == 0) {
		CreateEntity(Type::SHURIKEN, iPoint(470, 870));       // after player !

		for (int i = 1; i < 9; ++i) {
			CreateEntity(Type::COIN, iPoint(450 + i*coin_dist, 870));
		}
		CreateEntity(Type::ENEMY_FLYING, iPoint(2500, 150), 4);
		CreateEntity(Type::ENEMY_FLYING, iPoint(4000, 150), 3);
		CreateEntity(Type::ENEMY_LAND, iPoint(4500, 400), 1);
		
		CreateEntity(Type::COIN, iPoint(2690, 670));
		CreateEntity(Type::COIN, iPoint(4500, 1000));
		CreateEntity(Type::COIN, iPoint(4600, 1000));
		CreateEntity(Type::COIN, iPoint(4550, 1000));
	}

	else {
		CreateEntity(Type::SHURIKEN, iPoint(740, 520));       // after player !


		for (int i = 1; i < 6; ++i) {
			CreateEntity(Type::COIN, iPoint(150 + i * coin_dist, 870));
		}

		CreateEntity(Type::COIN, iPoint(1090, 870));
		CreateEntity(Type::COIN, iPoint(1250, 800));
		CreateEntity(Type::COIN, iPoint(1410, 740));
		CreateEntity(Type::COIN, iPoint(1480, 580));
		CreateEntity(Type::COIN, iPoint(1600, 700));
		CreateEntity(Type::COIN, iPoint(1635, 510));


		CreateEntity(Type::COIN, iPoint(4000, 490));
		CreateEntity(Type::COIN, iPoint(4400, 290));
		CreateEntity(Type::COIN, iPoint(3700, 620));

		for (int i = 1; i < 6; ++i) {
			CreateEntity(Type::COIN, iPoint(2300 + i * coin_dist, 580));
		}

		// enemies
	
		CreateEntity(Type::ENEMY_FLYING, iPoint(4000, 290), 3);
	}

	

	//CreateEntity(Type::ENEMY_LAND, iPoint(350, 200));
	//CreateEntity(Type::ENEMY_LAND, iPoint(250, 200));*/
  






   
	return ret;
}
j1Entity* j1Entity_Manager::CreateEntity(Type type, iPoint pos, int id) 
{
// 	static_assert(Type::UNKNOWN == (Type)3, "code needs update");
	j1Entity* ret = nullptr;
	switch (type) {
	case Type::ENEMY_FLYING: ret = new j1Enemy_Flying(pos,type,id); break; //New: Now we pass to paremeters to constructor
	case Type::ENEMY_LAND: ret = new j1Enemy_Walker(pos, type,id); break; //New: Land enemie :D
	case Type::PLAYER: ret = new j1Player_Entity(pos, type,id); break;
	case Type::COIN: ret = new j1Entity_Coin(pos, type,id); break;
	case Type::SHURIKEN: ret = new j1Shuriken(pos, type,id); break;
	}
	
	if (ret != nullptr) {
		entities.add(ret); // entities.push_back(ret);
	}

	return ret;
}


void j1Entity_Manager::Draw() {
	BROFILER_CATEGORY("Entity Manager Draw", Profiler::Color::BurlyWood);

	// if ( dt != 0) {
		p2List_item<j1Entity*>* item;
		item = entities.start;
		SDL_Rect Rect;

		for (item = entities.start; item != NULL; item = item->next)
		{

			Rect = item->data->animation->GetCurrentFrame();
		/*	if (!item->data->active) {
				Rect = { 0, 0, 0, 0 }; 
			}*/
		 if (item->data->active) {
				switch (item->data->type) {
				case Type::PLAYER:
					if (Get_Gravity_Reverse()) {
						App->render->Blit(player_tex, item->data->position.x, item->data->position.y, &Rect, 1, "player");
					}
					else {
						App->render->Blit(player_tex, item->data->position.x, item->data->position.y, &Rect, 1);
					}
					break;
				case Type::COIN:
					App->render->Blit(coin_tex, item->data->position.x, item->data->position.y, &Rect, 1);
					break;
				case Type::ENEMY_FLYING:
					App->render->Blit(enemy_fly_tex, item->data->position.x, item->data->position.y, &Rect, 1);
					break;
				case Type::ENEMY_LAND:
					App->render->Blit(enemy_walk_tex, item->data->position.x, item->data->position.y, &Rect, 1);
					break;
				case Type::SHURIKEN:
					App->render->Blit(shuriken_tex, item->data->position.x, item->data->position.y, &Rect, 1);
					break;
				}
			}
		}
	// }

}

bool j1Entity_Manager::Update(float dt)
{
	BROFILER_CATEGORY("Entity Manager Update", Profiler::Color::CadetBlue);
	bool ret = true;       
	
	
	if (dt != 0) {
		// TODO: add "do_logic" condition
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
		// ret = item->data->CleanUp();    // in the entity clean up, we already call destroy
		delete item->data;                 // so destroy must not call clean up; 
		item->data = nullptr;
		entities.del(item);
		LOG("                                                          !!!! entity destroyed !!!!"); 
		}
		
	}
	
}



bool j1Entity_Manager::CleanUp()      // as in App
{
	BROFILER_CATEGORY("Entity Manager Cleanup", Profiler::Color::Crimson);
	bool ret = true;
	p2List_item<j1Entity*>* item;  

	item = entities.start;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		 if (item->data->active) {
			
			 item->data->collider->to_delete = true;    // if an entity was still active, delete collider 
		 }
			// ret = item->data->CleanUp();
			delete item->data;                     // if not, just delete it
			item->data = nullptr;
		}
	
	entities.clear();

/*	// fxs
	App->audio->UnloadFx(0);
	App->audio->UnloadFx(5);           
	App->audio->UnloadFx(4);
	App->audio->UnloadFx(3);
	App->audio->UnloadFx(2);   
	App->audio->UnloadFx(1);  */

	// textures
	App->tex->UnLoad(player_tex);

	App->tex->UnLoad(shuriken_tex); 
	App->tex->UnLoad(coin_tex);
	App->tex->UnLoad(enemy_walk_tex);
	App->tex->UnLoad(enemy_fly_tex);
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


