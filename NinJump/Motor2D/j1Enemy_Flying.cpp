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
	
	// logic stuff
	dir_multiplier = 10; 
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



	/*	App->pathfinding->CreatePath(origin, dest); */                                        // create proper path 
	
	App->pathfinding->CreatePath(iPoint(5,5), iPoint(20,10));                                 // create random path 


	this->Path = App->pathfinding->GetLastPath();  // capture the path

	for (uint i = 0; i < this->Path->Count(); ++i) {
		if (i>0) {
			this->dir.x = Path->At(i)->x - Path->At(i - 1)->x;             // direction between path nodes
			this->dir.y = Path->At(i)->y - Path->At(i - 1)->y;
		}
		else if (i == 0) {
			this->dir.x = Path->At(i)->x;         
			this->dir.y = Path->At(i)->y;
		}
		LOG(" -----------------------     Enemy dir x is %i and y is %i", dir.x, dir.y);
	}

	if (dir.x == 0 && dir.y == 0) {                       // know the direction
		m_state = Movement_State::STOP; 
	}
	else if (dir.x == 1 && dir.y == 0) {
		m_state = Movement_State::RIGHT;
	}
	else if (dir.x == -1 && dir.y == 0) {
		m_state = Movement_State::LEFT;
	}
	else if (dir.x == 0 && dir.y == 1) {
		m_state = Movement_State::DOWN;
	}
	else if (dir.x == 0 && dir.y == -1) {
		m_state = Movement_State::UP;
	}
	
	Path_Dir_Logic(); 

	LOG(" <<<<<<<<<<<<<<<<<<<<<<<<    PATH HAS %i elements >>>>>>>>>>>>>>>>>>>", this->Path->Count()); 

}



void j1Enemy_Flying::Path_Dir_Logic() {
	

	switch (m_state) {
	case Movement_State::RIGHT:
		
		break;
	case Movement_State::LEFT:
	
		break;
	case Movement_State::UP:
		
		break;
	case Movement_State::DOWN:
		
		break;
	case Movement_State::STOP:
		
		break;
	}

	this->position.x += dir.x*dir_multiplier;
	this->position.y += dir.y*dir_multiplier;
}



