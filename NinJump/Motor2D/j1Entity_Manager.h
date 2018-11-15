#ifndef __j1ENTITY_MANAGER_H__
#define __j1ENTITY_MANAGER_H__


#include "j1Module.h"
#include "p2List.h"

enum class Type
{
	PLAYER,
	ENEMY,
	UNKNOWN
};

class j1Entity;

class j1Entity_Manager: public j1Module
{
public:
	j1Entity_Manager();

	// Destructor
	virtual ~j1Entity_Manager();

	bool Update(float dt); 
	bool UpdateAll(float dt, bool do_logic); 

	j1Entity* CreateEntity(Type); 

private: 
	p2List<j1Entity*> entities; 
	float accumulated_time = 0; 
	float update_ms_cycle = 0;  // time of a frame in ms ? 
	bool do_logic = false; 
	
}; 



#endif // __j1ENTITY_MANAGER_H__ 
