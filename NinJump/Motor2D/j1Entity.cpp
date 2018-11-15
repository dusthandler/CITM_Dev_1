#include "j1Entity_Manager.h"
#include "j1Player.h"
#include "j1Enemy_Flying.h"
#include "j1Entity.h"




j1Entity::j1Entity(iPoint pos): position(pos)
{
}

j1Entity::~j1Entity(){}

bool j1Entity::PreUpdate(float dt) {
	// TODO: should we update the colliders here or in entity_manager ?? 
}