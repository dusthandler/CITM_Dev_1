#include "j1Entity_Manager.h"
#include "j1Player.h"
#include "j1Enemy_Flying.h"
#include "j1Entity.h"




j1Entity::j1Entity(Type type): type(type)
{}


void j1Entity::SetPosition(int x, int y)
{
	position = { x,y };
}