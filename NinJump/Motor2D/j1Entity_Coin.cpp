#include "j1Entity_Coin.h"
#include "j1Entity_Manager.h"
#include "j1Collision.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "p2Log.h"

j1Entity_Coin::j1Entity_Coin(iPoint position, Type type) : j1Entity(position, type) {

	collider = App->collision->AddCollider({ position.x, position.y, coin_size, coin_size }, COLLIDER_COIN, this);
	tex = App->tex->Load("Maps/Objects/coins.png");
	animation = &Idle;
	Idle.PushBack({ 0, 0, coin_size, coin_size });
	Idle.PushBack({ 0, 22, coin_size, coin_size });
	Idle.PushBack({ 0, 44, coin_size, coin_size+1 });
	Idle.PushBack({ 0, 66, coin_size, coin_size+1});
	Idle.loop = true; 
	Idle.speed = 0.14f; 

	
	this->position = position;
}

void j1Entity_Coin::OnCollision(Collider* c1, Collider* c2) {

	if (c2->type == COLLIDER_PLAYER || c2->type == COLLIDER_GOD) {
		App->audio->PlayFx(5, 0); 
		Coin_Score++;
		CleanUp(); 
	}

}

bool j1Entity_Coin::CleanUp() {
	collider->to_delete = true;
	App->tex->UnLoad(tex);

	return true;
}

