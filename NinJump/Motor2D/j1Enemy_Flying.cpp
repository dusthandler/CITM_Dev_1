#include "j1Entity_Manager.h"
#include "j1Entity.h"
#include "j1Enemy_Flying.h"
#include "j1Collision.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1PathFinding.h"
#include "p2Log.h"
#include "j1Map.h"

j1Enemy_Flying::j1Enemy_Flying(iPoint position, Type type) : j1Entity(position, type) {

	collider = App->collision->AddCollider({ position.x, position.y,20,20 }, COLLIDER_ENEMY, (j1Module*)App->entity_manager);
	tex = App->tex->Load("Maps/Ninja/Ninja.png");
	animation = &Idle;
	Idle.PushBack({ 55, 2, 35, 45 });
	this->position = position; 
	
}

bool j1Enemy_Flying::Update(float dt) {
	bool ret = true;

	Follow_Path(); 


	return ret;
}

bool j1Enemy_Flying::Draw() {
	bool ret = true;



	return ret;
}

void j1Enemy_Flying::Follow_Path() {


    iPoint origin = App->map->WorldToMap(this->position.x, this->position.y);
	iPoint dest = App->map->WorldToMap(100, 200);                                           // change for player position


	// check if position is in the map
	if (App->pathfinding->CheckBoundaries(origin) && App->pathfinding->CheckBoundaries(dest)) {
		App->pathfinding->CreatePath(origin, dest);                                         // create path 
		LOG("Positions are inside the boundaries !!"); 
	}

	this->Path = App->pathfinding->GetLastPath();  // capture the path

	for (uint i = 0; i < this->Path->Count(); ++i) {
		// if () {
			this->dir.x = Path->At(i)->x - Path->At(i - 1)->x;             // direction between path nodes
			this->dir.y = Path->At(i)->y - Path->At(i - 1)->y;
			LOG(" -----------------------     Enemy dir x is %i and y is %i", dir.x, dir.y); 
		//}
	}

	LOG(" <<<<<<<<<<<<<<<<<<<<<<<<    PATH HAS %i elements >>>>>>>>>>>>>>>>>>>", this->Path->Count()); 

}



