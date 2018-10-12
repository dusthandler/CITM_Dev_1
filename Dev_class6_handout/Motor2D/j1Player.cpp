#include "j1Player.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "SDL/include/SDL_timer.h"
#include <math.h>
#include "j1FadeBlack.h"


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
	Idle.PushBack({ 55, 2, 35, 45 });        // do this in tiled 
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

	return true;

}



void j1Player::Set_Player_Info() {

	Alive = true; 
	Player_Texture = App->tex->Load("Graphics/Ninja.png");
	Pos.x = 0;                                                             // we need to load this from tiled 
	Pos.y = 300;
	Player_Collider = App->collision->AddCollider({ (int)Pos.x, (int)Pos.y + 45, 35, 45 }, COLLIDER_PLAYER, this);
}

void j1Player::OnCollision(Collider* c1, Collider* c2) {
	
	if (c1->type == COLLIDER_WALL || c2->type == COLLIDER_WALL) {

		if (c1->CheckCollision(c2->rect)) {
			Onplat = true;
			Jumping = false;
			Vel.y = 0;
		}
		
	}
	
	else if (c1->type == COLLIDER_DEATH || c2->type == COLLIDER_DEATH) {
		Alive = false; 
	}

}

bool j1Player::Update(float dt)
{


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

		Player_Collider->SetPos(Pos.x, Pos.y);
		Player_Animation = &Idle;
		SDL_Rect Rect = Player_Animation->GetCurrentFrame();
		App->render->Blit(Player_Texture, Pos.x, Pos.y, &Rect, 1);
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
			Vel.x = -10;
		}

	}

	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		Vel.x = 10;
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


	if (!Onplat) {   // IN THE AIR

		if (Vel.y < 0) {                 // GOING UP              

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
				State = JUMPING_LEFT;
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
			}
		}
	}

	else {    // IN THE GROUND

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			State = WALKING_LEFT;
		}

		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			State = WALKING_RIGHT;
		}

		else {
			State = IDLE;
		}
	}


	return State;
}

bool j1Player::Draw()
{
	

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


