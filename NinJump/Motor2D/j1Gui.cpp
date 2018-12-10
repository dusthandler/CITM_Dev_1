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
	SDL_Rect r = { 0, 0, 32, 32 };
	UI_coin = Create_Image(atlas, iPoint(820, 25), r);

	_TTF_Font* font = App->font->Load("fonts/open_sans/OpenSans-Bold.ttf", 36);
	char* ID = "coin_score"; 
	coin_score = Create_Label(iPoint(860, 15), font, "X0", ID);


	// UI LIVES 
	ID = "UI_lives"; 
    r = { 33, 0, 36, 32 };
	UI_lives = Create_Image(atlas, iPoint(940, 25), r, ID);

	ID = "life_count";
	live_count = Create_Label(iPoint(980, 15), font, "X3", ID);

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	Select_Clicked_Object(); 
	return true;
}

bool j1Gui::Update(float dt) {
	Blit(); 
	
	return true; 
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

j1Gui_Image* j1Gui::Create_Image(SDL_Texture* tex, iPoint pos, SDL_Rect& atlas_rect, char* ID) {

     j1Gui_Image* ret = new j1Gui_Image(tex, pos, atlas_rect, ID);

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


void j1Gui::Select_Clicked_Object() {

	int x, y; 
	p2List<j1Gui_Object*> selected_objects;
	

		App->input->GetMousePosition(x, y); 

		p2List_item<j1Gui_Object*>* item;
		item = objects.start;

		SDL_Rect obj_r; 

		for (item = objects.start; item != NULL; item = item->next)
		{
			
			obj_r.x = item->data->pos.x;
			obj_r.y = item->data->pos.y;
			obj_r.w = item->data->rect.w;
			obj_r.h = item->data->rect.h;

			// LOG("Mouse coordinates ----> %i, %i", x, y); 
			// LOG("Object coordinates ----> (%i, %i) , (%i %i)", obj_r.x, obj_r.y, obj_r.x + obj_r.w, obj_r.y + obj_r.h); 

			if (x > obj_r.x && x < obj_r.x + obj_r.w && y > obj_r.y && y < obj_r.y + obj_r.h) {
				
				switch (item->data->hover_state) {

				case Hover_State::OUTSIDE:
				    LOG("_____________________________________________________________outside    1");
					item->data->hover_state = Hover_State::HOVER;
					break; 

				case Hover_State::HOVER:
					LOG("_____________________________________________________________hover"); 


			

					if (App->input->GetMouseButtonDown(1)) {
						LOG("________________________________________________________next should be click"); 
						item->data->hover_state = Hover_State::CLICK;
					}


					break;

				case Hover_State::CLICK:
					LOG("_____________________________________________________________click");

					if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
						item->data->hover_state = Hover_State::HOVER;
					}

                    else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
						item->data->hover_state = Hover_State::DRAG;
					}

					// this should be at "drag" state, but does not work right now

					break;

				case Hover_State::DRAG:

					move_object = true;

					if (item->data != nullptr) {
						clicked_object = item->data;
					}

					if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
						move_object = false;
						item->data->hover_state = Hover_State::HOVER;
					}

					LOG("_____________________________________________________________drag");
					break;

				}

				
				
			

			}
		   /*else {
				LOG("_____________________________________________________________outside");
				item->data->hover_state = Hover_State::OUTSIDE;
			}*/

			
		}
		
		
		if (move_object && clicked_object != nullptr) {


			Move_Clicked_Object(clicked_object); 


		}  
			


	

	
	/*p2List_item<j1Gui_Object*>* item_s;
	j1Gui_Object* resolve_selected; 

	int index = 666;

	for (item_s = selected_objects.start; item_s != NULL; item_s = item_s->next)
	{
		

		if (item_s->data->hierarchy < item_s->prev->data->hierarchy) {
			
			resolve_selected = item->data; 

		}

	

	}*/

	// resolve_selected = selected_objects.At(index)->data;


}


j1Gui_Object* j1Gui::Get_Clicked_Object() {

	return clicked_object;
}


void j1Gui::Move_Clicked_Object(j1Gui_Object* obj) {

	LOG("Moving obj yeheaaaaaaaaaaaaaaaaaaaaaaaaaaa"); 

	App->input->GetCenteredMousePosition(obj->pos.x, obj->pos.y, obj);
}




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

