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
#include <string.h>

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

	// credits file 
	
	


	/*FILE *f = fopen("gui/prueba.txt", "r");

    char line[666]; 
	char* result[50]; 
	uint i = 0; 
	while ((fgets(line, sizeof(line), f) != NULL)) {
		char buffer[sizeof(line)];
		char* val1 = strtok(line, ",");
		char* val2 = strtok(NULL, ",");
		strcpy(buffer, val1);
		result[i] = buffer; 
		LOG("line is ----------------------------------> %s", result[i]);

		i++; 
	}
		
	LOG("file is ----------------------------------> %s", result);

	fclose(f); */
	
	



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

	
	LOG("                                                          actual DT                   %f", dt); 
	LOG(" 888888888888888888888888888888888888888888888888888888     CREATED LEVEL ----> %i times", creation_level_times);
	LOG(" 888888888888888888888888888888888888888888888888888888     CLEANED LEVEL ----> %i times", cleaned_level_times); 

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

				if (!settings_from_level) {          // when you come from level and go to settings, map must not clean up
					App->map->CleanUp();
				}
					Clean_Level_GUI();    // should this be cleaned ? it mmust preserve the state of the gameplay
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

	/*App->scene->Map_Loaded = false;        // restart scene map
    App->entity_manager->restart = true;     // restart entities */

	App->entity_manager->Start();      // start entites 
	
}

void j1Gui::Generate_Menu_GUI() {

	Hover_Anim anim_rects;
	anim_rects.a_Idle = { 1, 39, 70, 88 };
	anim_rects.a_Hover_light = { 525, 33, 129, 155 };
	anim_rects.a_Hover = { 71, 31, 180, 115 };


	if (create_menu_GUI.next_menu == Next_Menu::MAIN_NEXT) {

		// assign menu
		App->main_menu->active_menu = Active_Menu::MAIN; 

		// images
		menu_image = Create_Image(menu_image_tex, iPoint(0, 0), SDL_Rect{ 0, 0, 1050, 965 }, NULL, Menu_Level::Main_Menu);
		menu_label = Create_Image(atlas, iPoint(450, 30), SDL_Rect{ 2, 149, 573, 293 }, NULL, Menu_Level::Main_Menu, nullptr, true);


		// buttons
		play_button = Create_Button(anim_rects, atlas, iPoint(570, 40), "play_button", Menu_Level::Main_Menu, menu_label);
		continue_button = Create_Button(anim_rects, atlas, iPoint(570, 130), "continue_button", Menu_Level::Main_Menu, menu_label);
		settings_button = Create_Button(anim_rects, atlas, iPoint(570, 220), "settings_button", Menu_Level::Main_Menu, menu_label);

		anim_rects.a_Idle = { 252, 71, 111, 76 };
		anim_rects.a_Hover_light = { 680, 20, 219, 161 };
		anim_rects.a_Hover = { 364, 41, 113, 111 };
		

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
		
		if (!settings_from_level) {
			// buttons
			settings_to_main_button = Create_Button(anim_rects, atlas, iPoint(50, 600), "settings_to_main_button", Menu_Level::Settings_Menu);

			// labels
			settings_to_main_label = Create_Label(iPoint(125, 640), standard_font, "BACK", NULL, Menu_Level::Settings_Menu, settings_to_main_button);
		}
		else {
			// buttons
			settings_to_level_button = Create_Button(anim_rects, atlas, iPoint(50, 600), "settings_to_level_button", Menu_Level::Settings_Menu);

			// labels
			settings_to_level_label = Create_Label(iPoint(125, 640), standard_font, "RESUME", NULL, Menu_Level::Settings_Menu, settings_to_level_button);
		}
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
		
		// credits_label = Create_Label(iPoint(125, 100), standard_font, line, NULL, Menu_Level::Credits_Menu);

	}

	if(create_menu_GUI.Do)
	create_menu_GUI.Do = false;

}


void j1Gui::Generate_Level_GUI() {
	LOG("----------------------------------------------- creating lvl GUI");

	creation_level_times++; 
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


	// UI TIME 
	ID = "time_count";
	r = { 50, 0, 36, 32 };
	time_count = Create_Label(iPoint(30, 30), standard_font, "GO! 0", ID, Menu_Level::Level);


	if(create_level_GUI)
	create_level_GUI = false; 

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

	

		App->gui->create_level_GUI = true; 
		App->scene->MapSwap(0,first);
	}

	else if (object->ID == "exit_button") {       // quit game

		Exit_Quits_App = true;
	}

		else if (object->ID == "settings_button") {                                     // go to settings menu
			if (!App->gui->create_menu_GUI.Do)
				App->gui->create_menu_GUI.Do = true;

			App->gui->create_menu_GUI.next_menu = Next_Menu::SETTINGS_NEXT;
			settings_from_level = false;                         // flag for settings variant 1 
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


		else if (object->ID == "settings_to_level_button") {                               // go to main from credits
		App->gui->create_level_GUI = true;
		App->scene->game_paused = false;                                      // restart dt 

	}

	

}



void j1Gui::Clean_Menu_GUI(Active_Menu acitve_menu){

	cleaned_level_times++; 
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
	
	cleaned_times++; 
	LOG("Free --------------------------------------------------------->   %i gui objs", counter); 
	LOG("Cleaned --------------------------------------------------------->   %i times", cleaned_times);

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
			objects.del(item);
		}
		
	}


}


// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

j1Gui_Image* j1Gui::Create_Image(SDL_Texture* tex, iPoint pos, SDL_Rect& atlas_rect, char* ID, Menu_Level menu_level, j1Gui_Object* parent, bool draggable) {

     j1Gui_Image* ret = new j1Gui_Image(tex, pos, atlas_rect, ID, menu_level, parent, draggable);

	 if(ret != nullptr)
     objects.add(ret); 

	 return ret; 
};


j1Gui_Label* j1Gui::Create_Label(iPoint pos, _TTF_Font* font, char* text, char* ID, Menu_Level menu_level, j1Gui_Object* parent, bool draggable) {
	
	j1Gui_Label* ret = new j1Gui_Label(pos, font, text, ID, menu_level, parent, draggable);

	if (ret != nullptr)
	objects.add(ret); 

	return ret;
}; 


j1Gui_Button* j1Gui::Create_Button(Hover_Anim& hover_rects, SDL_Texture* tex, iPoint pos, char* ID, Menu_Level menu_level, j1Gui_Object* parent, bool draggable) {

	j1Gui_Button* ret = new j1Gui_Button(hover_rects, tex, pos, ID, menu_level, parent, draggable);

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


		/*p2List_item<j1Gui_Object*>* item_c;
		p2List<j1Gui_Object*> childs;
		uint child_count = 0;*/


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

					if (clicked_object != nullptr) {   
						reset_child_search = false;
					}
					break;


				case Hover_State::CLICK:

				    if (!reset_child_search) {

						/*for (item_c = objects.start; item_c != NULL; item_c = item_c->next)
						{

							if (item_c->data->parent == clicked_object) {
								LOG("_________________________________________________________________ - child of clicked obj detected");
								childs.add(item_c->data);
								child_count++;
							}

						}*/
						reset_child_search = true;
					}


					if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
						item->data->hover_state = Hover_State::HOVER;
					}

                    else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
						item->data->hover_state = Hover_State::DRAG;
						/*if (move_object && clicked_object != nullptr &&  && child_count > 0) {*/
						 //, childs);
						/*}*/
					}
					break;

				case Hover_State::DRAG:

					if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
						item->data->hover_state = Hover_State::HOVER;
					}
					if (!cancer) {
						last_mouse_pos = App->input->GetActualMousePosition();
						cancer = true;
					}
					/*move_object = true;
*/
					/*if (App->main_menu->active_menu == Active_Menu::SETTINGS && settings_from_level && item->data->menu_level == Menu_Level::Level) { // do not select an active level object when you are in settings){
						continue; 
					}*/
						if (item->data != nullptr ) {
						clicked_object = item->data;
					}

					if (/*clicked_object->menu_level != Menu_Level::Level &&*/ clicked_object->draggable) {
						Move_Clicked_Object(clicked_object);
					}
					

					break;

				}
			}
		   else {
				item->data->hover_state = Hover_State::OUTSIDE;
			}

		}
		
		if(clicked_object != nullptr)
		LOG("Clicked object is actually ---------------------------------------> %s", clicked_object->ID);

		



			


		


	
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


void j1Gui::Move_Clicked_Object(j1Gui_Object* obj) { //, p2List<j1Gui_Object*> childs) {


	// LOG(" *******************************   moving obj   *******************************"); 


	//App->input->GetMousePosition(obj->pos.x, obj->pos.y);

//	App->input->GetMousePosition(obj->pos.x, obj->pos.y);


	/*p2List_item<j1Gui_Object*>* item_c;
	for (item_c = childs.start; item_c != NULL; item_c = item_c->next)
	{
		App->input->GetMousePosition(item_c->data->pos.x, item_c->data->pos.y);
	}; */
	

	





	iPoint obj_pos = obj->Get_Pos();
	mouse_pos = App->input->GetActualMousePosition(); 



	
	iPoint dist; 
	dist.x = mouse_pos.x - obj_pos.x;
	dist.y = mouse_pos.y - obj_pos.y;

	iPoint new_pos; 
	new_pos.x = last_mouse_pos.x - (dist.x);
	new_pos.y = last_mouse_pos.y - (dist.y);

	obj->Set_Pos(new_pos.x, new_pos.y);


	LOG("mouse pos ________________________________________________________ %i %i", mouse_pos.x, mouse_pos.y);
	LOG("obj pos ________________________________________________________ %i %i", obj_pos.x, obj_pos.y);

	
	cancer = false;

	LOG("New object pos ________________________________________________________ %i %i", new_pos.x, new_pos.y);



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

