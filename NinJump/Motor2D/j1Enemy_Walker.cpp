#include "j1Entity_Manager.h"
#include "j1Entity.h"
#include "j1Collision.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Enemy_Walker.h"
#include "j1Input.h"
#include "j1Scene.h"


j1Enemy_Walker::j1Enemy_Walker(iPoint position, Type type) : j1Entity(position, type) {

	collider = App->collision->AddCollider({ position.x, position.y,70,70 }, COLLIDER_ENEMY, (j1Module*)App->entity_manager);
	tex = App->tex->Load("Maps/Ninja/Ninja.png");
	animation = &Idle;
	Idle.PushBack({ 30, 2, 35, 45 });
}


bool j1Enemy_Walker::Update(float dt) {
	bool ret = true;
	                                                                              // testing map swap
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {

		App->scene->MapSwap(1);
		mapLo = 1;

	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {

		App->scene->MapSwap(0);
		mapLo = 0;

	}







	return ret;
}

bool j1Enemy_Walker::Draw() {
	bool ret = true;



	return ret;
}



