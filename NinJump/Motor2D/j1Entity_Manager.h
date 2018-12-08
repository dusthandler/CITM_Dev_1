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
	COIN,
	SHURIKEN,
	UNKNOWN
};



class j1Entity;
struct SDL_Texture; 

class j1Entity_Manager: public j1Module
{
public:
	j1Entity_Manager();

	// Destructor
	virtual ~j1Entity_Manager();

/*	bool Awake(pugi::xml_node&); */ // TODO: check the necessity

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool UpdateAll(float dt, bool do_logic);
	iPoint GetPlayerPos();
	int GetPlayerDir();

	bool Get_Gravity_Reverse();
	bool PostUpdate();



	j1Entity* CreateEntity(Type, iPoint); 
	void DestroyEntity(j1Entity* entity);
  
	void Draw(); 

	// Called before quiting 
	bool CleanUp();
	bool Load(pugi::xml_node& node);
	bool Save(pugi::xml_node& node) const;
	bool restart = false;


private: 
	p2List<j1Entity*> entities; 
	float accumulated_time = 0.0f; 
	float update_ms_cycle = 0.0f;  
	bool do_logic = false; 

public: 
	uint Coin_Score = 0;                  // Where should this be?
	SDL_Texture* path_tex = nullptr;
	
}; 



#endif // __j1ENTITY_MANAGER_H__ 
