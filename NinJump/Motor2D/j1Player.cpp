#include "j1Player.h"
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
#include <math.h>
#include "j1FadeBlack.h"
#include "j1Window.h"


j1Player::j1Player(iPoint pos, Type type) : j1Entity(pos, type)

{
	name.create("player");                                         // generate the initial player info
	tex = App->tex->Load("Maps/Ninja/Ninja.png");
	collider = App->collision->AddCollider({ pos.x, pos.y, 35, 45 }, COLLIDER_PLAYER, this);
	Idle.PushBack({ 55, 2, PLAYER_WIDTH, PLAYER_HEIGHT });
	animation = &Idle;

	position = pos; 
}

// Destructor
j1Player::~j1Player()
{

}
// Called before render is available
bool j1Player::Awake(pugi::xml_node&)
{
	       // do this in tiled 
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


	return true;
}

// Called before the first frame
bool j1Player::Start()
{
	Set_Player_Info();

	return true;
}

// Called each loop iteration
bool j1Player::PreUpdate()
{
	

	collider->SetPos(Pos.x, Pos.y);
	return true;

}



void j1Player::Set_Player_Info() {

	Alive = true;
	
	Reset_Fx_2 = true;  // so that it plays death once 

	App->audio->LoadFx("Sound/Fx/jump.wav");          // FXs
	App->audio->LoadFx("Sound/Fx/death.wav");
	App->audio->LoadFx("Sound/Fx/landing.wav");
	App->audio->LoadFx("Sound/Fx/gravity_reverse.wav");
	
	/*pugi::xml_node InitPos = App->map->map_file.child("map");

	                                                          // we need to load this from tiled 
	Pos.x = InitPos.child("tileset").child("terraintypes").child("terrain").child("properties").child("property").attribute("value").as_float();
	Pos.y = InitPos.child("tileset").child("terraintypes").child("terrain").child("properties").child("property").next_sibling("property").attribute("value").as_float();*/
	
	


	
}

void j1Player::OnCollision(Collider* c1, Collider* c2) {

	Respawning = false;


	//if ((c1->type == COLLIDER_PLAYER || c1->type == COLLIDER_GOD) && c2->type == COLLIDER_WALL) {

	//	LOG("WE ARE HAVING A COLISION");

		

	if ((c1->type == COLLIDER_PLAYER || c1->type == COLLIDER_GOD) && c2->type == COLLIDER_WALL) {

		if (Reset_Fx_3 && Vel.y > 0) {
			App->audio->PlayFx(3, 0);
			Reset_Fx_3 = false;
		}

		if (c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y + c1->rect.h <= c2->rect.y + 20) {
			Vel.y = 0;
			Pos.y = c2->rect.y - PLAYER_HEIGHT;
			
			if (!gravity_reverse) {
				Onplat = true;             // without gravity, it lands
			}
			else {
				Onplat = false;            // with gravity, it falls
			}
			Jumping = false;
		}
		if (c1->rect.y >= c2->rect.h + c2->rect.y - 20 && c1->rect.y <= c2->rect.h + c2->rect.y) {
			Pos.y = c2->rect.y + c2->rect.h;
			if (gravity_reverse) {
				Onplat = true;                // with gravity, roof is now floor
			}
		}
		if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.h + c1->rect.y >= c2->rect.y + 10) {

			if ((c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w <= c2->rect.x + 20)) {
				if (Vel.x > 0) {
					Pos.x = c2->rect.x - PLAYER_WIDTH;
					Vel.x = 0;
				}
			}
			else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x > c2->rect.w + c2->rect.x - 20){
				if (Vel.x < 0) {
					Pos.x = c2->rect.x + c2->rect.w;
					Vel.x = 0;
				}
			}
		
		}
		
		
	}




	if (c2->type == COLLIDER_DEATH) {

		if (!God_Mode) {

			Alive = false;

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
					Pos.y = c2->rect.y - PLAYER_HEIGHT;

				}

			}

		}
	}




	//LOG("POSITION COLLIDER 1 x: %i  y: %i   COLLIDER 2 x: %i  y: %i", c1->rect.x, c1->rect.y, c2->rect.x, c2->rect.y);
	if (c2->type == COLLIDER_WIN) {
		Level_Win = true;
	}

}


bool j1Player::Update(float dt)
{
	// SDL_Delay(dt);
	
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

void j1Player::Debug_Keys() {

	Switch_Level_Logic();




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

void j1Player::Switch_Level_Logic() {

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN || Level_Win) {

		
		Level_Win = false;
		/*Disable();*/
		/*App->player->Disable(); */          // disable player before swapping maps
		App->scene->MapSwap(1);
		mapLo = 1;

	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN || (!Alive)) {
		/*Disable();*/
		/*App->player->Disable();    */       // disable player before swapping maps
		App->scene->MapSwap(0);
		mapLo = 0;
		
	}


}

void j1Player::Solve_Move(float dt) {

	if (Pos.y < -PLAYER_HEIGHT && gravity_reverse) {      // sky limit
		Alive = false; 
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



	/*float Prop = 0;
	if (Vel.x + Vel.y > MAX_TOTAL_SPEED) {      // speed nerf
		Prop = (Vel.x + Vel.y) / MAX_TOTAL_SPEED;
		Vel.x /= Prop;
		Vel.y /= Prop;S
		LOG("SPEED NERF IS ------------------- %f", Prop);
	}
	else if (Vel.x + Vel.y < -MAX_TOTAL_SPEED) {
		Prop = (Vel.x + Vel.y) / -MAX_TOTAL_SPEED;
		Vel.x /= Prop;
		Vel.y /= Prop;
		LOG("SPEED NERF IS ------------------- %f", Prop);
	}
	*/

	Pos.x += Vel.x;
	if (!gravity_reverse) {
		Pos.y += (Vel.y + Acc.y)*dt;
	}
	else {
		Pos.y -= (Vel.y + Acc.y)*dt;
	}

}



void j1Player::Movex(float dt) {

	if (!Respawning) {
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

			if (Pos.x <= 0) {
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

void j1Player::Movey(float dt) {
	// Change variables to can get the map



	if (!Respawning) {

		if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)) {

			Jump_Count++;


			if (God_Mode) {   // god player can jump infinitely
				Vel.y = -800;
				Cont = 20;
				Jumping = true;
				App->audio->PlayFx(1, 0);
			}								//Danger:
											//We should *dt every counter or we wont get the same result in diferent fps.
											//We should *dt every counter or we wont get the same result in diferent fps.
											//We should *dt every counter or we wont get the same result in diferent fps.
											//We should *dt every counter or we wont get the same result in diferent fps.
			else if (Jump_Count < 2) {
				Vel.y = -400;
				
				Jumping = true;
				App->audio->PlayFx(1, 0);
			}
		}

		else if (Jumping) {
			if (Vel.y <= -70) {
				Vel.y += 150*dt;
			}
			else {
				Cont += 100*dt;
				Vel.y += Cont*dt;
			}
		}
		else if (!Onplat && !Jumping) {

			Vel.y += 150*dt;
		}
	}

	else {
		Vel.y += 150*dt;
	}

	if (Vel.y > MAX_SPEED_Y) {
		Vel.y = MAX_SPEED_Y;
		
	}
	else if (Vel.y < -MAX_SPEED_Y) {
		Vel.y = -MAX_SPEED_Y;
	}

	
}

PlayerState j1Player::Get_Player_State() {

	if (Alive) {

		if (!Level_Win) {   // IN THE LEVEL 

			if (!Onplat) {   // IN THE AIR

				if (Vel.y < 0) {                 // GOING UP 

					if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
						State = JUMPING_LEFT;
						animation = &Jumping_Left;
					}

					else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
						State = JUMPING_RIGHT;
						animation = &Jumping_Right;
					}
					else {
						State = JUMPING_UP;
						animation = &Jumping_Up;
					}
				}

				else {                            // GOING DOWN

					if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
						State = FALLING_LEFT;
						animation = &Falling_Left;
					}

					else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
						State = FALLING_RIGHT;
						animation = &Falling_Right;
					}
					else {
						State = FALLING_DOWN;
						animation = &Falling;
					}
				}

			}

			else {    // IN THE GROUND


				if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

					State = WALKING_LEFT;

					if (Vel.x > -MAX_SPEED_X) {

						animation = &Walking_Left2;
					}

					else {
						animation = &Walking_Left;
					}
				}


				else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {

					State = WALKING_RIGHT;

					if (Vel.x < MAX_SPEED_X) {

						animation = &Walking_Right2;
					}
					else {
						animation = &Walking_Right;
					}
				}


				else {

					State = IDLE;
					animation = &Idle;
				}
			}

		}

		else {

			State = WIN;
		}

	}

	else {

		State = DIED;
		animation = &Death;
	}


	return State;
}



/*
bool j1Player::Draw()
{


	SDL_Rect Rect = Player_Animation->GetCurrentFrame();

	

	if (!gravity_reverse) {
		App->render->Blit(Player_Texture, Pos.x, Pos.y, &Rect, 1);
	}
	else {
		App->render->Blit(Player_Texture, Pos.x, Pos.y, &Rect, 1, "player");
	}

	return true;
}*/

bool j1Player::PostUpdate()
{

	Onplat = false;

	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
	/*App->tex->UnLoad(Player_Texture);
	// Player_Animation = &None;
	Player_Texture = nullptr;                           // this is now done in entity manager
	/*Player_Animation = nullptr;
	if (Player_Collider != nullptr)
	{
		Player_Collider->to_delete = true;

	}*/

	App->audio->UnloadFx(1);
	App->audio->UnloadFx(2);           // CLEAN FXs
	App->audio->UnloadFx(3);
	App->audio->UnloadFx(4);

	return true;
}



bool j1Player::Load(pugi::xml_node& node)
{
	bool ret = true;

	
	
	
	mapLo = node.child("position").attribute("Map_Logic").as_int(0);
	App->scene->MapSwap(mapLo);



	Pos.x = node.child("position").attribute("x").as_float(0);
	Pos.y = node.child("position").attribute("y").as_float(0);
	Vel.x = node.child("velocity").attribute("x").as_float(0);
	Vel.y = node.child("velocity").attribute("y").as_float(0);
	Acc.x = node.child("acceleration").attribute("x").as_float(0);
	Acc.y = node.child("acceleration").attribute("y").as_float(0);

	Jumping = node.child("var").attribute("Jumping").as_bool(false);
	Onplat = node.child("var").attribute("Onplat").as_bool(false);
	Alive = node.child("var").attribute("Alive").as_bool(true);
	Cont = node.child("cont").attribute("Cont").as_float(0);
	Cont_X = node.child("cont").attribute("Cont_X").as_float(0);
	God_Mode = node.child("var").attribute("God_Mode").as_bool(false);
	
	

	

	p2SString collider(node.child("state").attribute("collider_type").as_string(""));

	if (collider == "collider_player")
	{
		this->collider->type = COLLIDER_TYPE::COLLIDER_PLAYER;
	}
	else if (collider == "collider_god")
	{
		this->collider->type = COLLIDER_TYPE::COLLIDER_GOD;
	}
	
	return ret;

}

bool j1Player::Save(pugi::xml_node& node) const
{
	bool ret = true;

	pugi::xml_node pos = node.append_child("position");

	pos.append_attribute("x") = Pos.x;
	pos.append_attribute("y") = Pos.y;
	pos.append_attribute("Map_Logic") = mapLo;

	pugi::xml_node vel = node.append_child("velocity");

	vel.append_attribute("x") = Vel.x;
	vel.append_attribute("y") = Vel.y;

	pugi::xml_node cont = node.append_child("Cont");

	cont.append_attribute("Cont") = Cont;
	cont.append_attribute("Cont_X") = Cont_X;

	pugi::xml_node var = node.append_child("Cont");

	var.append_attribute("Jumping") = Jumping;
	var.append_attribute("Onplat") = Onplat;
	var.append_attribute("Alive") = Alive;
	var.append_attribute("God_Mode") = God_Mode;
	

	
	pugi::xml_node coll = node.append_child("state");

	p2SString collider;
	switch (this->collider->type)
	{
	case COLLIDER_TYPE::COLLIDER_PLAYER:
		collider.create("collider_player");
		break;
	case COLLIDER_TYPE::COLLIDER_GOD:
		collider.create("collider_god");
		break;
	}

	coll.append_attribute("collider_type") = collider.GetString();

	return ret;
}
