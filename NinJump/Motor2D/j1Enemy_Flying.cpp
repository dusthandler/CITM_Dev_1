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
	dir_multiplier = 5;
}

bool j1Enemy_Flying::Update(float dt) {

	BROFILER_CATEGORY("Enemy fly Update", Profiler::Color::Beige); 
	bool ret = true;

	if (!Reached_Player)
		Follow_Path(); 

	collider->SetPos(position.x, position.y);

	return ret;
}

bool j1Enemy_Flying::Draw() {
	bool ret = true;



	return ret;
}

void j1Enemy_Flying::Follow_Path() {

	BROFILER_CATEGORY("Enemy fly Pathfinding", Profiler::Color::Bisque);

	iPoint origin = App->map->WorldToMap(position.x, position.y);
	iPoint dest = App->map->WorldToMap(GetPlayerPos().x, GetPlayerPos().y);                                           // change for player position

	// if () {
		App->pathfinding->CreatePath(origin, dest);            // create path 
	//}

	Path = App->pathfinding->GetLastPath();       // capture the path

	/*for (uint i = 0; i < Path->Count(); ++i) {
		position = App->map->MapToWorld(Path->At(i)->x, Path->At(i)->y);    // move according to path
	}*/



	Path_Dir_Logic();
}



void j1Enemy_Flying::Move(float dt) {


	position.x += dir.x*dir_multiplier;      
	position.y += dir.y*dir_multiplier;


	// LOG("Enemy flying is moving in this direction: %i,%i", dir.x, dir.y); 
}





bool j1Enemy_Flying::CleanUp() {
	BROFILER_CATEGORY("Enemy fly CleanUp", Profiler::Color::Bisque);
	collider->to_delete = true;
	App->tex->UnLoad(tex);
	return true;
}




