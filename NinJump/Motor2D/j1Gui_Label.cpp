#include "j1Gui_Label.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Entity_Manager.h"
#include "j1Textures.h"

// Called before render is available
j1Gui_Label::j1Gui_Label(iPoint pos, _TTF_Font* f, char* text, char* ID, Menu_Level menu_level, j1Gui_Object* parent, bool draggable) : j1Gui_Object(pos) {
	this->text = text;
	font = f;
	this->ID = ID; 

	this->menu_level = menu_level;

	if (parent != nullptr) {
		has_button = true; 
		this->parent = parent;
	}
	
	this->draggable = draggable;
	
	Init_Text(); 

	hierarchy = (Hierarchy)3; 
	// hover_state = Hover_State::OUTSIDE;
};

void j1Gui_Label::Init_Text() {

	rect.x = 0; 
	rect.y = 0; 
	App->font->CalcSize(text, rect.w, rect.h, font);

	tex = App->font->Print(text, SDL_Color{ 0, 0, 0, 255 }, font); 

	
}


void j1Gui_Label::Blit() {

	

	if (ID == "coin_score") {

		char buffer[200];
		snprintf(buffer, sizeof(buffer), "X%i", App->gui->coins_collected);
		text = buffer;
		tex = App->font->Print(text, SDL_Color{ 0, 0, 0, 255 }, font);

	}

	else if (ID == "life_count") {
		char buffer[200];
		snprintf(buffer, sizeof(buffer), "X%i", App->entity_manager->player_live_count);
		text = buffer;
		tex = App->font->Print(text, SDL_Color{ 0, 0, 0, 255 }, font);
	}
	else if (ID == "time_count") {
		char buffer[200];
		snprintf(buffer, sizeof(buffer), "GO! %.0f", App->entity_manager->player_startup_time.ReadSec());
		text = buffer;
		tex = App->font->Print(text, SDL_Color{ 0, 0, 0, 255 }, font);
	}



	if (!has_button) {
		App->render->Blit(tex, pos.x, pos.y, &rect, 0.0f);
	}
	else if(parent->hover_state != Hover_State::OUTSIDE && parent != nullptr)
	App->render->Blit(tex, pos.x, pos.y, &rect, 0.0f); 
	



	if (ID == "coin_score" || ID == "life_count" || ID ==  "time_count") {

		App->tex->UnLoad(tex); 
		tex = nullptr; 

	}

}
