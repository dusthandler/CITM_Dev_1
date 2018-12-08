#include "j1Entity_Manager.h"
#include "j1Enemy_Flying.h"
#include "j1Entity.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Pathfinding.h"
#include "p2Log.h"


j1Entity::j1Entity(iPoint pos, Type type): position(pos), type(type)
{
}


void j1Entity::Get_Direction_State(){

	if (dir.x == 0 && dir.y == 0) {                       // know the direction
		m_state = Direction_State::STOP;
	}
	else if (dir.x > 0 && dir.y == 0) {
		m_state = Direction_State::RIGHT;
	}
	else if (dir.x < 0 && dir.y == 0) {
		m_state = Direction_State::LEFT;
	}
	else if (dir.x == 0 && dir.y > 0) {
		m_state = Direction_State::DOWN;
	}
	else if (dir.x == 0 && dir.y < 0) {
		m_state = Direction_State::UP;
	}


	// LOG("Enemy with direction: (%i,%i)", dir.x, dir.y); 
}



Camera_Limits j1Entity::Inside_Camera_Limits() {

	struct Camera_Limits limits = { true, true, true, true}; 

	if (App->render->camera.x == 0) {
		if (position.x < App->render->camera.x) {
			limits.left_x = false; 
		}
	}
	else if(-position.x > App->render->camera.x){
		limits.left_x = false;
	}
	else if (-position.x < App->render->camera.x - App->render->camera.w) {
		limits.right_x = false; 
	}
	
	if (!limits.left_x || !limits.right_x || !limits.top_y || !limits.bottom_y) {
		inside_limits = false; 
	}
	else {
		inside_limits = true; 
	}

	return limits; 
}