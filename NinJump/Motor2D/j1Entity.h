#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "j1Entity_Manager.h"


struct SDL_Texture;

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

protected:
	iPoint position; 
	Type type; 
	SDL_Texture* tex; 
	bool active = false; 
	
	
}; 

#endif // __j1ENTITY_H__ 