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

j1Enemy_Flying::j1Enemy_Flying(iPoint position, Type type) : j1Entity(position, type) {

	collider = App->collision->AddCollider({ position.x, position.y,20,20 }, COLLIDER_ENEMY, this);
	tex = App->tex->Load("Maps/Ninja/Ninja.png");
	animation = &Idle;
	Idle.PushBack({ 55, 2, 35, 45 });
	this->position = position; 
	
	// logic stuff
	dir_multiplier = 10; 
}

bool j1Enemy_Flying::Update(float dt) {
	bool ret = true;

	if(!Reached_Player)
	Follow_Path(); 

	collider->SetPos(this->position.x, this->position.y); 

	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN) {                                // testing DestroyEntity
		to_delete = true; 
		App->entity_manager->DestroyEntity(this); 
	}



	LOG("Flyer pos is %i", position.x); 
	return ret;
}

bool j1Enemy_Flying::Draw() {
	bool ret = true;



	return ret;
}

void j1Enemy_Flying::Follow_Path() {


	j1Entity::Follow_Path(); 
	
	Path_Dir_Logic(); 

	// LOG(" <<<<<<<<<<<<<<<<<<<<<<<<    PATH HAS %i elements >>>>>>>>>>>>>>>>>>>", this->Path->Count()); 

	/*if (!Reached_Player) {
		if (this->position == dest) {
			Reached_Player = true; 
		}
	}*/


	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) App->scene->MapSwap(1);

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) App->scene->MapSwap(0);
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

	this->position.x += dir.x*dir_multiplier;        // LATER, it can be changed to player pos - enemy pos (easier)
	this->position.y += dir.y*dir_multiplier;
}



