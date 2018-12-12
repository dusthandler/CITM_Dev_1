#ifndef __j1PLAYER_ENTITY_H__
#define __j1PLAYER_ENTITY_H__

#include "p2Point.h"
#include "Animation.h"  
#include "j1Collision.h"
#include "j1Entity.h"


#define PLAYER_WIDTH 35
#define PLAYER_HEIGHT 45
#define MAX_SPEED_X 5
#define MAX_ACCELERATION_Y 2
#define MAX_SPEED_Y 400
#define MAX_TOTAL_SPEED 20

struct SDL_Texture;
struct SDL_Rect;
struct Collider;

enum PlayerStat {
	IDLEE,
	WALKING_RIGHTT,
	WALKING_LEFTT,
	JUMPING_UPP,
	JUMPING_RIGHTT,
	JUMPING_LEFTT,
	FALLING_DOWNN,
	FALLING_RIGHTT,
	FALLING_LEFTT,
	DIEDD,
	WINN
};

class j1Player_Entity : public j1Entity

{
public:
	j1Player_Entity(iPoint pos, Type type, int id);

	~j1Player_Entity();

	//Updates
	bool PreUpdate();
	void OnCollision(Collider* c1, Collider* c2);
	bool Update(float dt);
	PlayerStat Get_Player_State();
	void Debug_Keys();
	void Movex(float dt);
	void Movey(float dt);
	void Solve_Move(float dt);
	bool PostUpdate();
	void Blit(); 
	bool CleanUp();
	bool Load(pugi::xml_node& node);
	bool Save(pugi::xml_node& node) const;


private:

	Animation Idle;
	Animation None;
	Animation Falling;
	Animation Walking_Right;
	Animation Walking_Right2;
	Animation Walking_Left;
	Animation Walking_Left2;
	Animation Jumping_Up;
	Animation Jumping_Right;
	Animation Jumping_Left;
	Animation Falling_Right;
	Animation Falling_Left;
	Animation Death;


public:

	int Map_Switch;
	PlayerStat State;

	uint Jump_Count = 0;
	float Cont;
	float Cont_X;

	bool Jumping = false;
	bool Onplat = false;

	bool Inside_Collider = false;
	bool God_Mode = false;
	bool In_Lvl_1 = false;

	
	bool Level_Win = false;
	bool Respawning = true;


	


	// fx
	bool Reset_Fx_2 = true;
	bool Reset_Fx_3 = true;
	bool Reset_Fx_Gravity = true;



};
	
#endif