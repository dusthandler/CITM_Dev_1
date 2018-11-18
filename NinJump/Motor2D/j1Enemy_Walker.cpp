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

	collider = App->collision->AddCollider({ position.x, position.y,70,70 }, COLLIDER_ENEMY, this); 
	tex = App->tex->Load("Maps/Enemies/Floor/floor_enemy.png");
	animation = &Iddle_Left;
	Iddle_Left.PushBack({ 0, 0, 35, 49 });
	Iddle_Left.PushBack({ 37, 0, 46, 49 });
	Iddle_Left.loop = true; 
	Iddle_Left.speed = 0.2f; 

	Walking_Left.PushBack({ 84, 0, 42, 49 });
	Walking_Left.PushBack({ 126, 0, 35, 49 });
	Walking_Left.PushBack({ 162, 0, 36, 49 });
	Walking_Left.loop = true; 
	Walking_Left.speed = 0.05f;

	Pushing_Left.PushBack({ 198, 0, 43, 49 });

	Iddle_Right.PushBack({ 0, 56, 35, 49 });
	Iddle_Right.PushBack({ 37, 56, 46, 49 });
	Walking_Right.PushBack({ 84, 56, 42, 49 });
	Walking_Right.PushBack({ 126, 56, 35, 49 });
	Walking_Right.PushBack({ 162, 56, 36, 49 });
	Walking_Right.loop = true;
	Walking_Right.speed = 0.05f;

	Pushing_Right.PushBack({ 198, 56, 43, 49 });

	Kicked_Left.PushBack({ 0, 115, 46, 49 });
	Kicked_Left.PushBack({ 50, 115, 45, 49 });
	Kicked_Left.PushBack({ 113, 115, 56, 34 });

	Kicked_Right.PushBack({ 0, 178, 46, 49 });
	Kicked_Right.PushBack({ 50, 178, 45, 49 });
	Kicked_Right.PushBack({ 113, 178, 56, 34 });

	Falling_Left.PushBack({ 189, 115, 40, 49 });
	Falling_Right.PushBack({ 190, 178, 40, 49 });

	dir_multiplier = 4;

	// testing
	
}


bool j1Enemy_Walker::Update(float dt) {

	BROFILER_CATEGORY("Enemy walker Update", Profiler::Color::Black);

	bool ret = true;
	
	Follow_Path();
	Move(dt);

	collider->SetPos(position.x, position.y);
	Set_Anim();

	return ret;
}

bool j1Enemy_Walker::PostUpdate() {

	// Onplat = false;  // same as player

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
		// position.y += dir.y*dir_multiplier;     // *dt
	}
	else {
		this->position.y += Gravity;          //*dt
	}

}


void j1Enemy_Walker::Set_Anim() {

	if (App->entity_manager->GetPlayerPos().x <= position.x) {
		animation = &Walking_Left;
	}
	else {
		animation = &Walking_Right;
	}



}


void j1Enemy_Walker::OnCollision(Collider* c1, Collider* c2) {                                     // extracted from player

	BROFILER_CATEGORY("Enemy walker OnCollision", Profiler::Color::Blue);

	// LOG("________________________ An enemy is colliding !!! _____________________________");

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

bool j1Enemy_Walker::Load(pugi::xml_node& node) {
	bool ret = true;


	position.x = node.child("Enemy_walker").attribute("x").as_int(0);
	position.y = node.child("Enemy_walker").attribute("y").as_int(0);
	Vel.x = node.child("Enemy_walker").attribute("vx").as_int(0);
	Vel.y = node.child("Enemy_walker").attribute("vy").as_int(0);
	Acc.x = node.child("Enemy_walker").attribute("ax").as_int(0);
	Acc.y = node.child("Enemy_walker").attribute("ay").as_int(0);
	dir.x = node.child("Enemy_walker").attribute("dirx").as_int(0);
	dir.y = node.child("Enemy_walker").attribute("diry").as_int(0);
	Path_Pos.x = node.child("Enemy_walker").attribute("pathx").as_int(0);
	Path_Pos.y = node.child("Enemy_walker").attribute("pathy").as_int(0);
	dir_multiplier = node.child("Enemy_walker").attribute("dir_m").as_uint(0);
	active = node.child("Enemy_walker").attribute("active").as_bool(false);
	to_delete = node.child("Enemy_walker").attribute("delet").as_bool(false);
	following_player = node.child("Enemy_walker").attribute("foll_pla").as_bool(false);
	Reached_Player = node.child("Enemy_walker").attribute("reached").as_bool(false);

	collider->type = COLLIDER_ENEMY;
	type = Type::ENEMY_LAND;


	//ene.append_attribute("dir_m") = dir_multiplier;
	//ene.append_attribute("act") = active;

	//ene.append_attribute("delet") = to_delete;
	//ene.append_attribute("foll_pla") = following_player;



	return ret;
}


bool j1Enemy_Walker::Save(pugi::xml_node& node) const
{
	bool ret = true;

	pugi::xml_node ene = node.append_child("Enemy_walker");

	ene.append_attribute("x") = position.x;
	ene.append_attribute("y") = position.y;

	ene.append_attribute("vx") = Vel.x;
	ene.append_attribute("vy") = Vel.y;

	ene.append_attribute("ax") = Acc.x;
	ene.append_attribute("ay") = Acc.y;

	ene.append_attribute("dirx") = dir.x;
	ene.append_attribute("diry") = dir.y;

	ene.append_attribute("pathx") = Path_Pos.x;
	ene.append_attribute("pathy") = Path_Pos.y;

	ene.append_attribute("dir_m") = dir_multiplier;
	ene.append_attribute("act") = active;
	ene.append_attribute("reached") = Reached_Player;

	ene.append_attribute("delet") = to_delete;
	ene.append_attribute("foll_pla") = following_player;



	return ret;
}







