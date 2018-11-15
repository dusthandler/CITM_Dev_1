#include "j1Entity_Manager.h"
#include "j1Entity.h"
#include "j1Collision.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Enemy_Walker.h"



j1Enemy_Walker::j1Enemy_Walker(iPoint position, Type type) : j1Entity(position, type) {

	collider = App->collision->AddCollider({ position.x, position.y,70,70 }, COLLIDER_ENEMY, (j1Module*)App->entity_manager);
	tex = App->tex->Load("Maps/Ninja/Ninja.png");
	animation = &Idle;
	Idle.PushBack({ 30, 2, 35, 45 });
}


bool j1Enemy_Walker::Update(float dt) {
	bool ret = true;








	return ret;
}

bool j1Enemy_Walker::Draw() {
	bool ret = true;



	return ret;
}



