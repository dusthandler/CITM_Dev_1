#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "j1Entity_Manager.h"
#include "p2DynArray.h"

struct SDL_Texture;
struct Collider;

class j1Entity : public j1Entity_Manager
{
public:
	j1Entity() {

	}

	// Destructor
	virtual ~j1Entity() {

	}

	virtual bool  Update(float dt) {
		return true; 
	}

	virtual bool Draw() {
		return true; 
	}

	virtual bool HandleInput() {
		return true; 
	}

	virtual void FollowPath(){}

public:

	iPoint position; 
	Type type; 
	bool active = false;
	bool to_delete = false;

	SDL_Texture* tex; 
	Animation* animation = nullptr;
	Collider* collider = nullptr; 
	
	p2DynArray<iPoint>* Path; 

}; 

#endif // __j1ENTITY_H__ 