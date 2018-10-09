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
	bool Update(float dt);
	bool PostUpdate();
	bool Draw(); 
	void Jump_Calculator(uint Jump_Speed, uint Speed_X, float Flying_Speed_Decrease);

	// Called before quitting
	bool CleanUp();

	/*void SwitchToDown(Animation*);     // switch animation functions, they should recieve the "idle" animation
	void SwitchToUp(Animation*);*/
private:

	SDL_Texture* Player_Texture = nullptr;
	Animation* Player_Animation = nullptr; 
	Animation Idle;
	Animation None; 
	fPoint Position;
	uint Gravity; 
	fPoint Speed;
	bool Is_Jumping = false;
public: 
	float Flying_Speed_Decrease = 0.5f;
	
	 
};

#endif // __j1PLAYER_H__