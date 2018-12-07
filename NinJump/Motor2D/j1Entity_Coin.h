#ifndef __j1ENTITY_COIN_H__
#define __j1ENTITY_COIN_H__

#include "j1Entity.h"

#define coin_size 20


struct SDL_Texture;

class j1Entity_Coin : public j1Entity
{
public:
	j1Entity_Coin(iPoint pos, Type type);

	// Destructor
	virtual ~j1Entity_Coin() {

	}

	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

	/*bool Load(pugi::xml_node& node);
	bool Save(pugi::xml_node& node) const;*/



private:
	Animation Idle;

};

#endif // __j1ENTITY_COIN_H__ 

