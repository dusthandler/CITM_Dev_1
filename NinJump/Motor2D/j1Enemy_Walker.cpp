#include "j1Entity_Manager.h"
#include "j1Entity.h"
#include "j1Collision.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Enemy_Walker.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "p2Log.h"

j1Enemy_Walker::j1Enemy_Walker(iPoint position, Type type) : j1Entity(position, type) {

	collider = App->collision->AddCollider({ position.x, position.y,70,70 }, COLLIDER_ENEMY, this);  // "this" works, "entity manager" does not
	tex = App->tex->Load("Maps/Ninja/Ninja.png");
	animation = &Idle;
	Idle.PushBack({ 30, 2, 35, 45 });  // width and height now are 35 and 45
}


bool j1Enemy_Walker::Update(float dt) {
	bool ret = true;
	                                                                              // testing map swap
	

	collider->SetPos(this->position.x, this->position.y);

	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN) {                        // testing DestroyEntity
		to_delete = true;
		App->entity_manager->DestroyEntity(this);
	}


	if (!Reached_Player)
		Follow_Path();



	return ret;
}

bool j1Enemy_Walker::PostUpdate(){

	Onplat = false;  // same as player

	return true; 
}

bool j1Enemy_Walker::Draw() {
	bool ret = true;



	return ret;
}

void j1Enemy_Walker::Follow_Path() {

	j1Entity::Follow_Path();

	Path_Dir_Logic();
}

void j1Enemy_Walker::Path_Dir_Logic() {

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

	if (Onplat) {
		this->position.x += dir.x*(int)dir_multiplier;
	}
	else {
		this->position.y += Gravity;
	}

	LOG("dir x =============================== %i", dir.x); 
}


         
void j1Enemy_Walker::OnCollision(Collider* c1, Collider* c2) {                                     // extracted from player

	// Onplat = true;

	LOG("________________________ An enemy is colliding !!! _____________________________"); 

	if (c2->type == COLLIDER_WALL) {

		if (c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y + c1->rect.h <= c2->rect.y + 20) {
			dir.y = 0;
			position.y = c2->rect.y - 45;   // 45 = height

			Onplat = true;             

		}
		if (c1->rect.y >= c2->rect.h + c2->rect.y - 20 && c1->rect.y <= c2->rect.h + c2->rect.y) {
			position.y = c2->rect.y + c2->rect.h;
		}
		if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.h + c1->rect.y >= c2->rect.y + 10) {

			if ((c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w <= c2->rect.x + 20)) {
				if (dir.x > 0) {
					position.x = c2->rect.x - 35;  // 35 = width
					dir.x = 0;
				}
			}
			else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x > c2->rect.w + c2->rect.x - 20) {
				if (dir.x < 0) {
					position.x = c2->rect.x + c2->rect.w;
					dir.x = 0;
				}
			}

		}


	}


	/*
	if (c2->type == COLLIDER_DEATH) {


	}


	if (c2->type == COLLIDER_PLAYER) {


	}*/

}

