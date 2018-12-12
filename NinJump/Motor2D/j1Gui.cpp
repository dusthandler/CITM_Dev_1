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
#include "j1Gui_Button.h"
#include "j1Main_Menu.h"
#include "j1Scene.h"
#include "j1FadeBlack.h"
#include "j1Entity_Manager.h"
#include "j1Audio.h"
#include "j1App.h"

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
	// textures
	// atlas = App->tex->Load(atlas_file_name.GetString());
	atlas = App->tex->Load("gui/atlas.png");                      // do this in tiled
	menu_image_tex = App->tex->Load("Maps/Textures/bg_menu.png");
	menu_font = App->font->Load("fonts/shai_fontai/SF_Shai_Fontai.ttf", 36);
	level_font = App->font->Load("fonts/open_sans/OpenSans-Bold.ttf", 36);
	menu_font_2 = App->font->Load("fonts/shai_fontai/SF_Shai_Fontai.ttf", 24);
	
	// audio
	App->audio->LoadFx("Sound/Fx/button_hover.wav");
	App->audio->LoadFx("Sound/Fx/button_click.wav");



	return true;
}



bool j1Gui::PreUpdate()
{
	Select_Clicked_Object(); 
	Check_Clicked(); 
	return true;
}

bool j1Gui::Update(float dt) {

	
		if (create_menu_GUI) {

			Clean_Level_GUI();
			Generate_Menu_GUI();
			create_menu_GUI = false;
		}
		else if (create_level_GUI) {
			Clean_Menu_GUI();
			Generate_Level_GUI();
			create_level_GUI = false;
		}



	Blit(); 
	
	return true; 
}



void j1Gui::Generate_Menu_GUI() {
	// images

	menu_image = Create_Image(menu_image_tex, iPoint(0, 0), SDL_Rect{ 0, 0, 1050, 965 }, NULL, Menu_Level::Menu);
	menu_label = Create_Image(atlas, iPoint(450, 30), SDL_Rect{ 2, 149, 573, 293 }, NULL, Menu_Level::Menu);




	// buttons

	Hover_Anim anim_rects;
	anim_rects.a_Idle = { 3, 43, 65, 79 };
	anim_rects.a_Hover = { 72, 37, 173, 114 };
	anim_rects.a_Click = { 1000, 1000, 1000, 1000 };

	play_button = Create_Button(anim_rects, atlas, iPoint(570, 40), "play_button", Menu_Level::Menu);
	continue_button = Create_Button(anim_rects, atlas, iPoint(570, 130), "continue_button", Menu_Level::Menu);
	settings_button = Create_Button(anim_rects, atlas, iPoint(570, 220), "settings_button", Menu_Level::Menu);

	anim_rects.a_Idle = { 252, 71, 111, 76 };
	anim_rects.a_Hover = { 364, 41, 113, 111 };
	anim_rects.a_Click = { 1000, 1000, 1000, 1000 };

	credits_button = Create_Button(anim_rects, atlas, iPoint(850, 600), "credits_button", Menu_Level::Menu);
	exit_button = Create_Button(anim_rects, atlas, iPoint(20, 600), "exit_button", Menu_Level::Menu);

	// labels

	play_button_label = Create_Label(iPoint(640, 75), menu_font, "PLAY", NULL, Menu_Level::Menu, play_button);
	continue_button_label = Create_Label(iPoint(615, 165), menu_font, "CONTINUE", NULL, Menu_Level::Menu, continue_button);
	settings_button_label = Create_Label(iPoint(618, 255), menu_font, "SETTINGS", NULL, Menu_Level::Menu, settings_button);

	credits_button_label = Create_Label(iPoint(880, 640), menu_font_2, "credits", NULL, Menu_Level::Menu, credits_button);
	exit_button_label = Create_Label(iPoint(60, 640), menu_font_2, "exit", NULL, Menu_Level::Menu, exit_button);



}


void j1Gui::Generate_Level_GUI() {
	LOG("----------------------------------------------- creating lvl GUI");

	// UI COINS                                            // labels should go after images
	SDL_Rect r = { 0, 0, 32, 32 };
	UI_coin = Create_Image(atlas, iPoint(820, 25), r, NULL, Menu_Level::Level);


	char* ID = "coin_score";
	coin_score = Create_Label(iPoint(860, 15), level_font, "X0", ID, Menu_Level::Level);


	// UI LIVES 
	ID = "UI_lives";
	r = { 33, 0, 36, 32 };
	UI_lives = Create_Image(atlas, iPoint(940, 25), r, ID, Menu_Level::Level);

	ID = "life_count";
	live_count = Create_Label(iPoint(980, 15), level_font, "X3", ID, Menu_Level::Level);

}


void j1Gui::Do_Logic_Hovered(j1Gui_Object* object) {

	if (object->type == GUI_TYPE::Button) {
		if (!reset_hover_fx) {
			App->audio->PlayFx(1, 0);
			reset_hover_fx = true;
		}
	}

}

void j1Gui::Check_Clicked() {

	p2List_item<j1Gui_Object*>* item;
	item = objects.start;

	for (item = objects.start; item != NULL; item = item->next)
	{
		
		if (item->data->menu_level != Menu_Level::Level) {

			if (item->data->hover_state == Hover_State::HOVER) {
				Do_Logic_Hovered(item->data); 
			}
			else if (item->data->hover_state == Hover_State::CLICK) {
				Do_Logic_Clicked(item->data);
			}
		}	
	}

}

void j1Gui::Do_Logic_Clicked(j1Gui_Object* object) {

	// menu buttons 
	if (object->type == GUI_TYPE::Button) {
		App->audio->PlayFx(2, 0); 
	}

	if (object->ID == "play_button") {
		App->entity_manager->Activate();
		App->scene->Activate();
		App->fade->FadeToBlack(App->main_menu, App->scene, 1.5f);
	}

	else if (object->ID == "exit_button") {
		Exit_Quits_App = true; 
	}
}



void j1Gui::Clean_Menu_GUI(){

	bool ret = true;
	p2List_item<j1Gui_Object*>* item;
	item = objects.start;

	for (item = objects.start; item != NULL; item = item->next)
	{
		if (item->data->menu_level == Menu_Level::Menu) {
			ret = item->data->CleanUp();
			delete item->data;
			item->data = nullptr;
		}
		objects.del(item);
	}
	

}

void j1Gui::Clean_Level_GUI() {

	bool ret = true;
	p2List_item<j1Gui_Object*>* item;
	item = objects.start;

	for (item = objects.start; item != NULL; item = item->next)
	{
		if (item->data->menu_level == Menu_Level::Level) {
			ret = item->data->CleanUp();
			delete item->data;
			item->data = nullptr;
		}
		objects.del(item);
	}


}


// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

j1Gui_Image* j1Gui::Create_Image(SDL_Texture* tex, iPoint pos, SDL_Rect& atlas_rect, char* ID, Menu_Level menu_level, j1Gui_Object* parent) {

     j1Gui_Image* ret = new j1Gui_Image(tex, pos, atlas_rect, ID, menu_level, parent);

	 if(ret != nullptr)
     objects.add(ret); 

	 return ret; 
};


j1Gui_Label* j1Gui::Create_Label(iPoint pos, _TTF_Font* font, char* text, char* ID, Menu_Level menu_level, j1Gui_Object* parent) {
	
	j1Gui_Label* ret = new j1Gui_Label(pos, font, text, ID, menu_level, parent);

	if (ret != nullptr)
	objects.add(ret); 

	return ret;
}; 


j1Gui_Button* j1Gui::Create_Button(Hover_Anim& hover_rects, SDL_Texture* tex, iPoint pos, char* ID, Menu_Level menu_level, j1Gui_Object* parent) {

	j1Gui_Button* ret = new j1Gui_Button(hover_rects, tex, pos, ID, menu_level, parent);

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
			// reset button fxs

			if (item->data->hover_state != Hover_State::HOVER && reset_hover_fx == true) {
				reset_hover_fx = false; 
			}

			obj_r.x = item->data->pos.x;
			obj_r.y = item->data->pos.y;
			obj_r.w = item->data->rect.w;
			obj_r.h = item->data->rect.h;

			// check mouse inside object

			if (x > obj_r.x && x < obj_r.x + obj_r.w && y > obj_r.y && y < obj_r.y + obj_r.h) {
				
				switch (item->data->hover_state) {

				case Hover_State::OUTSIDE:
					item->data->hover_state = Hover_State::HOVER;
					break; 

				case Hover_State::HOVER:
				    move_object = false; 
					if (App->input->GetMouseButtonDown(1) == KEY_DOWN && !move_object) {
						item->data->hover_state = Hover_State::CLICK;
					}
					break;

				case Hover_State::CLICK:
					if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
						item->data->hover_state = Hover_State::HOVER;
					}

                    else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
						item->data->hover_state = Hover_State::DRAG;
					}
					break;

				case Hover_State::DRAG:
					if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
						item->data->hover_state = Hover_State::HOVER;
					}

					move_object = true;

					if (item->data != nullptr) {
						clicked_object = item->data;
					}

					break;

				}
			}
		   else {
				item->data->hover_state = Hover_State::OUTSIDE;
			}

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


	LOG(" *******************************   moving obj   *******************************"); 
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
	//	if (App->main_menu->active && item->data->menu_level == Menu_Level::Menu) {
			 item->data->Blit(); 
		// }
		//else if (App->scene->active && item->data->menu_level == Menu_Level::Level) {
		//	item->data->Blit();
	//	}
		
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

	// textures
	App->tex->UnLoad(menu_image_tex);
	App->tex->UnLoad(atlas); 
//	App->font->CleanUp(); 

	// audio
	/*App->audio->UnloadFx(6); 
	App->audio->UnloadFx(7);*/

	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

