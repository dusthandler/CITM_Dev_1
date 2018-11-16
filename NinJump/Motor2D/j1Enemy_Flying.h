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
	//bool HandleInput(); 
	void Follow_Path();
	void Path_Dir_Logic(); 
	// void Start_Follow()    TODO: only if enemy is near player, start following path

private:
	
	Animation Idle; 
	Movement_State m_state; 
};

#endif // __j1ENEMY_FLYING_H__ 
