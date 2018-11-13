#ifndef __J1ENEMIES__
#define __J1ENEMIES__

#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"  
#include "j1Collision.h"

#define ENEMY_FLY_WIDTH 35
#define ENEMY_FLY_HEIGHT 45
#define ENEMY_FLY_MAX_SPEED_X 20
#define ENEMY_FLY_MAX_ACCELERATION_Y 2

struct SDL_Texture;
struct SDL_Rect;
struct Collider;

enum EnemyState {
	IDLE,
	WALKING_RIGHT,
	WALKING_LEFT,
	DIED,
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

#endif