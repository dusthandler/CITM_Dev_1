#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"  
#include "j1Collision.h"

#define PLAYER_WIDTH 35
#define PLAYER_HEIGHT 45
#define MAX_SPEED_X 5
#define MAX_ACCELERATION_Y 2
#define MAX_SPEED_Y 700
#define MAX_TOTAL_SPEED 20

struct SDL_Texture;
struct SDL_Rect;
struct Collider;


enum PlayerState {
	IDLE,
	WALKING_RIGHT,
	WALKING_LEFT,
	JUMPING_UP,
	JUMPING_RIGHT,
	JUMPING_LEFT,
	FALLING_DOWN,
	FALLING_RIGHT,
	FALLING_LEFT,
	DIED,
	WIN
};

class j1Player : public j1Module
{
public:
	j1Player();

	// Destructor
	virtual ~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool Draw();
	void OnCollision(Collider* c1, Collider* c2);

	void Movex(float dt);
	void Movey(float dt);
	void Solve_Move(float dt); 
	PlayerState Get_Player_State();
	void Set_Player_Info();
	void Debug_Keys(); 
	void Switch_Level_Logic(); 


	// Called before quitting
	bool CleanUp();
	bool Load(pugi::xml_node& node);
	bool Save(pugi::xml_node& node) const;

	/*void SwitchToDown(Animation*);     // switch animation functions, they should recieve the "idle" animation
	void SwitchToUp(Animation*);*/
private:

	SDL_Texture* Player_Texture = nullptr;
	Animation* Player_Animation = nullptr;
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
	int mapLo;
	PlayerState State;
	
	fPoint Pos;
	fPoint Vel;
	fPoint Acc;
	float Cont;
	float Cont_X; 
	bool Jumping = false;
	bool Onplat = false;
	bool Alive = true; 
	bool Inside_Collider = false; 
	bool God_Mode = false; 
	bool In_Lvl_1 = false;
	
	uint Jump_Count = 0; 
	bool Level_Win = false; 
	bool Respawning = true; 


	Collider* Player_Collider = nullptr;

	// gravity
	bool gravity_reverse = false; 


	// fx
	bool Reset_Fx_2 = true;
	bool Reset_Fx_3 = true;
	bool Reset_Fx_Gravity = true;

public:





};

#endif