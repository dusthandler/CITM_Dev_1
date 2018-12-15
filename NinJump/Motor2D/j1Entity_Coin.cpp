#include "j1Entity_Coin.h"
#include "j1Entity_Manager.h"
#include "j1Collision.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Gui.h"
#include "p2Log.h"

j1Entity_Coin::j1Entity_Coin(iPoint position, Type type,int id) : j1Entity(position, type,id) {

	collider = App->collision->AddCollider({ position.x, position.y, coin_size, coin_size }, COLLIDER_COIN, this);
	animation = &Idle;
	Idle.PushBack({ 0, 0, coin_size, coin_size });
	Idle.PushBack({ 0, 22, coin_size, coin_size });
	Idle.PushBack({ 0, 44, coin_size, coin_size+1 });
	Idle.PushBack({ 0, 66, coin_size, coin_size+1});
	Idle.loop = true; 
	Idle.speed = 0.14f; 

	this->my_id = id;
	this->position = position;
}

void j1Entity_Coin::OnCollision(Collider* c1, Collider* c2) {

	if (c2->type == COLLIDER_PLAYER || c2->type == COLLIDER_GOD || c2->type == COLLIDER_SHOT) {
		App->audio->PlayFx(7, 0); 
		App->gui->coins_collected++; 
		CleanUp(); 
	}
    
}

bool j1Entity_Coin::CleanUp() {
	collider->to_delete = true;
	active = false; 


	
	 DestroyEntity(this); 

	return true;
}

