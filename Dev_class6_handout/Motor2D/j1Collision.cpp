#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "p2Log.h"

j1Collision::j1Collision()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_DEATH] = false;
	matrix[COLLIDER_WALL][COLLIDER_GOD] = true;
	matrix[COLLIDER_WALL][COLLIDER_WIN] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true; 
	matrix[COLLIDER_PLAYER][COLLIDER_DEATH] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_GOD] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_WIN] = true;

	matrix[COLLIDER_DEATH][COLLIDER_DEATH] = false;
	matrix[COLLIDER_DEATH][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_DEATH][COLLIDER_WALL] = false;
	matrix[COLLIDER_DEATH][COLLIDER_GOD] = true;
	matrix[COLLIDER_DEATH][COLLIDER_WIN] = false;

	matrix[COLLIDER_GOD][COLLIDER_GOD] = false;
	matrix[COLLIDER_GOD][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_GOD][COLLIDER_WALL] = true;
	matrix[COLLIDER_GOD][COLLIDER_DEATH] = true;
	matrix[COLLIDER_GOD][COLLIDER_WIN] = true;


	matrix[COLLIDER_WIN][COLLIDER_WIN] = false;
	matrix[COLLIDER_WIN][COLLIDER_GOD] = true;
	matrix[COLLIDER_WIN][COLLIDER_PLAYER] = true; 
	matrix[COLLIDER_WIN][COLLIDER_WALL] = false;
	matrix[COLLIDER_WIN][COLLIDER_DEATH] = false;
	
}

// Destructor
j1Collision::~j1Collision()
{}

bool j1Collision::PreUpdate()
{
	bool ret = true; 
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];
		
		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);
				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);

			}
		}
	}

	// Calculate collisions
	

	return ret;

}



// Called before render is available
bool j1Collision::Update(float dt)
{

	bool ret = true;
	
	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_REPEAT) {
		DebugDraw();
	}

	return ret;
}

void j1Collision::DebugDraw()
{
	/*if (App->input->keyboard[SDL_SCANCODE_1] == KEY_DOWN) {
		if (debug == false) debug = true;
		else debug = false;
	}

	if (debug == false)
		return;*/
	
	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{

		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_WALL: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_DEATH: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_GOD: // violet
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 255, alpha);
			break;
		case COLLIDER_WIN: 
			App->render->DrawQuad(colliders[i]->rect, 255, 150, 255, alpha);
			break;
		}
	}
}

// Called before quitting
bool j1Collision::CleanUp()
{
	bool ret = true;
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			
			colliders[i] = nullptr;
		}
	}

	return ret; 
}

bool j1Collision::CleanWallDeath()
{
	bool ret = true;
	LOG("Freeing wall and death colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			if (colliders[i]->type == COLLIDER_WALL || colliders[i]->type == COLLIDER_DEATH || colliders[i]->type == COLLIDER_WIN) {
				delete colliders[i];
				colliders[i] = nullptr;
			}
			
		}
	}

	return ret;
}

Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	bool ret = true;

	if (r.x + r.w < rect.x) ret = false;
	else if (r.x > rect.x + rect.w) ret = false;
	else if (r.y + r.h < rect.y)ret = false;
	else if (r.y > rect.h + rect.y)ret = false;

	return ret;
}