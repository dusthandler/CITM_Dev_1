#include "j1Entity_Manager.h"
#include "j1Entity.h"
#include "j1Collision.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Enemy_Walker.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "Brofiler/Brofiler.h"


j1Enemy_Walker::j1Enemy_Walker(iPoint position, Type type) : j1Entity(position, type) {

	collider = App->collision->AddCollider({ position.x, position.y,70,70 }, COLLIDER_ENEMY, this);  // "this" works, "entity manager" does not
	tex = App->tex->Load("Maps/Ninja/Ninja.png");
	animation = &Idle;
	Idle.PushBack({ 30, 2, 35, 45 });  // width and height now are 35 and 45

	dir_multiplier = 10;

	// testing
	
}


bool j1Enemy_Walker::Update(float dt) {

	BROFILER_CATEGORY("Enemy walker Update", Profiler::Color::Black);

	bool ret = true;
	
	/*Follow_Path();
	Move(dt);*/

	collider->SetPos(position.x, position.y);

	return ret;
}

bool j1Enemy_Walker::PostUpdate() {

	//Onplat = false;  // same as player

	return true;
}



void j1Enemy_Walker::Follow_Path() {

	BROFILER_CATEGORY("Enemy walker Pathfinding", Profiler::Color::BlanchedAlmond);

	iPoint origin = App->map->WorldToMap(position.x, position.y);
	iPoint dest = App->map->WorldToMap(App->entity_manager->GetPlayerPos().x, App->entity_manager->GetPlayerPos().y);                                           // change for player position

	if (App->scene->Player_Alive) {
		following_player = true; 

		App->pathfinding->CreatePath(origin, dest);
		Path = App->pathfinding->GetLastPath();                  // create path 

		Target_Map_Pos = iPoint(Path->At(0)->x, Path->At(0)->y);
		Enemy_Map_Pos = App->map->WorldToMap(position.x, position.y);           // position logic
		dir.x = Target_Map_Pos.x - Enemy_Map_Pos.x;
		dir.y = Target_Map_Pos.y - Enemy_Map_Pos.y;
	}
	else {
		following_player = false; 

		dir.x = 1 / dir_multiplier;
		dir.y = 1 / dir_multiplier;
	}
	

}

void j1Enemy_Walker::Move(float dt) {


	if (Onplat) {
		position.x += dir.x*dir_multiplier;      // *dt
		position.y += dir.y*dir_multiplier;     // *dt
	}
	else {
		this->position.y += Gravity;          //*dt
	}

}




void j1Enemy_Walker::OnCollision(Collider* c1, Collider* c2) {                                     // extracted from player

	BROFILER_CATEGORY("Enemy walker OnCollision", Profiler::Color::Blue);

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



bool j1Enemy_Walker::CleanUp() {
	BROFILER_CATEGORY("Enemy walker OnCollision", Profiler::Color::BlueViolet);
	collider->to_delete = true;
	App->tex->UnLoad(tex);
	return true;
}



