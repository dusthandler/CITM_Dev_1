#include "j1Main_Menu.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1FadeBlack.h"
#include "j1Gui_Button.h"
#include "j1Gui_Image.h"
#include "j1Gui_Slider.h"
#include "SDL/include/SDL.h"
#include "p2Log.h"

#include "Brofiler/Brofiler.h"

j1Main_Menu::j1Main_Menu() : j1Module()
{
	name.create("mainmenu");

	
}

// Destructor
j1Main_Menu::~j1Main_Menu()
{}



// Called before the first frame
bool j1Main_Menu::Start()
{
	BROFILER_CATEGORY("Menu Start", Profiler::Color::PaleGreen);
   
	LOG("------------------------------------------- MENU START ---------------------");
	mus = App->audio->LoadMus("Sound/Music/main_menu.ogg");
	mus_set = App->audio->LoadMus("Sound/Music/settings_mus.ogg");
	mus_cred = App->audio->LoadMus("Sound/Music/credits_mus.ogg");

	if (active_menu == Active_Menu::MAIN) {
		App->audio->PlayMus(mus);
	}

	else if (active_menu == Active_Menu::SETTINGS) {
		App->audio->PlayMus(mus_set);
	}

	else if (active_menu == Active_Menu::CREDITS) {
		App->audio->PlayMus(mus_cred);
	}

	if(!App->gui->create_menu_GUI.Do)
	App->gui->create_menu_GUI.Do = true;

	


	return true;
}

bool j1Main_Menu::Update(float dt)
{
	
	bool ret = true;
	
	return ret;
}


bool j1Main_Menu::PostUpdate() {
	bool ret = true;
	App->gui->play_button->ClickFunction = &Clicked_Play;
	App->gui->continue_button->ClickFunction = &Clicked_Continue;
	App->gui->exit_button->ClickFunction = &Clicked_Exit;
	App->gui->settings_button->ClickFunction = &Clicked_Settings;
	App->gui->credits_button->ClickFunction = &Clicked_Credits;
	if (App->gui->create_menu_GUI.next_menu == Next_Menu::SETTINGS_NEXT) {
		
		App->gui->settings_to_main_button->ClickFunction = &Return_to_Main;
		App->gui->settings_fx_slider->MoveSlider = &Move_Slider;
		App->gui->settings_mus_slider->MoveSlider = &Move_Slider;
	}
	if (App->gui->create_menu_GUI.next_menu == Next_Menu::CREDITS_NEXT) {

		App->gui->credits_to_main_button->ClickFunction = &Return_to_Main;
	}
	App->gui->menu_label->MoveFunction = &Move_Object;
	
	//App->gui->settings_to_level_button->ClickFunction = &Clicked_Pause;

	
	return ret;
}
// Called before quitting
bool j1Main_Menu::CleanUp()
{
	BROFILER_CATEGORY("Main Menu CleanUp", Profiler::Color::PeachPuff);

	App->audio->UnloadMus(mus);
	App->audio->UnloadMus(mus_cred);
	App->audio->UnloadMus(mus_set);

	return true;
}


bool j1Main_Menu::Load(pugi::xml_node& node) {
	bool ret = true;


	return ret;
}



bool j1Main_Menu::Save(pugi::xml_node& node) const
{
	bool ret = true;


	return ret;
}

void Clicked_Play() {
	App->gui->create_level_GUI = true;
	App->scene->MapSwap(0, App->gui->first); //guillem
}

void Clicked_Continue() {
	App->LoadGame("save_game.xml");
	App->gui->create_level_GUI = true;
}

void Clicked_Exit() {
	App->gui->Exit_Quits_App = true;
}

void Clicked_Settings() {
	if (!App->gui->create_menu_GUI.Do)
		App->gui->create_menu_GUI.Do = true;

	App->gui->create_menu_GUI.next_menu = Next_Menu::SETTINGS_NEXT;
	App->gui->settings_from_level = false;                         // flag for settings variant 1 
	App->fade->FadeToBlack(App->main_menu, App->main_menu, 1.5f);
}

void Clicked_Credits() {
	if (!App->gui->create_menu_GUI.Do)
		App->gui->create_menu_GUI.Do = true;

	App->gui->create_menu_GUI.next_menu = Next_Menu::CREDITS_NEXT;
	App->fade->FadeToBlack(App->main_menu, App->main_menu, 1.5f);
}

void Return_to_Main() {
	if (!App->gui->create_menu_GUI.Do)
		App->gui->create_menu_GUI.Do = true;

	App->gui->create_menu_GUI.next_menu = Next_Menu::MAIN_NEXT;
	App->fade->FadeToBlack(App->main_menu, App->main_menu, 1.5f);
}

void Clicked_Pause() {
	App->gui->create_level_GUI = true;
	App->scene->game_paused = false;
}

void Move_Object(j1Gui_Object* obj) {
	LOG(" *******************************   moving obj   *******************************");

	App->gui->frame_count++;

	if (App->gui->frame_count == 5) {
		App->gui->last_mouse_pos = App->gui->mouse_pos;
		App->gui->frame_count = 0;
	}


	if (App->gui->last_mouse_pos != App->gui->mouse_pos) {

		iPoint obj_pos = obj->Get_Pos();
		App->gui->mouse_pos = App->input->GetActualMousePosition();

		iPoint dist;
		dist.x = App->gui->mouse_pos.x - obj_pos.x;
		dist.y = App->gui->mouse_pos.y - obj_pos.y;

		iPoint dist_last_frame_mouse;
		dist_last_frame_mouse.x = App->gui->mouse_pos.x - App->gui->last_mouse_pos.x;
		dist_last_frame_mouse.y = App->gui->mouse_pos.y - App->gui->last_mouse_pos.y;

		iPoint new_pos;
		new_pos.x = (App->gui->mouse_pos.x) - (dist.x) + dist_last_frame_mouse.x;
		new_pos.y = (App->gui->mouse_pos.y) - (dist.y) + dist_last_frame_mouse.y;


		if (obj->type == GUI_TYPE::Slider) {

			//Move_Slider(obj, new_pos);

		}
		else {
			obj->Set_Pos(new_pos.x, new_pos.y);
		}



	}
}

void Move_Slider(j1Gui_Object* obj, iPoint new_pos) {
	uint right_offset = 48;
	uint left_offset = 5;

	uint start_pos;
	uint end_pos;

	if (obj->parent != nullptr) {
		if (new_pos.x >= obj->parent->pos.x + left_offset && new_pos.x <= obj->parent->pos.x + obj->parent->rect.w - right_offset) {
			obj->pos.x = new_pos.x;
		}
	}

	start_pos = obj->parent->pos.x + left_offset;
	end_pos = obj->parent->pos.x + obj->parent->rect.w - right_offset;

	uint total_range = end_pos - start_pos;
	uint range_volume_factor = total_range / SDL_MIX_MAXVOLUME;

	uint volume = (obj->pos.x - obj->initial_pos.x) / range_volume_factor;


	// music volume

	if (obj->ID == "mus_slider") {

		App->audio->Change_Mus_Volume(volume);

	}

	else if (obj->ID == "fx_slider") {

		App->audio->Change_Fx_Volume(volume);
	}

	LOG("VOLUME is ------------------------------------------------>>>>> %i", volume);
}