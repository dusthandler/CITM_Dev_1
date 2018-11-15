/*#ifndef __J1ENEMIES__
#define __J1ENEMIES__

#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"  
#include "j1Collision.h"
#include "p2DynArray.h"


#define ENEMY_FLY_WIDTH 35
#define ENEMY_FLY_HEIGHT 45
#define ENEMY_FLY_MAX_SPEED_X 20
#define ENEMY_FLY_MAX_ACCELERATION_Y 2

struct SDL_Texture;
struct SDL_Rect;
struct Collider;
struct Enemy;

enum EnemyState {
	ENEMY_IDLE,
	ENEMY_WALKING_RIGHT,
	ENEMY_WALKING_LEFT,
	ENEMY_DIED,
	HUNTING
};


class j1Enemies : public j1Module
{
public:

	j1Enemies();

	// Destructor
	virtual ~j1Enemies();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	//bool Update(float dt);
	//bool Draw();
	//void OnCollision(Collider* c1, Collider* c2);
	void Set_Enemies_Info();

private:

	Animation* EnemyFly_Animation = nullptr;
	Animation EnemyFly_Idle;
	Animation Enemy_Flying_Right;
	Animation Enemy_Flying_Left;
	Animation Enemy_Fly_Death;


public:

	p2DynArray<Enemy> enemies;

};

struct Enemy {

	SDL_Texture* Enemy_Texture = nullptr;
	
	EnemyState State;

	fPoint Pos;
	fPoint Vel;
	fPoint Acc;
	bool Flying = false;
	bool Onplat = false;
	bool Alive = true;
	bool Inside_Collider = false;

	Collider* Enemy_Collider = nullptr;

};



#endif*/