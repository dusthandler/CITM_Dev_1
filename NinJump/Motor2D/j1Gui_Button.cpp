#include "j1Gui_Button.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Entity_Manager.h"
#include "p2Log.h"

// Called before render is available
j1Gui_Button::j1Gui_Button(Hover_Anim* anim, SDL_Texture* tex, SDL_Rect atlas_rect, iPoint pos, _TTF_Font* f, char* text, char* ID, Menu_Level menu_level) : j1Gui_Object(pos) {

	this->anim = anim; 
	this->tex = tex;
	rect = atlas_rect;
	this->pos = pos;
	this->text = text;
	font = f;
	this->ID = ID;
	this->menu_level = menu_level; 



	hierarchy = (Hierarchy)5;
	// hover_state = Hover_State::OUTSIDE; 
};



void j1Gui_Button::Blit() {

	App->render->Blit(tex, pos.x, pos.y, &current_anim->GetCurrentFrame(), 0.0f);

}
