#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "j1Entity_Manager.h"
#include "p2DynArray.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;
struct SDL_Rect;

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

	virtual bool CleanUp() {
		return true;
	}

	virtual void Follow_Path(){} 

	virtual void OnCollision(Collider* c1, Collider* c2) {

	}

	

public:

	iPoint position; 
	iPoint Vel;
	iPoint Acc;
	Type type; 
	bool active = false;
	bool to_delete = false;

	SDL_Texture* tex = nullptr; 
	Animation* animation = nullptr;
	Collider* collider = nullptr; 
	
	p2DynArray<iPoint>* Path; 

}; 

#endif // __j1ENTITY_H__ 