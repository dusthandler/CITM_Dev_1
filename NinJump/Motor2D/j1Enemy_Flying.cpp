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

j1Enemy_Flying::j1Enemy_Flying(iPoint position, Type type,int id) : j1Entity(position, type,id) {

	collider = App->collision->AddCollider({ position.x, position.y,Fly_Width,45 }, COLLIDER_ENEMY, this);
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
	this->my_id = id;

	// logic stuff
	dir_multiplier = 3;
}

bool j1Enemy_Flying::Update(float dt) {

	BROFILER_CATEGORY("Enemy fly Update", Profiler::Color::Beige); 
	bool ret = true;            

	uint activation_distance = App->map->MapToWorld(31, 0).x; 

	if (!start_following && position.x - App->entity_manager->GetPlayerPos().x < activation_distance) {
		start_following = true; 
		LOG("Enemy ready to follow player "); 
	}

	if (start_following) {
		Inside_Camera_Limits();
		if (inside_limits) {
			LOG("Enemy is pathfinding ... ... ... ... ... ... "); 
			Follow_Path();
		}
		Move(dt);
	}

	collider->SetPos(position.x, position.y);
	Set_Anim();
	

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

		if (App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN) {   // change to F9

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

	if (!Inside_Camera_Limits().left_x) {           // x
		position.x += 3; 
	}
	else if (!Inside_Camera_Limits().right_x) {
		position.x -= 3;
	}
	else if (!Inside_Camera_Limits().top_y) {                                   // y
		position.y += 20;
	}
	else if (!Inside_Camera_Limits().bottom_y) {
		position.y -= 20;
	}

	else {
		position.x += dir.x*dir_multiplier;      // *dt
		position.y += dir.y*dir_multiplier;     // *dt
	}
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
//    	App->render->Blit(path_tex, surface_pos.x, surface_pos.y); 
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
	active = false; 


	DestroyEntity(this); 
	return true;
}


bool j1Enemy_Flying::Load(pugi::xml_node& node) {
	bool ret = true;
	p2SString EntityName;
	EntityName.create("Enemie_Fliying_%i", my_id);

	position.x = node.child(EntityName.GetString()).attribute("x").as_int(0);
	position.y = node.child(EntityName.GetString()).attribute("y").as_int(0);
	Vel.x = node.child(EntityName.GetString()).attribute("vx").as_int(0);
	Vel.y = node.child(EntityName.GetString()).attribute("vy").as_int(0);
	Acc.x = node.child(EntityName.GetString()).attribute("ax").as_int(0);
	Acc.y = node.child(EntityName.GetString()).attribute("ay").as_int(0);
	dir.x = node.child(EntityName.GetString()).attribute("dirx").as_int(0);
	dir.y = node.child(EntityName.GetString()).attribute("diry").as_int(0);
	Path_Pos.x = node.child(EntityName.GetString()).attribute("pathx").as_int(0);
	Path_Pos.y = node.child(EntityName.GetString()).attribute("pathy").as_int(0);
	dir_multiplier = node.child(EntityName.GetString()).attribute("dir_m").as_uint(0);
	active = node.child(EntityName.GetString()).attribute("active").as_bool(false);
	to_delete = node.child(EntityName.GetString()).attribute("delet").as_bool(false);
	following_player = node.child(EntityName.GetString()).attribute("foll_pla").as_bool(false);
	Reached_Player = node.child(EntityName.GetString()).attribute("reached").as_bool(false);

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
	p2SString EntityName;
	EntityName.create("Enemie_Fliying_%i", my_id);
	LOG(EntityName.GetString());
	pugi::xml_node ene = node.append_child(EntityName.GetString());

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





