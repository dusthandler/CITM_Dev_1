#include "j1Gui_Image.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"

// Called before render is available
j1Gui_Image::j1Gui_Image(SDL_Texture* tex, iPoint pos, SDL_Rect atlas_rect) : j1Gui_Object(pos) {

	this->tex = tex; 
	rect = atlas_rect; 
	this->pos = pos; 
};



void j1Gui_Image::Blit() {

	App->render->Blit(tex, pos.x, pos.y, &rect, 0.0f);

}
