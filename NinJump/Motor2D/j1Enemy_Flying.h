#ifndef __j1ENEMY_FLYING_H__
#define __j1ENEMY_FLYING_H__

#include "j1Entity.h"

#define Fly_Width 64

struct SDL_Texture;

class j1Enemy_Flying : public j1Entity
{
public:
	j1Enemy_Flying(iPoint pos, Type type);

	// Destructor
	virtual ~j1Enemy_Flying() {

	}

	bool Update(float dt);
	bool CleanUp(); 
	void Follow_Path() override;
	void Move(float dt); 
	void Set_Anim(); 
	
	// void Start_Follow()    TODO: only if enemy is near player, start following path

private:

	Animation Idle;
	Animation Left;
	Animation Right;

	bool Path_Generated = false; 
	iPoint Target_Map_Pos; 
	iPoint Enemy_Map_Pos; 
};

#endif // __j1ENEMY_FLYING_H__ 
