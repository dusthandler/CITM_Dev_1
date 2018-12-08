#include "j1Shuriken.h"
#include "j1Entity_Manager.h"
#include "j1Collision.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "p2Log.h"

j1Shuriken::j1Shuriken(iPoint position, Type type) : j1Entity(position, type) {

	collider = App->collision->AddCollider({ position.x, position.y, shuriken_size, shuriken_size }, COLLIDER_SHOT, this);
	tex = App->tex->Load("Maps/Objects/shuriken.png");
	animation = &Idle;
	Idle.PushBack({ 18, 0, shuriken_size, shuriken_size });
	Idle_Left.PushBack({ 0, 0, shuriken_size, shuriken_size });

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

		position.x += 20;
		break;

	case ShuriState::RETURNING:

		return_pos.x = App->entity_manager->GetPlayerPos().x;
		return_pos.y = App->entity_manager->GetPlayerPos().y + 20;

		dir.x = return_pos.x - position.x;
		dir.y = return_pos.y - position.y;

		position.x += dir.x* 0.05f;
		position.y += dir.y* 0.05f;

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
	App->tex->UnLoad(tex);

	return true;
}

