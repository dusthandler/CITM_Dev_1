#include "j1Gui_Button.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1FadeBlack.h"
#include "j1Entity_Manager.h"
#include "j1Main_Menu.h"
#include "p2Log.h"

// Called before render is available
j1Gui_Button::j1Gui_Button(Hover_Anim hover_rects, SDL_Texture* tex, iPoint pos, char* ID, Menu_Level menu_level, j1Gui_Object* parent, bool draggable) : j1Gui_Object(pos) {

	this->hover_rects = hover_rects;
    rect = hover_rects.a_Idle;
	this->tex = tex;
	this->pos = pos;
	this->ID = ID;
	this->menu_level = menu_level; 
	
	this->draggable = draggable; 
	this->parent = parent; 
	type = GUI_TYPE::Button;
	hierarchy = (Hierarchy)5;
	// hover_state = Hover_State::OUTSIDE; 
};



void j1Gui_Button::Blit() {

	if (hover_state == Hover_State::OUTSIDE) {
		rect = hover_rects.a_Idle;
	}
	else if (hover_state == Hover_State::HOVER) {
		rect = hover_rects.a_Hover;
	}
	else if (hover_state == Hover_State::CLICK) {
		rect = hover_rects.a_Hover;                // change to click
	}

	App->render->Blit(tex, pos.x, pos.y, &rect, 0.0f);

}
