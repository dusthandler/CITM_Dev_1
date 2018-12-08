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

	// atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	// atlas = App->tex->Load(atlas_file_name.GetString());
	atlas = App->tex->Load("gui/atlas.png");                      // do this properly
	                                                           
	// UI COINS                                            // do this in scene? 
	SDL_Rect r = { 0, 0, 40, 40 };                 
	UI_coin = Create_Image(atlas, iPoint(850, 25), r);

	_TTF_Font* font = App->font->Load("fonts/open_sans/OpenSans-Bold.ttf", 36);
	char* ID = "coin_score"; 
	coin_score = Create_Label(iPoint(890, 25), font, "X0", ID);

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

j1Gui_Image* j1Gui::Create_Image(SDL_Texture* tex, iPoint pos, SDL_Rect& atlas_rect) {

     j1Gui_Image* ret = new j1Gui_Image(tex, pos, atlas_rect);

	 if(ret != nullptr)
     objects.add(ret); 

	 return ret; 
};


j1Gui_Label* j1Gui::Create_Label(iPoint pos, _TTF_Font* font, char* text, char* ID) {
	
	j1Gui_Label* ret = new j1Gui_Label(pos, font, text, ID);

	if (ret != nullptr)
	objects.add(ret); 

	return ret;
}; 

/*
void j1Gui::Delete_Object() {




};*/


void j1Gui::Blit(){

	p2List_item<j1Gui_Object*>* item;
	item = objects.start;


	for (item = objects.start; item != NULL; item = item->next)
	{
		   
		 item->data->Blit(); 
	}

};

// Called before quitting
bool j1Gui::CleanUp()
{                                    // as entity manager
	LOG("Freeing GUI");
	bool ret = true;
	p2List_item<j1Gui_Object*>* item;
	item = objects.start;

	for (item = objects.start; item != NULL; item = item->next)
	{
		ret = item->data->CleanUp();
		delete item->data;
		item->data = nullptr;

	}
	objects.clear(); 


	App->tex->UnLoad(atlas); 
//	App->font->CleanUp(); 


	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

