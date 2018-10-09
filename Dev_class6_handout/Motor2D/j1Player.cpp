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
	Move(); 
	
	return true; 
}


void j1Player::Move() {

	uint speed = 20;
	uint Impulse = 2;
	int ground = 300;     // WE HAVE TO CHANGE THIS WITH FLOOR LEVEL 

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		Is_Jumping = true;
		Position.y -= Impulse;           // Initial impulse
	}

	if (Is_Jumping == true) {

		if (Position.y <= ground) {                                              // WE HAVE TO CHANGE THIS WITH FLOOR LEVEL 
				Position.y -= Impulse + Flying_Speed_Decrease + Gravity;
		}

		else {
         	Position.y = ground;
			Is_Jumping = false;
			Flying_Speed_Decrease = 0.5f;
		}
		
			Flying_Speed_Decrease -= 1.5f;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		Position.x -= speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		Position.x += speed;
	}





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
