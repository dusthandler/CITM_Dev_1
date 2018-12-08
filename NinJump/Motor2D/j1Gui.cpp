#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Gui_Label.h"
#include "j1Gui_Image.h"
#include "j1Gui_Object.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	SDL_Rect r = { 485, 830, 328, 101 };                 // spaceship in atlas
	Create_Image(atlas, iPoint(500, 100), r); 

	_TTF_Font* font = App->font->Load("fonts/open_sans/OpenSans-Bold.ttf", 12);
	Create_Label(iPoint(500, 50), font, "Hello World");

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

bool j1Gui::Update(float dt) {
	Blit(); 
	LOG("GUI update ... ... ..."); 
	return true; 
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

void j1Gui::Create_Image(SDL_Texture* tex, iPoint pos, SDL_Rect& atlas_rect) {

     j1Gui_Image* ret = new j1Gui_Image(tex, pos, atlas_rect);
     objects.add(ret); 

};


void j1Gui::Create_Label(iPoint pos, _TTF_Font* font, char* text) {
	
	j1Gui_Label* ret = new j1Gui_Label(pos, font, text);
	objects.add(ret); 

}; 

/*
void j1Gui::Delete_Object() {




};*/

void j1Gui::Blit(){

	p2List_item<j1Gui_Object*>* item;
	item = objects.start;


	for (item = objects.start; item != NULL; item = item->next)
	{
		   
		/*	App->render->Blit(atlas, item->data->pos.x, item->data->pos.y, &item->data->rect, 1);
			LOG("Printing GUI object !!!"); */
		item->data->Blit(); 
	}

};

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

