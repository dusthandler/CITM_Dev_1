#ifndef __j1PARTICLES_H__
#define __j1PARTICLES_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "j1Collision.h"

#define MAX_ACTIVE_PARTICLES 100
#define MAX_CHUNKS 50
#define MAX_TEXTURES 20

struct SDL_Texture;
struct Collider;
struct Mix_Chunk; 
enum COLLIDER_TYPE;

struct Particle
{
	Collider* collider = nullptr;
	Animation anim;
	uint fx = -1;
	iPoint position;
	fPoint speed;
	Uint32 born = 0;
	Uint32 life = 0;
	uint texture = -1; 
	bool fx_played = false;

	Particle();
	Particle(const Particle& p);
	~Particle();
	bool Update();
};

class j1Particles : public j1Module
{
public:
	j1Particles();
	~j1Particles();
	uint explosion;
	uint finished;
	bool TNT = true;

	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	void AddParticle(const Particle& particle, int x, int y, COLLIDER_TYPE collider_type = COLLIDER_NONE, Uint32 delay = 0);

private:

	Particle* active[MAX_ACTIVE_PARTICLES];
	uint last_particle = 0;
	
public:

	Particle Player_Death; 

	SDL_Texture* textures[MAX_TEXTURES];
	Mix_Chunk* chunks[MAX_CHUNKS];
	
};

#endif // __j1PARTICLES_H__