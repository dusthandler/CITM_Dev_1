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
	void Draw_Path();
	bool Load(pugi::xml_node& node);
	bool Save(pugi::xml_node& node) const;
	
	
	// void Start_Follow()    TODO: only if enemy is near player, start following path

private:

	Animation Idle;
	Animation Left;
	Animation Right;

	bool Path_Generated = false; 
	bool Drawing_Path = false; 
	iPoint Target_Map_Pos; 
	iPoint Enemy_Map_Pos; 


	p2DynArray<iPoint>* Path;
};

#endif // __j1ENEMY_FLYING_H__ 
