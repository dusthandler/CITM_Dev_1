#include "j1Entity_Manager.h"
#include "j1Enemy_Flying.h"
#include "j1Entity.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Pathfinding.h"


j1Entity::j1Entity(iPoint pos, Type type): position(pos), type(type)
{
}




	// TODO: should we update the colliders here or in entity_manager ?? 


void j1Entity::Follow_Path() {

	iPoint origin = App->map->WorldToMap(this->position.x, this->position.y);
	iPoint dest = App->map->WorldToMap(1000, 100);                                           // change for player position

	App->pathfinding->CreatePath(origin, dest);            // create path 

	this->Path = App->pathfinding->GetLastPath();       // capture the path

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
}
