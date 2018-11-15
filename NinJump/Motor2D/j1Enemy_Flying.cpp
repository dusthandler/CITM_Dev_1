#include "j1Entity_Manager.h"
#include "j1Entity.h"
#include "j1Enemy_Flying.h"
#include "j1Collision.h"
#include "j1App.h"



j1Enemy_Flying::j1Enemy_Flying() : j1Entity(Type::ENEMY_FLYING) {

	collider = App->collision->AddCollider({ 0,0,20,20 }, COLLIDER_DEATH, (j1Module*)App->entity_manager);
}
