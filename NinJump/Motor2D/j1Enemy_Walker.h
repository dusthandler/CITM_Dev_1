#ifndef __j1ENEMY_WALKER_H__
#define __j1ENEMY_WALKER_H__

#include "j1Entity.h"

struct SDL_Texture;

class j1Enemy_Walker : public j1Entity
{
public:
	j1Enemy_Walker(iPoint pos, Type type,int id);

	// Destructor
	virtual ~j1Enemy_Walker() {

	}

	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp(); 
	void Follow_Path(); 
	void OnCollision(Collider* c1, Collider* c2);
	void Move(float dt);
	//void Draw_Path(); 
	void Set_Anim();
	bool Load(pugi::xml_node& node);
	bool Save(pugi::xml_node& node) const;
private:

	Animation Iddle_Left;
	Animation Walking_Left;
	Animation Pushing_Left;
	Animation Pushing_Right;
	Animation Iddle_Right;
	Animation Walking_Right;
	Animation Kicked_Left;
	Animation Kicked_Right;
	Animation Falling_Left;
	Animation Falling_Right;

	bool Onplat = false;
	uint Gravity = 10;
	iPoint Target_Map_Pos;
	iPoint Enemy_Map_Pos;
};

#endif // __j1ENEMY_FLYING_H__ 