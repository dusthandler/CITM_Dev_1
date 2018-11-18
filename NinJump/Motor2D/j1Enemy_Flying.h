#ifndef __j1ENEMY_FLYING_H__
#define __j1ENEMY_FLYING_H__

#include "j1Entity.h"

struct SDL_Texture;

class j1Enemy_Flying : public j1Entity
{
public:
	j1Enemy_Flying(iPoint pos, Type type);

	// Destructor
	virtual ~j1Enemy_Flying() {

	}

	bool Update(float dt);
	bool Draw();
	bool CleanUp(); 
	//bool HandleInput(); 
	void Follow_Path() override;
	void Move(float dt); 
	
	// void Start_Follow()    TODO: only if enemy is near player, start following path

private:

	Animation Idle;
	bool Path_Generated = false; 
};

#endif // __j1ENEMY_FLYING_H__ 
