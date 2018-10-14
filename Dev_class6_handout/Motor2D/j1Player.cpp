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


j1Player::j1Player() : j1Module()

{
	name.create("player"); // aun no se poruqe


}

// Destructor
j1Player::~j1Player()
{

}
// Called before render is available
bool j1Player::Awake(pugi::xml_node&)
{
	Player_Animation = &Idle;
	Idle.PushBack({ 55, 2, PLAYER_WIDTH, PLAYER_HEIGHT });       // do this in tiled 
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
	Player_Collider->SetPos(Pos.x, Pos.y + 10);
	return true;

}



void j1Player::Set_Player_Info() {

	Alive = true; 
	Player_Texture = App->tex->Load("Maps/Ninja/Ninja.png");             
	Reset_Fx_2 = true;  // so that it plays death once 

	App->audio->LoadFx("Sound/Fx/jump.wav");          // FXs
	App->audio->LoadFx("Sound/Fx/death.wav");
	App->audio->LoadFx("Sound/Fx/landing.wav");

	Pos.x = 0;                                                             // we need to load this from tiled 
	Pos.y = 0;
	Player_Collider = App->collision->AddCollider({ (int)Pos.x, (int)Pos.y, 35, 45 }, COLLIDER_PLAYER, this);
}

void j1Player::OnCollision(Collider* c1, Collider* c2) {

	
		if ((c1->type == COLLIDER_PLAYER || c1->type == COLLIDER_GOD) && c2->type == COLLIDER_WALL) {

			LOG("WE ARE HAVING A COLISION"); 

			if (Reset_Fx_3) {
				App->audio->PlayFx(3, 0);
				Reset_Fx_3 = false;
		   }

			if (c1->rect.y <= c2->rect.y) {     // player on top (Landing) 

				
				if (Vel.y >= 0) {
					Onplat = true;
					Jumping = false;
					Vel.y = 0;
					Pos.y = c2->rect.y - PLAYER_HEIGHT;

				}
				
			}

			else if (c1->rect.x + PLAYER_WIDTH >= c2->rect.x || c1->rect.x >= (c2->rect.x + c2->rect.w)) {         // tries to go right

				if (c1->type == COLLIDER_PLAYER) {   // god player does not hace laterlal colliders

					if (Vel.x > 0) {

						Pos.x = c2->rect.x - PLAYER_WIDTH;
					}

					else if (Vel.x < 0) {
						Pos.x = c2->rect.x + c2->rect.w;
					}
				}
					
			 }
			
            
		}

	


		else if (c2->type == COLLIDER_PLAYER && c1->type == COLLIDER_WALL) {

			if (c2->rect.y <= c1->rect.y) {     // player on top (Landing) 

				if (Vel.y >= 0) {
					Onplat = true;
					Jumping = false;
					Vel.y = 0;
					Pos.y = c1->rect.y - PLAYER_HEIGHT;

				}

			}

			else if (c2->rect.x + PLAYER_WIDTH >= c1->rect.x || c2->rect.x >= (c1->rect.x + c1->rect.w)) {         // tries to go right

				if (Vel.x > 0) {
					Pos.x = c1->rect.x - PLAYER_WIDTH;
				}
				else if (Vel.x < 0) {
					Pos.x = c1->rect.x + c2->rect.w;
				}


			}


		}




				 if (c2->type == COLLIDER_DEATH) {

					 if (!God_Mode) {

						 Alive = false;

						 if (Reset_Fx_2) {

							 App->audio->PlayFx(2, 0);
							 Reset_Fx_2 = false;
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



				/*LOG("POSITION COLLIDER 1 x: %i  y: %i   COLLIDER 2 x: %i  y: %i", c1->rect.x, c1->rect.y, c2->rect.x, c2->rect.y);*/


			
		
	
}


bool j1Player::Update(float dt)
{

	Get_Player_State();
	Debug_Keys(); 
	
	
		if (God_Mode) {
			Player_Collider->type = COLLIDER_TYPE::COLLIDER_GOD; 
		}

			Movex();
			Movey();
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
			Pos.x += Vel.x;
			Pos.y += Vel.y + Acc.y;
	

		// Draw --------------------------------------------------------------------------------------------------------------------------
		

	//Draw -------------------------------------------------------------------------------------------------------------------------------

	return true;
}

void j1Player::Debug_Keys() {

	Switch_Level_Logic(); 
	
	
	
	
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN) {   // CHANGE TO F10	
		God_Mode = true; 
	}
	
	// F9 located in collision module 
}

void j1Player::Switch_Level_Logic() {

	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN) {   
		Disable();
		App->scene->MapSwap(1);
		
		/*App->player->CleanUp();*/// change this with the win collider
	}
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN) {
		Disable();
		App->scene->MapSwap(0);
		
		/*App->player->CleanUp();*/// change this with the win collider
	}

	/*if (!Alive && !Arrived_Lvl2) {             // Dies in level 1
		App->player->Disable();
		App->fade->FadeToBlack(App->scene, App->scene, 2);  
	}*/

	

	/*else if (!Alive && Arrived_Lvl2) {
		App->player->Disable();
		App->fade->FadeToBlack(App->scene2, App->scene2, 2);           // Dies in level 2
	}*/

	LOG("ALIVE %i ARRIVED LVL 2    %i", Alive, Arrived_Lvl2); 

}



void j1Player::Movex() {


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

		if (Pos.x <= 0) {
			Vel.x = 0;
		}
		else {

			if (Vel.x == 0) {
				Cont_X = 0;
			}

		
				if (Vel.x > -MAX_SPEED_X) {

					Vel.x = -10 + Cont_X;
					Cont_X -= 1.0f;
				}

				else {
					Vel.x = -MAX_SPEED_X;
				}

			
		}
	}

	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {

		    if (Vel.x == 0) {
			Cont_X = 0; 
			}

				if (Vel.x < MAX_SPEED_X) {

					Vel.x = 10 + Cont_X;
					Cont_X += 1.0f;
				}

				else {
					Vel.x = MAX_SPEED_X;
				}
		
	}
	else
		Vel.x = 0;


}

void j1Player::Movey() {
	// Change variables to can get the map
	if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)) {
		
		Jump_Count++; 

		
		if (God_Mode) {   // god player can jump infinitely
			Vel.y = -26;
			Cont = 1.3;
			Jumping = true;
			App->audio->PlayFx(1, 0);
		}

		else if(Jump_Count < 2) {
			Vel.y = -16;
			Cont = 1.3;
			Jumping = true;
			App->audio->PlayFx(1, 0);
		}
	}

	else if (Jumping) {
		if (Vel.y <= 4) {
			Vel.y += 1.3; 
		}
		else {
			Cont += 0.1;
			Vel.y += Cont;
		}
	}
	else if (!Onplat && !Jumping) {
		
		Vel.y += 1.1;
	}
	
	
}

PlayerState j1Player::Get_Player_State() {

	if (Alive) {   

		if (!Level_Win) {   // IN THE LEVEL 

			if (!Onplat) {   // IN THE AIR

				if (Vel.y < 0) {                 // GOING UP 

					if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
						State = JUMPING_LEFT;
						Player_Animation = &Jumping_Left;
					}

					else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
						State = JUMPING_RIGHT;
						Player_Animation = &Jumping_Right;
					}
					else {
						State = JUMPING_UP;
						Player_Animation = &Jumping_Up;
					}
				}

				else {                            // GOING DOWN

					if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
						State = FALLING_LEFT;
						Player_Animation = &Falling_Left;
					}

					else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
						State = FALLING_RIGHT;
						Player_Animation = &Falling_Right;
					}
					else {
						State = FALLING_DOWN;
						Player_Animation = &Falling;
					}
				}

			}

			else {    // IN THE GROUND


				if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

					State = WALKING_LEFT;

					if (Vel.x > -MAX_SPEED_X) {

						Player_Animation = &Walking_Left2;
					}

					else {
						Player_Animation = &Walking_Left;
					}
				}


				else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {

					State = WALKING_RIGHT;

					if (Vel.x < MAX_SPEED_X) {

						Player_Animation = &Walking_Right2;
					}
					else {
						Player_Animation = &Walking_Right;
					}
				}


				else {

					State = IDLE;
					Player_Animation = &Idle;
				}
			}

		}

		else {

			State = WIN; 
		}

	}

	else {

		State = DIED;
		Player_Animation = &Death; 
	}


	return State;
}




bool j1Player::Draw()
{
	
	
	SDL_Rect Rect = Player_Animation->GetCurrentFrame();
	App->render->Blit(Player_Texture, Pos.x, Pos.y, &Rect, 1);

	return true;
}

bool j1Player::PostUpdate()
{
	
	Onplat = false;
	
	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
	App->tex->UnLoad(Player_Texture);
	// Player_Animation = &None;
	Player_Texture = nullptr;
	/*Player_Animation = nullptr;*/
	if (Player_Collider != nullptr)
	{
		Player_Collider->to_delete = true;
		
	}

	App->audio->UnloadFx(1); 
	App->audio->UnloadFx(2);           // CLEAN FXs
	App->audio->UnloadFx(3);


	return true;
}


