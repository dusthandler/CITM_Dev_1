#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"

struct SDL_Texture;
struct _Mix_Music;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node& node);
	bool Save(pugi::xml_node& node) const;


	bool MapSwap(int,bool init = false);


	int SwitchM = 0;
	bool Player_Alive = true;
	bool Player_Win;
	bool Restart = false;
	bool Second_Start = false;
	bool Player_lvl_2 = false; 
	bool Map_Loaded = false; 
	bool game_paused = false; 

private:
	iPoint Player_act_pos;
    // int camera_relative_x = 0; 
	
	// music
	_Mix_Music* mus = nullptr;
	_Mix_Music* mus2 = nullptr;
	bool Mus_1 = true; 
	uint Mus_Id = 1; 

	// extra
	j1PerfTimer			ptimer;
	SDL_Texture* debug_tex;

};

#endif // __j1SCENE_H__