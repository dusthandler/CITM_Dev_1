#include "j1Main_Menu.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Gui.h"
#include "j1Input.h"
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

bool j1Main_Menu::Update(float dt) {

	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) {
		App->audio->Change_Mus_Volume(100); 

	}
	return true; 
		
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