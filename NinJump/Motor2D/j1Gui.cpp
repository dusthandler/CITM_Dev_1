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
#include "j1Map.h"

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
	credits_image_tex = App->tex->Load("Maps/Textures/bg_credits.png");
	settings_image_tex = App->tex->Load("Maps/Textures/bg_settings.png");

	                              // the next elements don't have Clean Up
	// fonts
	standard_font = App->font->Load("fonts/shai_fontai/SF_Shai_Fontai.ttf", 36);
	menu_font_2 = App->font->Load("fonts/shai_fontai/SF_Shai_Fontai.ttf", 24);
	title_font = App->font->Load("fonts/shai_fontai/SF_Shai_Fontai_Extended.ttf", 72);
	
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

	


	Menu_Level_GUI_Manager(); 
	Blit(); 
	
	return true; 
}


void j1Gui::Menu_Level_GUI_Manager() {

	if (create_menu_GUI.Do) {        /// CREATE MENU
		if (first_start) {                                              
			create_menu_GUI.next_menu = Next_Menu::MAIN_NEXT;                
			Generate_Menu_GUI();
			// create_menu_GUI.next_menu = Next_Menu::NONE_NEXT;
			create_menu_GUI.Do = false;
			first_start = false;
		}
		else {
			if (App->main_menu->active_menu == Active_Menu::NONE) { // level to menu
				Clean_Level_GUI();
			}
			else {
				Clean_Menu_GUI(App->main_menu->active_menu);  // menu to menu
			}
			Generate_Menu_GUI();
			create_menu_GUI.Do = false;
		}
	}
	else if (create_level_GUI) {         /// CREATE LEVEL

		if (App->main_menu->active_menu != Active_Menu::NONE) {    // menu to level
			Clean_Menu_GUI(App->main_menu->active_menu);
			Restart_Level_Entities_and_Map();
		}
		else {
			Clean_Level_GUI();       // level to level
		}

		App->main_menu->active_menu = Active_Menu::NONE; 
		Generate_Level_GUI();
		create_level_GUI = false;
	}
}

void j1Gui::Restart_Level_Entities_and_Map() {

	App->scene->Map_Loaded = false;        // restart scene map
	App->entity_manager->restart = true;     // restart entities 

}

void j1Gui::Generate_Menu_GUI() {

	Hover_Anim anim_rects;
	anim_rects.a_Idle = { 1, 39, 70, 88 };
	anim_rects.a_Hover = { 71, 31, 180, 115 };
	anim_rects.a_Click = { 1000, 1000, 1000, 1000 };

	if (create_menu_GUI.next_menu == Next_Menu::MAIN_NEXT) {

		// assign menu
		App->main_menu->active_menu = Active_Menu::MAIN; 

		// images
		menu_image = Create_Image(menu_image_tex, iPoint(0, 0), SDL_Rect{ 0, 0, 1050, 965 }, NULL, Menu_Level::Main_Menu);
		menu_label = Create_Image(atlas, iPoint(450, 30), SDL_Rect{ 2, 149, 573, 293 }, NULL, Menu_Level::Main_Menu);


		// buttons
		play_button = Create_Button(anim_rects, atlas, iPoint(570, 40), "play_button", Menu_Level::Main_Menu);
		continue_button = Create_Button(anim_rects, atlas, iPoint(570, 130), "continue_button", Menu_Level::Main_Menu);
		settings_button = Create_Button(anim_rects, atlas, iPoint(570, 220), "settings_button", Menu_Level::Main_Menu);

		anim_rects.a_Idle = { 252, 71, 111, 76 };
		anim_rects.a_Hover = { 364, 41, 113, 111 };
		anim_rects.a_Click = { 1000, 1000, 1000, 1000 };

		credits_button = Create_Button(anim_rects, atlas, iPoint(850, 600), "credits_button", Menu_Level::Main_Menu);
		exit_button = Create_Button(anim_rects, atlas, iPoint(20, 600), "exit_button", Menu_Level::Main_Menu);

		// labels
		game_title = Create_Label(iPoint(100, 400), title_font, "NinJump", NULL, Menu_Level::Main_Menu);

		play_button_label = Create_Label(iPoint(645, 75), standard_font, "PLAY", NULL, Menu_Level::Main_Menu, play_button);
		continue_button_label = Create_Label(iPoint(625, 165), standard_font, "CONTINUE", NULL, Menu_Level::Main_Menu, continue_button);
		settings_button_label = Create_Label(iPoint(628, 255), standard_font, "SETTINGS", NULL, Menu_Level::Main_Menu, settings_button);

		credits_button_label = Create_Label(iPoint(880, 640), menu_font_2, "credits", NULL, Menu_Level::Main_Menu, credits_button);
		exit_button_label = Create_Label(iPoint(60, 640), menu_font_2, "exit", NULL, Menu_Level::Main_Menu, exit_button);
	}


	else if (create_menu_GUI.next_menu == Next_Menu::SETTINGS_NEXT) {

		// assign menu
		App->main_menu->active_menu = Active_Menu::SETTINGS;

		// images
		settings_image = Create_Image(settings_image_tex, iPoint(0, 0), SDL_Rect{ 0, 0, 1050, 817 }, NULL, Menu_Level::Settings_Menu);
		
		// buttons
		settings_to_main_button = Create_Button(anim_rects, atlas, iPoint(50, 600), "settings_to_main_button", Menu_Level::Settings_Menu);

		// labels
		settings_to_main_label = Create_Label(iPoint(125, 640), standard_font, "BACK", NULL, Menu_Level::Settings_Menu, settings_to_main_button);
	}


	else if (create_menu_GUI.next_menu == Next_Menu::CREDITS_NEXT) {

		// assign menu
		App->main_menu->active_menu = Active_Menu::CREDITS;

		// images
		credits_image = Create_Image(credits_image_tex, iPoint(0, 0), SDL_Rect{ 0, 0, 1050, 788 }, NULL, Menu_Level::Credits_Menu);

		// buttons
		credits_to_main_button = Create_Button(anim_rects, atlas, iPoint(50, 600), "credits_to_main_button", Menu_Level::Credits_Menu);

		// labels
		credits_to_main_label = Create_Label(iPoint(125, 640), standard_font, "BACK", NULL, Menu_Level::Credits_Menu, credits_to_main_button);

	}


}


void j1Gui::Generate_Level_GUI() {
	LOG("----------------------------------------------- creating lvl GUI");

	// UI COINS                                            // labels should go after images
	SDL_Rect r = { 0, 0, 32, 32 };
	UI_coin = Create_Image(atlas, iPoint(820, 25), r, NULL, Menu_Level::Level);


	char* ID = "coin_score";
	coin_score = Create_Label(iPoint(860, 30), standard_font, "X0", ID, Menu_Level::Level);


	// UI LIVES 
	ID = "UI_lives";
	r = { 33, 0, 36, 32 };
	UI_lives = Create_Image(atlas, iPoint(940, 25), r, ID, Menu_Level::Level);

	ID = "life_count";
	live_count = Create_Label(iPoint(980, 30), standard_font, "X3", ID, Menu_Level::Level);

}


void j1Gui::Do_Logic_Hovered(j1Gui_Object* object) {

	if (object->type == GUI_TYPE::Button) {
		if (!reset_hover_fx) {
		//	App->audio->PlayFx(1, 0);
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

void j1Gui::Do_Logic_Clicked(j1Gui_Object* object) {        // menu swap TRIGGERS

	// menu buttons 
	if (object->type == GUI_TYPE::Button) {
		LOG("...................................   Clicked button fx   .........");
		App->audio->PlayFx(2, 0); 
	}

	if (object->ID == "play_button") {                                 // go to level

		 if (!App->entity_manager->active && !App->scene->active) {     // first time, entities and scene are not active
			App->scene->Activate();
		}

		App->gui->create_level_GUI = true; 
		App->fade->FadeToBlack(App->main_menu, App->scene, 1.5f);
	}

	else if (object->ID == "exit_button") {       // quit game

		Exit_Quits_App = true;
	}

		else if (object->ID == "settings_button") {                                     // go to settings menu
			if (!App->gui->create_menu_GUI.Do)
				App->gui->create_menu_GUI.Do = true;

			App->gui->create_menu_GUI.next_menu = Next_Menu::SETTINGS_NEXT;
			App->fade->FadeToBlack(App->main_menu, App->main_menu, 1.5f);
		}

		else if (object->ID == "credits_button") {                                       // go to credits menu
			if (!App->gui->create_menu_GUI.Do)
				App->gui->create_menu_GUI.Do = true;

			App->gui->create_menu_GUI.next_menu = Next_Menu::CREDITS_NEXT;
			App->fade->FadeToBlack(App->main_menu, App->main_menu, 1.5f);

		}

		else if (object->ID == "credits_to_main_button" || object->ID == "settings_to_main_button") {                               // go to main from credits
			if (!App->gui->create_menu_GUI.Do)
				App->gui->create_menu_GUI.Do = true;

			App->gui->create_menu_GUI.next_menu = Next_Menu::MAIN_NEXT;
			App->fade->FadeToBlack(App->main_menu, App->main_menu, 1.5f);
		}



}



void j1Gui::Clean_Menu_GUI(Active_Menu acitve_menu){
	
	bool ret = true;
	p2List_item<j1Gui_Object*>* item;
	item = objects.start;

	bool do_it = false; 
	uint counter = 0; 

	for (item = objects.start; item != NULL; item = item->next)
	{
		if (item->data->menu_level == Menu_Level::Main_Menu && acitve_menu == Active_Menu::MAIN) {
			do_it = true; 
		}
		else if (item->data->menu_level == Menu_Level::Settings_Menu && acitve_menu == Active_Menu::SETTINGS) {
			do_it = true;
		}
		else if (item->data->menu_level == Menu_Level::Credits_Menu && acitve_menu == Active_Menu::CREDITS) {
			do_it = true;
		}
	

		if (do_it) {
			counter++; 
			ret = item->data->CleanUp();
			delete item->data;
			item->data = nullptr;
			objects.del(item);
		}
	}
	
	LOG("Free --------------------------------------------------------->   %i gui objs", counter); 
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
	App->tex->UnLoad(credits_image_tex);
	App->tex->UnLoad(settings_image_tex);
	App->tex->UnLoad(atlas); 
	// App->font->CleanUp(); 

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

