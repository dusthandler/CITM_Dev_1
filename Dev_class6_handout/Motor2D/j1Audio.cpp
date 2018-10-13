#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Audio.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

j1Audio::j1Audio() : j1Module()
{
	for (int i = 0; i < MAX_MUSICS; ++i) {
		musics[i] = nullptr;
	}
	name.create("audio");
}

// Destructor
j1Audio::~j1Audio()
{}

// Called before render is available
bool j1Audio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	return ret;
}

// Called before quitting
bool j1Audio::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	for (int i = 0; i < MAX_MUSICS; ++i)
	{
		if (musics[i] != nullptr) {
			Mix_FreeMusic(musics[i]);
			musics[i] = nullptr;
		}
	}


	p2List_item<Mix_Chunk*>* item;
	for(item = fx.start; item != NULL; item = item->next)
		Mix_FreeChunk(item->data);

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file

_Mix_Music* j1Audio::LoadMus(const char * path)
{
	_Mix_Music* music = NULL;
	music = Mix_LoadMUS(path);

	if (music == NULL)
	{
		LOG("Could not load music with path: %s. Mus_Load: %s", path, Mix_GetError());
	}
	else
	{
		bool room = false;
		for (int i = 0; i < MAX_MUSICS; ++i)
		{
			if (musics[i] == nullptr)
			{
				musics[i] = music;
				room = true;
				break;
			}
		}
		if (room == false) {
		LOG("Music buffer overflow");
		}
	}
	
	return music;
}

void j1Audio::PlayMus(_Mix_Music* mus)
{
	Mix_PlayMusic(mus, -1);
}

bool j1Audio::UnloadMus(_Mix_Music* mus)
{
	bool ret = false;
	if (mus != nullptr) {
		for (int i = 0; i < MAX_MUSICS; ++i)
		{
			if (musics[i] == mus)
			{
				Mix_FreeMusic(musics[i]);
				musics[i] = nullptr;
				ret = true;
				LOG("Could unload the music properly");
				break;
			}
		}
	}

	return ret;
}


// Load WAV
unsigned int j1Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.add(chunk);
		ret = fx.count();
	}

	return ret;
}

// Play WAV
bool j1Audio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	if(!active)
		return false;

	if(id > 0 && id <= fx.count())
	{
		Mix_PlayChannel(-1, fx[id - 1], repeat);
	}

	return ret;
}