#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"


#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes

class j1Gui_Object; 
struct SDL_Texture; 
struct SDL_Rect; 
struct SDL_Color;
struct _TTF_Font; 

enum TYPE {
	Label,
	Button,
	Input_Text,
	Image,
	Checkbox
}; 

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	bool Update(float);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions

	void Create_Image(SDL_Texture*, iPoint, SDL_Rect&);  


	void Create_Label(iPoint, _TTF_Font*, char*);

	// void Delete_Object();

	virtual void Blit();

     const SDL_Texture* GetAtlas() const;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
	p2List<j1Gui_Object*> objects; 
	

};

#endif // __j1GUI_H__