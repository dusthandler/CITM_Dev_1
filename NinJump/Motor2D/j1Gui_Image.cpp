#include "j1Gui_Image.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Entity_Manager.h"
#include "p2Log.h"

// Called before render is available
j1Gui_Image::j1Gui_Image(SDL_Texture* tex, iPoint pos, SDL_Rect atlas_rect, char* ID, Menu_Level menu_level, j1Gui_Object* parent) : j1Gui_Object(pos) {

	this->tex = tex; 
	rect = atlas_rect; 
	this->pos = pos; 
	this->ID = ID;
	this->menu_level = menu_level;

	this->parent = parent;
	hierarchy = (Hierarchy)4; 
	// hover_state = Hover_State::OUTSIDE; 
};



void j1Gui_Image::Blit() {

	

	if (ID == "UI_lives") {

		switch (App->entity_manager->player_live_count) {
		case 2: 
			rect.x = 68;             // half life heart
			break; 
		case 0: 
			rect.x = 104;            // no life heart
			break; 
		}

	}

	App->render->Blit(tex, pos.x, pos.y, &rect, 0.0f);

}
