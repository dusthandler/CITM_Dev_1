#include "j1Gui_Label.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"

// Called before render is available
j1Gui_Label::j1Gui_Label(iPoint pos, _TTF_Font* f, char* text) : j1Gui_Object(pos) {
	this->text = text;
	font = f;

	Init_Text(); 

};

void j1Gui_Label::Init_Text() {

	App->font->CalcSize(text, rect.w, rect.h, font);
	tex = App->font->Print(text, SDL_Color{ 255, 255, 255, 255 }, font); 
	
}


void j1Gui_Label::Blit() {


	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN) {

		text = "Allahu Akbar";
		App->font->CalcSize(text, rect.w, rect.h, font);
		tex = App->font->Print(text, SDL_Color{ 255, 0, 255, 255 }, font);
	}

	App->render->Blit(tex, pos.x, pos.y, &rect, 0.0f); 

}
