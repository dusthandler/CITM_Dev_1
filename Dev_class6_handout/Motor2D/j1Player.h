#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"

#include "p2Point.h"
#include "Animation.h"  
#include "j1Collision.h"

#define PLAYER_WIDTH 35
#define PLAYER_HEIGHT 45

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




	// Called before quitting
	bool CleanUp();

	/*void SwitchToDown(Animation*);     // switch animation functions, they should recieve the "idle" animation
	void SwitchToUp(Animation*);*/
private:

	SDL_Texture* Player_Texture = nullptr;
	Animation* Player_Animation = nullptr;
	Animation Idle;

	

public: 

	PlayerState State;
	Animation None;
	fPoint Pos;
	fPoint Vel;
	fPoint Acc;
	float Cont;
	bool Jumping = false;
	bool Onplat = false;
	bool Alive = true; 
	Collider* Player_Collider = nullptr;

public:





};

#endif