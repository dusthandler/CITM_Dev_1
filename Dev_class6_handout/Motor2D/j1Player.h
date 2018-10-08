#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"

#include "p2Point.h"
#include "Animation.h"  

struct SDL_Texture;
struct SDL_Rect; 

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
	bool Update();
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


	/*void SwitchToDown(Animation*);     // switch animation functions, they should recieve the "idle" animation
	void SwitchToUp(Animation*);*/
public:

	SDL_Texture* Player_Texture = nullptr;
	SDL_Rect* Player_Rect = nullptr; 
	Animation* Player_Animation = nullptr; 
	iPoint Position;
	uint Gravity; 
	iPoint Speed;
	 
};

#endif // __j1PLAYER_H__