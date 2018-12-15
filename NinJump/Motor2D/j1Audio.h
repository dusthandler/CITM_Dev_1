#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

#define MAX_MUSICS 5

struct _Mix_Music;
struct Mix_Chunk;

class j1Audio : public j1Module
{
public:

	j1Audio();

	// Destructor
	virtual ~j1Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	_Mix_Music* LoadMus(const char* path);
	void PlayMus(_Mix_Music* mus);
	bool UnloadMus(_Mix_Music* mus);


	void Change_Mus_Volume(int volume); 
	void Change_Fx_Volume(int volume);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);
	// bool UnloadFx(unsigned int fx);

private:

	_Mix_Music*			musics[MAX_MUSICS];
	p2List<Mix_Chunk*>	fx;
	int mus_vol = 15; 
	int fx_vol = 15;

};

#endif // __j1AUDIO_H__