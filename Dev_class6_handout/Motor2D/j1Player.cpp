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

	Player_Texture = App->tex->Load("Graphics/Ninja.png");
	pos.x = 0;                                                             // we need to load this from tiled 
	pos.y = 300;
	Player_Collider = App->collision->AddCollider({ (int)pos.x, (int)pos.y + 45, 35, 45 }, COLLIDER_PLAYER, this);
}

void j1Player::OnCollision(Collider* c1, Collider* c2) {
	
	if (c1->type == COLLIDER_WALL || c2->type == COLLIDER_WALL) {

		if (c1->CheckCollision(c2->rect)) {
			onplat = true;
			jumping = false;
			vel.y = 0;
		}
		
	}
	

}

bool j1Player::Update(float dt)
{
	Movex();
	Movey();
	acc.y = 13;
	if (!onplat) acc.y = 13;
	else acc.y = 0;

	pos.x = pos.x + vel.x;
	pos.y = pos.y + vel.y + acc.y;


	// Draw --------------------------------------------------------------------------------------------------------------------------
	Player_Collider->SetPos(pos.x, pos.y);
	Player_Animation = &Idle;
	SDL_Rect Rect = Player_Animation->GetCurrentFrame();
	App->render->Blit(Player_Texture, pos.x, pos.y, &Rect, 1);
	//Draw -------------------------------------------------------------------------------------------------------------------------------
	return true;
}


void j1Player::Movex() {

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		vel.x = -10;
		// Acc? future!!
	}

	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		vel.x = 10;
	}
	else
		vel.x = 0;
}

void j1Player::Movey() {
	// Change variables to can get the map
	if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) && !jumping) {
		vel.y = -26;
		cont = 0.8;
		jumping = true;
	} 
	else if (jumping) {
		if (vel.y <= 4) {
			vel.y = vel.y + 0.8;
		}
		else {
			cont += 0.1;
			vel.y = vel.y + cont;
		}
	}
}

PlayerState j1Player::Get_Player_State() {


	

	return state;
}

bool j1Player::Draw()
{
	

	return true;
}

bool j1Player::PostUpdate()
{
	onplat = false;
	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
	App->tex->UnLoad(Player_Texture);
	Player_Animation = &none;

	return true;
}


