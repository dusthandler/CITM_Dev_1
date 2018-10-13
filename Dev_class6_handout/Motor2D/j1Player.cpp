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
	Falling.PushBack({ 149, 1, PLAYER_WIDTH, PLAYER_HEIGHT });
	Walking_Right.PushBack({ 3, 96, 44, 45 });
	Walking_Left.PushBack({ 341, 95, 44, 45 });
	Walking_Right2.PushBack({ 55, 96, 42, 45 });
	Walking_Left2.PushBack({ 298, 95, 42, 45 });

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
	Player_Texture = App->tex->Load("Graphics/Ninja.png");
	Pos.x = 0;                                                             // we need to load this from tiled 
	Pos.y = 0;
	Player_Collider = App->collision->AddCollider({ (int)Pos.x, (int)Pos.y, 35, 45 }, COLLIDER_PLAYER, this);
}

void j1Player::OnCollision(Collider* c1, Collider* c2) {

	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_WALL) {

		if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_WALL) {

			if (c1->rect.y <= c2->rect.y) {     // player on top (Landing) 

				if (Vel.y >= 0) {
					Onplat = true;
					Jumping = false;
					Vel.y = 0;
					Pos.y = c2->rect.y - PLAYER_HEIGHT;
				}
				
			}

			else {
				if (c1->rect.x + PLAYER_WIDTH >= c2->rect.x || c1->rect.x >= (c2->rect.x + c2->rect.w)) {         // tries to go right

					if (Vel.x > 0) {
						Pos.x = c2->rect.x - PLAYER_WIDTH;
					}
					else if (Vel.x < 0) {
						Pos.x = c2->rect.x + c2->rect.w;
					}

				}
				//else if (c1->rect.x >= (c2->rect.x + c2->rect.w)) {    // tries to go left

				//	
				//		Pos.x = c2->rect.x + c2->rect.w;
				//	
				//}









		/*else if (c1->type == COLLIDER_WALL && c2->type == COLLIDER_PLAYER) {

			if (c2->rect.x + PLAYER_WIDTH >= c1->rect.x && c2->rect.y <= c1->rect.y) {            // player goes right and on top (Landing)

				Onplat = true;
				Jumping = false;
				Vel.y = 0;
				Pos.y = c1->rect.y - PLAYER_HEIGHT;
			}
			else if (c2->rect.x + PLAYER_WIDTH >= c1->rect.x && c2->rect.x >= (c1->rect.x + c1->rect.w)) {

				if(Vel.x > 0){
					Pos.x = c1->rect.x - PLAYER_WIDTH;
				}
				else {
					Pos.x = c1->rect.x + c1->rect.w;
				}
				//Inside_Collider = true;
				LOG("PLAYER IS INSIDE COLLIDER WHOAAAAA");
			}
			/*else if (c2->rect.x >= (c1->rect.x + c1->rect.w)) {
				Pos.x = c1->rect.x + c1->rect.w;

			}

		}*/


				else if (c1->type == COLLIDER_DEATH || c2->type == COLLIDER_DEATH) {
					Alive = false;
				}



				LOG("POSITION COLLIDER 1 x: %i  y: %i   COLLIDER 2 x: %i  y: %i", c1->rect.x, c1->rect.y, c2->rect.x, c2->rect.y);


			}
		}
	}
}


bool j1Player::Update(float dt)
{

	Get_Player_State();

	

	if (!Alive) {
		App->player->Disable(); 
		App->fade->FadeToBlack(App->scene, App->scene, 2);  // get time from Tiled
	}

	else {

		Movex();
		Movey();
		Acc.y = 13;
		if (!Onplat) Acc.y = 4;
		else Acc.y = 0;

		Pos.x += Vel.x;
		Pos.y += Vel.y + Acc.y;


		// Draw --------------------------------------------------------------------------------------------------------------------------
		
		
	}

	//Draw -------------------------------------------------------------------------------------------------------------------------------

	return true;
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
	if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) && !Jumping) {
		Vel.y = -26;
		Cont = 1.3;
		Jumping = true;
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

	uint last = 0; 
	uint now = SDL_GetTicks(); 
	uint Switch_Time = 500; 

	if (!Onplat) {   // IN THE AIR

		if (Vel.y < 0) {                 // GOING UP 

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
				State = JUMPING_LEFT;
				/*if()

				else if (now > last + Switch_Time) {

				}
				else if(now > last + Switch_Time) {

				}*/
			}

			else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				State = JUMPING_RIGHT;
			}
			else {
				State = JUMPING_UP;
			}
		}

		else {                            // GOING DOWN

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
				State = FALLING_LEFT;
			}

			else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				State = FALLING_RIGHT;
			}
			else {
				State = FALLING_DOWN;
				Player_Animation = &Falling;
			}
		}

		last = now; 
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
	Player_Animation = &None;
	Player_Collider->to_delete = true;   // CLEAN COLLIDERS


	return true;
}


