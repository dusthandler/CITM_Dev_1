#include "j1Gui_Label.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"

// Called before render is available
j1Gui_Label::j1Gui_Label(iPoint pos, _TTF_Font* f, char* text, char* ID) : j1Gui_Object(pos) {
	this->text = text;
	font = f;
	this->ID = ID; 

	Init_Text(); 

};

void j1Gui_Label::Init_Text() {

	rect.x = 0; 
	rect.y = 0; 
	App->font->CalcSize(text, rect.w, rect.h, font);
	tex = App->font->Print(text, SDL_Color{ 0, 0, 0, 255 }, font); 

	
}


void j1Gui_Label::Blit() {


	if (ID == "coin_score") {
		char buffer[20];

		snprintf(buffer, sizeof(buffer), "X%i", App->gui->coins_collected_by_player);

		text = buffer; 

		App->font->CalcSize(text, rect.w, rect.h, font);
		tex = App->font->Print(text, SDL_Color{ 0, 0, 0, 255 }, font);
	}

	App->render->Blit(tex, pos.x, pos.y, &rect, 0.0f); 

}