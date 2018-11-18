#include "j1Entity_Manager.h"
#include "j1Enemy_Flying.h"
#include "j1Entity.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Pathfinding.h"


j1Entity::j1Entity(iPoint pos, Type type): position(pos), type(type)
{
}


Direction_State* j1Entity::Get_Direction_State(){

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
}
