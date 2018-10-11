




#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"

#include "p2Point.h"
#include "Animation.h"  
#include "j1Collision.h"

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

	void Move();
	PlayerState Get_Player_State();
	void OnCollision(Collider* c1, Collider* c2);
	void Set_Player_Info();
<<<<<<< HEAD

=======
>>>>>>> f5c8d5309d3cc32524cee759da48eee198ff97d1

	// Called before quitting
	bool CleanUp();

	/*void SwitchToDown(Animation*);     // switch animation functions, they should recieve the "idle" animation
	void SwitchToUp(Animation*);*/
private:

	SDL_Texture* Player_Texture = nullptr;
	Animation* Player_Animation = nullptr;
	Animation Idle;
<<<<<<< HEAD
	Animation None; 
	float Gravity; 
	fPoint Speed;
	bool Is_Flying = false; 
	PlayerState State = IDLE; 
	Collider* Player_Collider = nullptr; 
	

public: 
	float Flying_Speed_Decrease = 0.5f;
	fPoint Position;
	int FloorLevel = 300;
	bool Floor_Level_Active = false;
=======
	Animation None;
	fPoint Position;
	uint Gravity;
	fPoint Speed;
	bool Is_Flying = false;
	PlayerState State = IDLE;

	Collider* Player_Collider = nullptr;
public:
	float Flying_Speed_Decrease = 0.5f;
	bool higher_jump = false;
	bool normal_jump = false;


>>>>>>> f5c8d5309d3cc32524cee759da48eee198ff97d1
};

#endif