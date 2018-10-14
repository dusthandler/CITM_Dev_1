#include <math.h>
#include "j1FadeBlack.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Window.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

j1FB::j1FB()
{
	name.create("fadeToBlack");
}

j1FB::~j1FB()
{}

// Load assets
bool j1FB::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	screen = { 0, 0, (int)App->win->width * 1, (int)App->win->height * 1 };
	return true;
}

// Update: draw background
bool j1FB::Update(float dt)
{
	bool ret = true;
	if (current_step == fade_step::none) {
		return ret;
		
	}
		

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			
			ModuleOff->Deactivate(); 
			ModuleOn->Activate(); 
			LOG("FADING_1");
			// ---
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;
		LOG("FADING_2");
		if (now >= total_time)
			current_step = fade_step::none;
	} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return ret;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1FB::FadeToBlack(j1Module* module_off, j1Module* module_on, float time)
{
	bool ret = false;
	LOG("INIT FADE");
	
	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		ModuleOff = module_off;
		ModuleOn = module_on;
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
	}

	return ret;
}