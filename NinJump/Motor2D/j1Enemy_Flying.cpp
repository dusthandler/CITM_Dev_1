#include "j1Entity_Manager.h"
#include "j1Entity.h"
#include "j1Enemy_Flying.h"
#include "j1Collision.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1PathFinding.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "Brofiler/Brofiler.h"
#include "j1Render.h"

j1Enemy_Flying::j1Enemy_Flying(iPoint position, Type type) : j1Entity(position, type) {

	collider = App->collision->AddCollider({ position.x, position.y,Fly_Width,45 }, COLLIDER_ENEMY, this);
	tex = App->tex->Load("Maps/Enemies/Flyer/Fly.png");
	animation = &Idle;
	Idle.PushBack({ 55, 2, 35, 45 });


	Right.PushBack({ 566, 21, Fly_Width, 45 });
	Right.PushBack({ 483, 17, Fly_Width, 50 });
	Right.PushBack({ 404, 21, Fly_Width, 47 });
	Right.PushBack({ 329, 28, Fly_Width, 39 });
	Right.loop = true; 
	Right.speed = 0.2f; 

	Left.PushBack({ 10, 21, Fly_Width, 45 });
	Left.PushBack({ 91, 17, Fly_Width, 50 });
	Left.PushBack({ 172, 21, Fly_Width, 47 });
	Left.PushBack({ 248, 28, Fly_Width, 39 });
	Left.loop = true;
	Left.speed = 0.2f;


	this->position = position;

	// logic stuff
	dir_multiplier = 3;
}

bool j1Enemy_Flying::Update(float dt) {

	BROFILER_CATEGORY("Enemy fly Update", Profiler::Color::Beige); 
	bool ret = true;            


	iPoint surface_pos;

	Inside_Camera_Limits(); 
	if (inside_limits) {
		Follow_Path();
	}
		Move(dt);
	

	collider->SetPos(position.x, position.y);
	Set_Anim();
	
	LOG("Camera x -------------> %i", App->render->camera.x);
	LOG("Player x -------------> %i", App->entity_manager->GetPlayerPos().x);
	LOG("Enemy x -------------> %i", position.x);

	return ret;
}


void j1Enemy_Flying::Follow_Path() {

	BROFILER_CATEGORY("Enemy fly Pathfinding", Profiler::Color::Bisque);

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

		if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {   // change to F9

		if(!Drawing_Path)
			Drawing_Path = true; 

			else {
				Drawing_Path = false; 
			}
			LOG("Draw path autorized"); 
		}

     }
	 else {
		 following_player = false;

		 dir.x = 1 / dir_multiplier; 
		 dir.y = 1 / dir_multiplier; 
	 }
	 
	 if(Drawing_Path)
	 Draw_Path();


	 // LOG("POSITIONS IN MAP ARE player %i, %i  and  enemy %i, %i", dest.x, dest.y, origin.x, origin.y);
	
}


void j1Enemy_Flying::Move(float dt) {

	if (!Inside_Camera_Limits().left_x) {
		position.x += 3; 
	}
	else if (!Inside_Camera_Limits().right_x) {
		position.x -= 3;
	}
	position.x += dir.x*dir_multiplier;      // *dt
	position.y += dir.y*dir_multiplier;     // *dt
}


void j1Enemy_Flying::Set_Anim() {

	if (App->entity_manager->GetPlayerPos().x <= position.x) {
		animation = &Left; 
	}
	else {
		animation = &Right; 
	}

}

void j1Enemy_Flying::Draw_Path() {

	LOG("Draw path doing");

	// drawing not working now
	iPoint surface_pos; 

	for (uint i = 0; i < Path->Count(); ++i) {

		surface_pos = App->map->MapToWorld(Path->At(i)->x, Path->At(i)->y);
		SDL_Rect rect;
		rect.x = surface_pos.x; 
		rect.y = surface_pos.y; 
		rect.h = App->map->data.tile_height; 
		rect.w = App->map->data.tile_width; 

		App->render->DrawQuad(rect, 0, 255, 255, 50, true, true); 
		}


}

void j1Enemy_Flying::OnCollision(Collider* c1, Collider* c2) {

	

	
	if (c2->type == COLLIDER_WALL) {

		if (c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y + c1->rect.h <= c2->rect.y + 20) {
			dir.y = 0;
			position.y = c2->rect.y - 45;   // 45 = height


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

	if (c2->type == COLLIDER_SHOT) {
		CleanUp(); 
	}

}

bool j1Enemy_Flying::CleanUp() {
	BROFILER_CATEGORY("Enemy fly CleanUp", Profiler::Color::Bisque);
	collider->to_delete = true;
	App->tex->UnLoad(tex);
	return true;
}


bool j1Enemy_Flying::Load(pugi::xml_node& node) {
	bool ret = true;


	position.x = node.child("Enemy_Flying").attribute("x").as_int(0);
	position.y = node.child("Enemy_Flying").attribute("y").as_int(0);
	Vel.x = node.child("Enemy_Flying").attribute("vx").as_int(0);
	Vel.y = node.child("Enemy_Flying").attribute("vy").as_int(0);
	Acc.x = node.child("Enemy_Flying").attribute("ax").as_int(0);
	Acc.y = node.child("Enemy_Flying").attribute("ay").as_int(0);
	dir.x = node.child("Enemy_Flying").attribute("dirx").as_int(0);
	dir.y = node.child("Enemy_Flying").attribute("diry").as_int(0);
	Path_Pos.x = node.child("Enemy_Flying").attribute("pathx").as_int(0);
	Path_Pos.y = node.child("Enemy_Flying").attribute("pathy").as_int(0);
	dir_multiplier = node.child("Enemy_Flying").attribute("dir_m").as_uint(0);
	active = node.child("Enemy_Flying").attribute("active").as_bool(false);
	to_delete = node.child("Enemy_Flying").attribute("delet").as_bool(false);
	following_player = node.child("Enemy_Flying").attribute("foll_pla").as_bool(false);
	Reached_Player = node.child("Enemy_Flying").attribute("reached").as_bool(false);

	collider->type = COLLIDER_ENEMY;
	type = Type::ENEMY_FLYING;


	//ene.append_attribute("dir_m") = dir_multiplier;
	//ene.append_attribute("act") = active;

	//ene.append_attribute("delet") = to_delete;
	//ene.append_attribute("foll_pla") = following_player;



	return ret;
}


bool j1Enemy_Flying::Save(pugi::xml_node& node) const
{
	bool ret = true;

	pugi::xml_node ene = node.append_child("Enemy_Flying");

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





