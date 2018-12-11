#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "j1Entity_Manager.h"
#include "p2DynArray.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;
struct SDL_Rect;

enum Direction_State {
	RIGHT,
	LEFT,
	DOWN,
	UP,
	STOP
};

struct Camera_Limits {
	bool left_x; 
	bool right_x; 
	bool top_y;
	bool bottom_y;
};

class j1Entity : public j1Entity_Manager
{
public:
	j1Entity(iPoint pos, Type type,int id);

	// Destructor
	virtual ~j1Entity() {

	}

	virtual bool PreUpdate() {
		return true;
	}



	virtual bool Update(float dt) {
		return true;
	}

	virtual bool PostUpdate() {
		return true;
	}

	struct Camera_Limits Inside_Camera_Limits();


	virtual void Follow_Path() {};

	bool Load(pugi::xml_node& node){
		return true;
	}
	bool Save(pugi::xml_node& node) const {
		return true;

	}

	virtual void Move(float dt) {};
	
	void Get_Direction_State(); 


public:
	
	int my_id;
	iPoint position;
	iPoint Vel;
	iPoint Acc;
	iPoint dir;
	iPoint Path_Pos; 
	uint dir_multiplier;
	Type type;
	bool active = true;
	bool to_delete = false;
	bool Reached_Player = false;
	bool following_player = false; 
	bool inside_limits = false; 
	// gravity
	bool gravity_reverse = false;
	uint gravity_times = 0; 
	                                        // nullptr's
	SDL_Texture* tex = nullptr;
	Animation* animation = nullptr;
	Collider* collider = nullptr;

	p2DynArray<iPoint>* Path;
	Direction_State m_state;

};

#endif __j1ENTITY_H__
