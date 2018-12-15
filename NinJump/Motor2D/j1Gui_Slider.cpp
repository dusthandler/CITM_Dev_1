#include "j1Gui_Slider.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Log.h"

// Called before render is available
j1Gui_Slider::j1Gui_Slider(SDL_Texture* tex, iPoint pos, SDL_Rect rect, j1Gui_Object* parent, char* ID, Menu_Level menu_level, bool draggable) : j1Gui_Object(pos) {

	this->tex = tex;
	this->rect = rect;
	this->pos = pos;
	this->ID = ID;
	this->menu_level = menu_level;

	this->draggable = draggable;
	this->parent = parent;

	type = GUI_TYPE::Slider; 
	// hover_state = Hover_State::OUTSIDE; 
};



void j1Gui_Slider::Blit() {

	App->render->Blit(tex, pos.x, pos.y, &rect, 0.0f);

}



