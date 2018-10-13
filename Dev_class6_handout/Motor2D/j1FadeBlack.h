#ifndef __j1FADETOBLACK_H__
#define __j1FADETOBLACK_H__

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "j1Module.h"

class j1FB : public j1Module
{
public:
	j1FB();
	~j1FB();

	bool Start();
	bool Update(float dt);
	bool FadeToBlack(j1Module* module_off, j1Module* module_on, float time = 2.0f);

private:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
	j1Module* ModuleOff = nullptr;
	j1Module* ModuleOn = nullptr;

};

#endif //__j1FADETOBLACK_H__
