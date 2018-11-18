#include "j1Entity_Manager.h"
#include "j1Entity.h"
#include "j1Enemy_Flying.h"
#include "j1Collision.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1PathFinding.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "Brofiler/Brofiler.h"


j1Enemy_Flying::j1Enemy_Flying(iPoint position, Type type) : j1Entity(position, type) {

	collider = App->collision->AddCollider({ position.x, position.y,20,20 }, COLLIDER_ENEMY, this);
	tex = App->tex->Load("Maps/Ninja/Ninja.png");
	animation = &Idle;
	Idle.PushBack({ 55, 2, 35, 45 });
	this->position = position;

	// logic stuff
	dir_multiplier = 3;
}

bool j1Enemy_Flying::Update(float dt) {

	BROFILER_CATEGORY("Enemy fly Update", Profiler::Color::Beige); 
	bool ret = true;
	

	Follow_Path(); 
	Move(dt);

	collider->SetPos(position.x, position.y);

	return ret;
}


void j1Enemy_Flying::Follow_Path() {

	BROFILER_CATEGORY("Enemy fly Pathfinding", Profiler::Color::Bisque);

	iPoint origin = App->map->WorldToMap(position.x, position.y);
	iPoint dest = App->map->WorldToMap(App->entity_manager->GetPlayerPos().x, App->entity_manager->GetPlayerPos().y);                                           // change for player position

	 if (App->scene->Player_Alive) {
		 following_player = true; 

		App->pathfinding->CreatePath(origin, dest);     
		Path = App->pathfinding->GetLastPath();                  // create path 

		Target_Map_Pos = iPoint(Path->At(0)->x, Path->At(0)->y);
		Enemy_Map_Pos = App->map->WorldToMap(position.x, position.y);           // position logic
		dir.x = Target_Map_Pos.x - Enemy_Map_Pos.x; 
		dir.y = Target_Map_Pos.y - Enemy_Map_Pos.y;
     }
	 else {
		 following_player = false;

		 dir.x = 1 / dir_multiplier; 
		 dir.y = 1 / dir_multiplier; 
	 }
	 
	 

	 LOG("POSITIONS IN MAP ARE player %i, %i  and  enemy %i, %i", dest.x, dest.y, origin.x, origin.y);
	
}


void j1Enemy_Flying::Move(float dt) {


	position.x += dir.x*dir_multiplier;      // *dt
	position.y += dir.y*dir_multiplier;     // *dt

}



bool j1Enemy_Flying::CleanUp() {
	BROFILER_CATEGORY("Enemy fly CleanUp", Profiler::Color::Bisque);
	collider->to_delete = true;
	App->tex->UnLoad(tex);
	return true;
}




