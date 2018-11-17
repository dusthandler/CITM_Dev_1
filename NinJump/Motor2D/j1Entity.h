#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "j1Entity_Manager.h"
#include "p2DynArray.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;
struct SDL_Rect;

enum Movement_State {
	RIGHT,
	LEFT,
	DOWN,
	UP,
	STOP
};

class j1Entity : public j1Entity_Manager
{
public:
	j1Entity(iPoint pos, Type type);

	// Destructor
	virtual ~j1Entity() {

	}

	virtual bool PreUpdate() {
		return true;
	}



	virtual bool Update(float dt) {
		return true;
	}

	virtual bool PostUpdate() {
		return true;
	}

	virtual bool Draw() {
		return true;
	}

	virtual bool Handle_Input() {
		return true;
	}

	virtual void Follow_Path();

	virtual void Path_Dir_Logic() {};

public:

	iPoint position;
	iPoint Vel;
	iPoint Acc;
	iPoint dir;
	uint dir_multiplier;
	Type type;
	bool active = false;
	bool to_delete = false;
	bool Reached_Player = false;

	SDL_Texture* tex;
	Animation* animation = nullptr;
	Collider* collider = nullptr;

	p2DynArray<iPoint>* Path;
	Movement_State m_state;

};

#endif __j1ENTITY_H__
