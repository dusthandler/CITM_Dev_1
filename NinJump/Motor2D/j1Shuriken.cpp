#include "j1Shuriken.h"
#include "j1Entity_Manager.h"
#include "j1Collision.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "p2Log.h"

j1Shuriken::j1Shuriken(iPoint position, Type type,int id) : j1Entity(position, type, id) {

	collider = App->collision->AddCollider({ position.x, position.y, shuriken_size, shuriken_size }, COLLIDER_COIN, this);
	// tex = App->tex->Load("Maps/Objects/shuriken.png");
	// tex = shuriken_tex; 
	animation = &Idle;
	Idle.PushBack({ 18, 0, shuriken_size, shuriken_size });
	Idle_Left.PushBack({ 0, 0, shuriken_size, shuriken_size });

	Shot_Right.PushBack({ 18, 0, shuriken_size, shuriken_size });
	Shot_Right.PushBack({ 54, 0, shuriken_size, shuriken_size });
	Shot_Right.loop = true; 
	Shot_Right.speed = 0.1f; 

	Shot_Left.PushBack({ 0, 0, shuriken_size, shuriken_size });
	Shot_Left.PushBack({ 36, 0, shuriken_size, shuriken_size });
	Shot_Left.loop = true;
	Shot_Left.speed = 0.1f;

	this->my_id = id;
	state = ShuriState::IDLE; 
	this->position = position;
}

bool j1Shuriken::Update(float dt) {

	
	Movement(); 
	Set_Anim_and_Collider(); 

	return true; 
}


void j1Shuriken::Movement() {

	uint offset = 0;

	switch (state) {
	case ShuriState::WITH_PLAYER:

		if (collider->type == COLLIDER_SHOT) {
			collider->type = COLLIDER_NONE;
		}
		
		if (App->entity_manager->GetPlayerDir() == -1) {
			offset = 35; 
		}
		else if (App->entity_manager->GetPlayerDir() == 1) {
			offset = -shuriken_size;
		}
		position.x = App->entity_manager->GetPlayerPos().x + offset;
		position.y = App->entity_manager->GetPlayerPos().y + 15;

		if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN) {
			App->audio->PlayFx(6);
			state = ShuriState::LAUNCHED;
		}
		break;

	case ShuriState::LAUNCHED:

		if (collider->type == COLLIDER_NONE) {
			collider->type = COLLIDER_SHOT;
		}

		animation = &Shot_Right; 

		position.x += 20;
		break;

	case ShuriState::RETURNING:

		return_pos.x = App->entity_manager->GetPlayerPos().x;
		return_pos.y = App->entity_manager->GetPlayerPos().y + 20;

		dir.x = return_pos.x - position.x;
		dir.y = return_pos.y - position.y;

		/*if (dir.x > 0) {
			animation = &Shot_Right; 
		}
		else {*/
			animation = &Shot_Left;
	//	}
			float return_speed = 0.1f; 
		position.x += dir.x* return_speed;
		position.y += dir.y* return_speed;
		

		break;

	}


}

void j1Shuriken::Set_Anim_and_Collider() {

	if (App->entity_manager->GetPlayerDir() == -1 ){
		animation = &Idle_Left; 
	}
	else if (App->entity_manager->GetPlayerDir() == 1) {
		animation = &Idle; 
	}
   
	collider->SetPos(position.x, position.y);
}


void j1Shuriken::OnCollision(Collider* c1, Collider* c2) {


	if (c2->type == COLLIDER_PLAYER || c2->type == COLLIDER_GOD) {

		if (animation != &Idle) {
			animation = &Idle; 
		}

		if (state == ShuriState::RETURNING) {
			obtained = false; 
		}

		if (!obtained) {
			position.x = App->entity_manager->GetPlayerPos().x + 45;
			position.y = App->entity_manager->GetPlayerPos().y + 15;

			state = ShuriState::WITH_PLAYER;
			obtained = true; 
		}
	}

	if (c2->type == COLLIDER_ENEMY || c2->type == COLLIDER_WALL) {

		state = ShuriState::RETURNING;

	}

}

bool j1Shuriken::CleanUp() {
	collider->to_delete = true;
	//App->tex->UnLoad(tex);
	active = false; 
	return true;
}

