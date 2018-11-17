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
	dir_multiplier = 5;
}

bool j1Enemy_Flying::Update(float dt) {
	bool ret = true;

	/*if (!Reached_Player)
		Follow_Path();*/

	collider->SetPos(this->position.x, this->position.y);

	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN) {                                // testing DestroyEntity
		to_delete = true;
		App->entity_manager->DestroyEntity(this);                // right now destroys ALL entites of this type
	}


	return ret;
}

bool j1Enemy_Flying::Draw() {
	bool ret = true;



	return ret;
}

void j1Enemy_Flying::Follow_Path() {

	
	iPoint origin = App->map->WorldToMap(this->position.x, this->position.y);
	iPoint dest = App->map->WorldToMap(GetPlayerPos().x, GetPlayerPos().y);                                           // change for player position

	App->pathfinding->CreatePath(origin, dest);            // create path 

	this->Path = App->pathfinding->GetLastPath();       // capture the path

	if (Path_Generated == false) {
		for (uint i = 0; i < this->Path->Count(); ++i) {
			if (i > 0) {
				this->dir.x = Path->At(i)->x - Path->At(i - 1)->x;             // direction between path nodes
				this->dir.y = Path->At(i)->y - Path->At(i - 1)->y;
			}
			else if (i == 0) {
				this->dir.x = Path->At(i)->x;   // this should be 0 at th start ?? 
				this->dir.y = Path->At(i)->y;
			}
			//	LOG("Enemy dir x is %i and y is %i", dir.x, dir.y);
		}
		Path_Generated = true; 
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


	LOG("Enemy flying is moving in this direction: %i,%i", dir.x, dir.y); 
}





bool j1Enemy_Flying::CleanUp() {
	collider->to_delete = true;
	App->tex->UnLoad(tex);
	return true;
}




