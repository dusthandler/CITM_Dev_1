#include "j1Player.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"

j1Player::j1Player() : j1Module() 
{
	Player_Animation = &idle;
	idle.PushBack({ 111, 32, 193, 320 });

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
	Player_Texture = App->tex->Load("Game/Textures/ninja.png"); 
	Position.x = 0; 
	Position.y = 0; 


	return true; 
}

// Called each loop iteration
bool j1Player::PreUpdate() 
{

	return true; 

}

bool j1Player::Update() 
{
	Player_Animation = &idle;
	SDL_Rect Rect= Player_Animation->GetCurrentFrame();
    App->render->Blit(Player_Texture, Position.x , Position.y, &Rect);

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
	Player_Animation = nullptr; 

	return true; 
}
