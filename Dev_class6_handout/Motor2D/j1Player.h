#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"  
#include "j1Collision.h"

#define PLAYER_WIDTH 35
#define PLAYER_HEIGHT 45
#define MAX_SPEED_X 20
#define MAX_ACCELERATION_Y 2

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

	void Movex();
	void Movey();
	PlayerState Get_Player_State();
	void OnCollision(Collider* c1, Collider* c2);
	void Set_Player_Info();
	void Debug_Keys(); 
	void Switch_Level_Logic(); 


	// Called before quitting
	bool CleanUp();

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

	Animation Jumping_Right;
	Animation Jumping_Left; 
	Animation Falling_Right;
	Animation Falling_Left; 

public: 

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
	bool Arrived_Lvl2 = false; 
	Collider* Player_Collider = nullptr;


public:





};

#endif