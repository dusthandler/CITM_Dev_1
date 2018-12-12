#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "SDL/include/SDL_timer.h"
#include "j1FadeBlack.h"
#include "j1Window.h"
#include "j1Player_Entity.h"
#include "Brofiler/Brofiler.h"
#include "j1Gui.h"


j1Player_Entity::j1Player_Entity(iPoint position, Type type, int id) : j1Entity(position, type,id) 
{
	//First we load the tex and the animations.

	this->position = position;
	this->type = type;
	this->my_id = id;
	// tex = App->tex->Load("Maps/Ninja/Ninja.png");
	// tex = player_tex; 
	animation = &Idle;
	Idle.PushBack({ 55, 2, PLAYER_WIDTH, PLAYER_HEIGHT });       
	Walking_Right.PushBack({ 3, 96, 44, 45 });
	Walking_Left.PushBack({ 341, 95, 44, 45 });
	Walking_Right2.PushBack({ 55, 96, 42, 45 });
	Walking_Left2.PushBack({ 298, 95, 42, 45 });

	Jumping_Up.PushBack({ 100, 143, PLAYER_WIDTH, PLAYER_HEIGHT });
	Jumping_Right.PushBack({ 8, 49, 31, 46 });
	Jumping_Left.PushBack({ 348, 49, 31, 46 });

	Falling_Right.PushBack({ 103, 49, 32, 45 });
	Falling_Left.PushBack({ 253, 49, 32, 45 });


	Death.PushBack({ 149, 1, PLAYER_WIDTH, PLAYER_HEIGHT });

	Falling.PushBack({ 56, 143,PLAYER_WIDTH, PLAYER_HEIGHT });
	App->scene->Player_Alive = true;
	Reset_Fx_2 = true; 



	
	Respawning = false;
	
	collider = App->collision->AddCollider({ position.x, position.y, 35, 45 }, COLLIDER_PLAYER, this);

	App->gui->coins_collected = 0;

}


// Destructor
j1Player_Entity::~j1Player_Entity()
{
}


bool j1Player_Entity::PreUpdate() {
	BROFILER_CATEGORY("Player PreUpdate", Profiler::Color::Cyan);
	//We move the collider first.
	collider->SetPos(position.x, position.y);
	return true;
}


void  j1Player_Entity::OnCollision(Collider* c1, Collider* c2) {


	BROFILER_CATEGORY("Player OnCollision", Profiler::Color::DarkCyan);


	if ((c1->type == COLLIDER_PLAYER || c1->type == COLLIDER_GOD) && c2->type == COLLIDER_WALL) {

		if (Reset_Fx_3 && Vel.y > 0) {
			App->audio->PlayFx(3, 0);
			Reset_Fx_3 = false;
		}

		if (c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y + c1->rect.h <= c2->rect.y + 20) {
			Vel.y = 0;
			position.y = c2->rect.y - PLAYER_HEIGHT;

			if (!gravity_reverse) {
				Onplat = true;             // without gravity, it lands
			}
			else {
				Onplat = false;            // with gravity, it falls
			}
			Jumping = false;
		}
		if (c1->rect.y >= c2->rect.h + c2->rect.y - 20 && c1->rect.y <= c2->rect.h + c2->rect.y) {
			position.y = c2->rect.y + c2->rect.h;
			if (gravity_reverse) {
				Onplat = true;                // with gravity, roof is now floor
			}
		}
		if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.h + c1->rect.y >= c2->rect.y + 10) {

			if ((c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w <= c2->rect.x + 20)) {
				if (Vel.x > 0) {
					position.x = c2->rect.x - PLAYER_WIDTH;
					Vel.x = 0;
				}
			}
			else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x > c2->rect.w + c2->rect.x - 20) {
				if (Vel.x < 0) {
					position.x = c2->rect.x + c2->rect.w;
					Vel.x = 0;
				}
			}

		}


	}

	if (c2->type == COLLIDER_DEATH || c2->type == COLLIDER_ENEMY) {


		if (!God_Mode) {

			App->entity_manager->player_live_count--;          // substract life total

			App->scene->Player_Alive = false;

			if (Reset_Fx_2) {

				App->audio->PlayFx(2, 0);
				Reset_Fx_2 = false;
				Vel.y = 0;
			}
		}

		else {

			if (c1->rect.y <= c2->rect.y) {     // god player cant die 

				if (Vel.y >= 0) {

					Onplat = true;
					Jumping = false;
					Vel.y = 0;
					position.y = c2->rect.y - PLAYER_HEIGHT;

				}

			}

		}
	}


	if (c2->type == COLLIDER_WIN) {
		App->entity_manager->player_live_count = 3; 
		App->scene->Player_Win= true;
	}


}


bool j1Player_Entity::Update(float dt) {

	BROFILER_CATEGORY("Player Update", Profiler::Color::DarkBlue);

	Get_Player_State();
	Debug_Keys();
	if (God_Mode) {
		collider->type = COLLIDER_TYPE::COLLIDER_GOD;
	}
	else {
		collider->type = COLLIDER_TYPE::COLLIDER_PLAYER;
	}
	Movex(dt);
	Movey(dt);
	Solve_Move(dt);

	


	

	return true;





}

PlayerStat j1Player_Entity::Get_Player_State() {
	if (App->scene->Player_Alive) {

		if (!App->scene->Player_Win) {   // IN THE LEVEL 

			if (!Onplat) {   // IN THE AIR

				if (Vel.y < 0) {                 // GOING UP 

					if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
						State = JUMPING_LEFTT;
						animation = &Jumping_Left;
					}

					else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
						State = JUMPING_RIGHTT;
						animation = &Jumping_Right;
					}
					else {
						State = JUMPING_UPP;
						animation = &Jumping_Up;
					}
				}

				else {                            // GOING DOWN

					if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
						State = FALLING_LEFTT;
						animation = &Falling_Left;
					}

					else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
						State = FALLING_RIGHTT;
						animation = &Falling_Right;
					}
					else {
						State = FALLING_DOWNN;
						animation = &Falling;
					}
				}

			}

			else {    // IN THE GROUND


				if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

					State = WALKING_LEFTT;

					if (Vel.x > -MAX_SPEED_X) {

						animation = &Walking_Left2;
					}

					else {
						animation = &Walking_Left;
					}
				}


				else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {

					State = WALKING_RIGHTT;

					if (Vel.x < MAX_SPEED_X) {

						animation = &Walking_Right2;
					}
					else {
						animation = &Walking_Right;
					}
				}


				else {

					State = IDLEE;
					animation = &Idle;
				}
			}

		}

		else {

			State = WINN;
		}

	}

	else {

		State = DIEDD;
		animation = &Death;
	}


	return State;

}


void j1Player_Entity::Debug_Keys() {

	//TDO: Mirar la logica para que funcione bien.
	

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN) {   // CHANGE TO F10	

		if (!God_Mode) {
			God_Mode = true;
		}
		else {
			God_Mode = false;
		}
	}


	// F9 located in collision module 
	if (!Respawning) {

		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {  // change for a tiled variable

			gravity_times++; 
			if (gravity_times < 3) {
				if (!gravity_reverse) {

					gravity_reverse = true;
					if (Reset_Fx_Gravity) {
						App->audio->PlayFx(4, 0);
						Reset_Fx_Gravity = false;
					}
				}

				else {
					gravity_reverse = false;
					if (!Reset_Fx_Gravity) {
						App->audio->PlayFx(4, 0);
						Reset_Fx_Gravity = true;
					}
				}
			}
		}
	}

}

void j1Player_Entity::Movex(float dt) {

	if (!Respawning) {
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

			if (position.x <= 0) {
				Vel.x = 0;
			}
			else {

				if (Vel.x == 0) {
					Cont_X = 0;
				}


				if (Vel.x > -MAX_SPEED_X) {

					Vel.x = (-200 + Cont_X)*dt;
					Cont_X -= 100.0f*dt;
					/*	Vel.x = -200 * dt;*/
				}

				else {
					Vel.x = (-200 + Cont_X)*dt;
				}


			}
		}

		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {

			if (Vel.x == 0) {
				Cont_X = 0;
			}

			if (Vel.x < MAX_SPEED_X) {
				/*	Vel.x = 200 * dt;*/
				Vel.x = (200 + Cont_X)*dt;
				Cont_X += 100.0f*dt;
			}

			else {
				Vel.x = (200 + Cont_X)*dt;
			}

		}
		else
			Vel.x = 0;

	}

	else {
		Vel.x = 0;
	}

}

void j1Player_Entity::Movey(float dt) {
	// Change variables to can get the map



	if (!Respawning) {

		if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)) {

			Jump_Count++;


			if (God_Mode) {   // god player can jump infinitely
				Vel.y = -800;
				Cont = 20;
				Jumping = true;
				App->audio->PlayFx(1, 0);
			}								
			else if (Jump_Count < 2) {
				Vel.y = -200;

				Jumping = true;
				App->audio->PlayFx(1, 0);
			}
		}

		else if (Jumping) {
			if (Vel.y <= -70) {
				Vel.y += 200 * dt;
			}
			else {
				Cont += 200 * dt;
				Vel.y += Cont*dt;
			}
		}
		else if (!Onplat && !Jumping) {

			Vel.y += 200 * dt;
		}
	}

	else {
		Vel.y += 150 * dt;
	}

	if (Vel.y > MAX_SPEED_Y) {
		Vel.y = MAX_SPEED_Y;

	}
	else if (Vel.y < -MAX_SPEED_Y) {
		Vel.y = -MAX_SPEED_Y;
	}


}

void j1Player_Entity::Solve_Move(float dt) {

	if (position.y < -PLAYER_HEIGHT && gravity_reverse) {      // sky limit
		App->scene->Player_Alive = false;
		gravity_reverse = false;
		Respawning = true;
	}

	Acc.y = 13;
	if (!Onplat) {
		Acc.y = 4;
		if (!Reset_Fx_3) {
			Reset_Fx_3 = true;                   // So that landing fx are available next time
		}
	}

	else {
		Acc.y = 0;
		Jump_Count = 0;  // reset double jump
	}

	position.x += Vel.x;
	if (!gravity_reverse) {
		position.y += (Vel.y + Acc.y)*dt;
	}
	else {
		position.y -= (Vel.y + Acc.y)*dt;
	}

}


bool j1Player_Entity::PostUpdate() {
	BROFILER_CATEGORY("Player PostUpdate", Profiler::Color::DarkGoldenRod);
	Onplat = false;

	return true;

}

void j1Player_Entity::Blit() {
	SDL_Rect r = animation->GetCurrentFrame();

	App->render->Blit(player_tex, position.x, position.y, &r, 1);
}

bool j1Player_Entity::CleanUp() {
	BROFILER_CATEGORY("Player CleanUp", Profiler::Color::DarkGray);
	// App->tex->UnLoad(tex);
	// Player_Animation = &None;
	// tex = nullptr;
	/*Player_Animation = nullptr;*/
	collider->to_delete = true;
	active = false;
	

	/*App->audio->UnloadFx(1);
	App->audio->UnloadFx(2);           // CLEAN FXs
	App->audio->UnloadFx(3);
	App->audio->UnloadFx(4);*/

	return true;

}




bool j1Player_Entity::Load(pugi::xml_node& node) {
	bool ret = true;



	position.x = node.child("Player_Entity").attribute("x").as_int(0);
    position.y = node.child("Player_Entity").attribute("y").as_int(0);
	Vel.x = node.child("Player_Entity").attribute("xv").as_int(0);
	Vel.y = node.child("Player_Entity").attribute("yv").as_int(0);
	
	Jumping = node.child("Player_Entity").attribute("Jumping").as_bool(false);
	Onplat = node.child("Player_Entity").attribute("Onplat").as_bool(false);
	Cont = node.child("Player_Entity").attribute("Cont").as_float(0);
	Cont_X = node.child("Player_Entity").attribute("Cont_X").as_float(0);
	God_Mode = node.child("Player_Entity").attribute("God_Mode").as_bool(false);
	p2SString Player(node.child("Player_Entity").attribute("PType").as_string(""));

	type = Type::PLAYER;
	
	
	
	
	
	p2SString Collider(node.child("State").attribute("collider_type").as_string(""));
	
	if (Collider == "collider_player")
	{
		this->collider->type = COLLIDER_TYPE::COLLIDER_PLAYER;
	}
	else if (Collider == "collider_god")
	{
		this->collider->type = COLLIDER_TYPE::COLLIDER_GOD;
	}
	
	return ret;
}

bool j1Player_Entity::Save(pugi::xml_node& node) const
{
	bool ret = true;

	pugi::xml_node posi = node.append_child("Player_Entity");

	posi.append_attribute("x") = position.x;
	posi.append_attribute("y") = position.y;

	posi.append_attribute("xv") = Vel.x;
	posi.append_attribute("yv") = Vel.y;

	posi.append_attribute("Cont") = Cont;
	posi.append_attribute("Cont_X") = Cont_X;

	posi.append_attribute("Jumping") = Jumping;
	posi.append_attribute("Onplat") = Onplat;
	posi.append_attribute("God_Mode") = God_Mode;
	p2SString PlayerType;
	PlayerType.create("PLAYER");
	posi.append_attribute("PType") = PlayerType.GetString();


	pugi::xml_node coll = node.append_child("State");

	p2SString Collider;
	switch (collider->type)
	{
	case COLLIDER_TYPE::COLLIDER_PLAYER:
		Collider.create("collider_player");
		break;
	case COLLIDER_TYPE::COLLIDER_GOD:
		Collider.create("collider_god");
		break;
	}

	coll.append_attribute("collider_type") = Collider.GetString();


	

	return ret;
}
