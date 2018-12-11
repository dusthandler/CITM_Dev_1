#ifndef __j1SHURIKEN_H__
#define __j1SHURIKEN_H__

#include "j1Entity.h"

#define shuriken_size 18

struct SDL_Texture;

enum ShuriState {
	IDLE,
	WITH_PLAYER,
	LAUNCHED,
	RETURNING,
};

class j1Shuriken : public j1Entity
{
public:
	j1Shuriken(iPoint pos, Type type, int id);

	// Destructor
	virtual ~j1Shuriken() {

	}

	bool Update(float dt); 
	bool CleanUp();
	void Movement(); 
	void Set_Anim_and_Collider(); 
	void OnCollision(Collider* c1, Collider* c2);

	/*bool Load(pugi::xml_node& node);
	bool Save(pugi::xml_node& node) const;*/



private:
	Animation Idle;
	Animation Idle_Left;
	Animation Shot_Right; 
	Animation Shot_Left; 
	bool obtained = false; 
	iPoint return_pos; 
	ShuriState state = IDLE; 
};

#endif // __j1SHURIKEN_H__ 

