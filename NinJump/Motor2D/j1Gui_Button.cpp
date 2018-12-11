#include "j1Gui_Button.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Entity_Manager.h"
#include "p2Log.h"

// Called before render is available
j1Gui_Button::j1Gui_Button(/*Hover_Anim* hover_rects,*/ SDL_Texture* tex, iPoint pos, char* ID, Menu_Level menu_level, j1Gui_Object* parent) : j1Gui_Object(pos) {

	//this->hover_rects = hover_rects;
	// rect = hover_rects->a_Idle;
	this->tex = tex;
	this->pos = pos;
	this->ID = ID;
	this->menu_level = menu_level; 
	
	
	this->parent = parent; 
	hierarchy = (Hierarchy)5;
	// hover_state = Hover_State::OUTSIDE; 
};



void j1Gui_Button::Blit() {

/*	if (ID == "play_button") {
		rect = { 3, 43, 65, 79 };
		rect = { 72, 41, 170, 95 };
		rect = { 1000, 1000, 1000, 1000 }; 
	}; */


	if (hover_state == Hover_State::OUTSIDE) {
		// rect = hover_rects->a_Idle;
		rect = { 3, 43, 65, 79 };
	}
	else if (hover_state == Hover_State::HOVER) {
		rect = { 72, 41, 170, 95 };
		// rect = hover_rects->a_Hover;
	}
	else if (hover_state == Hover_State::CLICK) {
		// rect = hover_rects->a_Hover;                // change to click
	}
	App->render->Blit(tex, pos.x, pos.y, &rect, 0.0f);

}
