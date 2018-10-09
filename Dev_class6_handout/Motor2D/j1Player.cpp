#include "j1Player.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Log.h"
#include "SDL/include/SDL_timer.h"
#include <math.h>


j1Player::j1Player() : j1Module() 
{
	Player_Animation = &Idle;
	Idle.PushBack({ 11, 9, 32, 53 });
	Gravity = 11; 
}

// Destructor
j1Player::~j1Player() 
{

}
// Called before render is available
bool j1Player::Awake(pugi::xml_node&) 
{
	
	return true; 
}

// Called before the first frame
bool j1Player::Start() 
{
	Player_Texture = App->tex->Load("textures/ninja.png"); 
	Position.x = 0; 
	Position.y = 300; 


	return true; 
}

// Called each loop iteration
bool j1Player::PreUpdate() 
{

	return true; 

}

bool j1Player::Update(float dt)
{
	
	uint speed = 20; 
	uint JumpSpeed = 2; 
	

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		Is_Jumping = true; 
		Position.y -= JumpSpeed;
	}
	if (Is_Jumping == true) {
		Jump_Calculator(JumpSpeed, speed, Flying_Speed_Decrease);
		Flying_Speed_Decrease -= 1; 
		LOG("     speed decrease BEFORE:  %f      ", Flying_Speed_Decrease);
	}
	
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			Position.x -= speed; 
			App->render->camera.x += speed; 
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			Position.x += speed;
			App->render->camera.x -= speed; 
	}
	

	return true; 
}

void j1Player::Jump_Calculator(uint Jump_Speed, uint Speed_X, float Flying_Speed_Decrease) {

	int ground = 300;     // WE HAVE TO CHANGE THIS WITH FLOOR LEVEL 
/*
	uint First_Time = 0; 
	uint Now = SDL_GetTicks();
	float Delta_Time = (Now - First_Time)/1000;   // Get Ticks is in miliseconds
	fPoint Flying_Speed; 
	float Flying_Speed_Module; 

	Flying_Speed.x = speed;                                        
	Flying_Speed.y = speed - Gravity * Delta_Time;                                       // Physics flying object speed 
	Flying_Speed_Module = sqrt(pow(Flying_Speed.x, 2) + pow(Flying_Speed.y, 2)); 

	int Angle = asin(Flying_Speed.y / Flying_Speed_Module);

	Position.x += Flying_Speed.x * cos(Angle) * Delta_Time;                               // Physics flying object position

	if (Is_Jumping == true) {
		if (Position.y <= ground) {
			Position.y += Flying_Speed.y * Delta_Time * sin(Angle) - (Gravity * (pow(Delta_Time, 2))) / 2;
		}
		else {
			Position.y = ground; 
			Is_Jumping = false; 
		}
	}
	Now = First_Time; */
	
	if (Is_Jumping == true) {
		if (Position.y <= ground) {
			Position.y -= Jump_Speed + Flying_Speed_Decrease + Gravity;
			LOG("     speed decrease:  %f      ", Flying_Speed_Decrease); 
		}
		else {
			Position.y = ground;
			Is_Jumping = false;
		}
	}
	Position.x += Speed_X; 


	//LOG("SPEED X---> %f   SPEED Y ---> %f", Flying_Speed.x, Flying_Speed.y); 

}


bool j1Player::Draw()
{
	Player_Animation = &Idle;
	SDL_Rect Rect = Player_Animation->GetCurrentFrame();
	App->render->Blit(Player_Texture, Position.x, Position.y, &Rect, 0);

	return true; 
}

bool j1Player::PostUpdate() 
{
	return true; 
}

// Called before quitting
bool j1Player::CleanUp() 
{
	App->tex->UnLoad(Player_Texture); 
	Player_Animation = &None; 

	return true; 
}
