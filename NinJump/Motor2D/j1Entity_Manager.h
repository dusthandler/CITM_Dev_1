#ifndef __j1ENTITY_MANAGER_H__
#define __j1ENTITY_MANAGER_H__


#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"

enum class Type
{
	PLAYER,
	ENEMY_FLYING,
	ENEMY_LAND,
	UNKNOWN
};

class j1Entity;

class j1Entity_Manager: public j1Module
{
public:
	j1Entity_Manager();

	// Destructor
	virtual ~j1Entity_Manager();

/*	bool Awake(pugi::xml_node&); */ // TODO: check the necessity

	bool Start();

	bool PreUpdate(float dt) {
		return true;
	}
	bool Update(float dt);
	bool UpdateAll(float dt, bool do_logic);

	bool PostUpdate(float dt) {
		return true;
	}
	

	j1Entity* CreateEntity(Type, iPoint); 
	void DestroyEntity(j1Entity* entity); 
  
	void Draw(); 

	// Called before quiting 
	bool CleanUp();
	bool Load(pugi::xml_node& node);
	bool Save(pugi::xml_node& node) const;

	// player
	iPoint Initialize_Player_Pos(); 
	j1Entity* Get_Player(); 


private: 
	p2List<j1Entity*> entities; 
	float accumulated_time = 0.0f; 
	float update_ms_cycle = 0.0f;  // time of a frame in ms ? 
	bool do_logic = false; 
	
public: 
	uint Player_Count = 0;
}; 



#endif // __j1ENTITY_MANAGER_H__ 
