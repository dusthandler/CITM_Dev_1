#include "j1Player.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Collision.h"
#include "SDL/include/SDL_timer.h"
#include <math.h>


j1Player::j1Player() : j1Module() 
{
	Player_Animation = &Idle;
	Idle.PushBack({ 55, 2, 35, 45 });        // do this in tiled 
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
	Set_Player_Info(); 
	
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

	// Get_Player_State(); 
     
	

	return true; 
}

void j1Player::Set_Player_Info() {

	Player_Texture = App->tex->Load("Graphics/Ninja/Ninja.png");
	Position.x = 0;                                                             // we need to load this from tiled 
	Position.y = 300;
	Player_Collider = App->collision->AddCollider({ (int)Position.x, (int)Position.y, 35, 45 }, COLLIDER_PLAYER, this);
}

void j1Player::OnCollision(Collider* c1, Collider* c2) {

	/*if (c2->type == COLLIDER_TYPE::COLLIDER_WALL)
	{
	 
	}*/
}

void j1Player::Move() {

	uint speed = 20;
	uint Impulse = 2;
	int ground = 300;     // WE HAVE TO CHANGE THIS WITH FLOOR LEVEL 


	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		Is_Flying = true;
		Speed.y = Impulse; 
		Position.y -= Speed.y;           // Initial impulse
	}

	if (Is_Flying == true) {

		if (Position.y <= ground) {    
			Speed.y = Impulse + Flying_Speed_Decrease + Gravity;     	// WE HAVE TO CHANGE THIS WITH FLOOR LEVEL 
			Position.y -= Speed.y;  
			LOG("Speed y        %f", Speed.y); 
		}

		else {
         	Position.y = ground;
			Speed.y = 0; 
			Is_Flying = false;
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

PlayerState j1Player::Get_Player_State() {

	
	if (Is_Flying == true) {   // IN THE AIR

		if (Speed.y > 0) {                 // GOING UP              We should consider when Speed.y == 0

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
	Player_Animation = &Idle;
	SDL_Rect Rect = Player_Animation->GetCurrentFrame();
	App->render->Blit(Player_Texture, Position.x, Position.y, &Rect, 0);
	Player_Collider->SetPos(Position.x, Position.y);

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
