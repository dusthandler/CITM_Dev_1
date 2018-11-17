#include "j1Entity_Manager.h"
#include "j1Enemy_Flying.h"
#include "j1Entity.h"




j1Entity::j1Entity(iPoint pos, Type type): position(pos), type(type)
{
}




	// TODO: should we update the colliders here or in entity_manager ?? 
