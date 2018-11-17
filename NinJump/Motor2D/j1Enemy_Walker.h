#ifndef __j1ENEMY_WALKER_H__
#define __j1ENEMY_WALKER_H__

#include "j1Entity.h"

struct SDL_Texture;

class j1Enemy_Walker : public j1Entity
{
public:
	j1Enemy_Walker(iPoint pos, Type type);

	// Destructor
	virtual ~j1Enemy_Walker() {

	}

	bool Update(float dt);
	bool PostUpdate(); 
	bool Draw();
	//bool HandleInput(); 
	 void Follow_Path() override;
     void OnCollision(Collider* c1, Collider* c2);
	 void Path_Dir_Logic();
private:

	Animation Idle;
	bool Onplat = false; 
	uint Gravity = 10; 

};

#endif // __j1ENEMY_FLYING_H__ 
