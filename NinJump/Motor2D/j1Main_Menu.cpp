#include "j1Main_Menu.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Gui.h"
#include "p2Log.h"

#include "Brofiler/Brofiler.h"

j1Main_Menu::j1Main_Menu() : j1Module()
{
	name.create("main menu");

	
}

// Destructor
j1Main_Menu::~j1Main_Menu()
{}



// Called before the first frame
bool j1Main_Menu::Start()
{
	BROFILER_CATEGORY("Main Menu Start", Profiler::Color::PaleGreen);
   
	LOG("------------------------------------------- MAIN MENU START ---------------------");

	if (active_menu == Active_Menu::MAIN) {
		mus = App->audio->LoadMus("Sound/Music/main_menu.ogg");
		App->audio->PlayMus(mus);
	}
	else if (active_menu == Active_Menu::SETTINGS) {

	}
	else if (active_menu == Active_Menu::CREDITS) {

	}
	App->gui->create_menu_GUI.Do = true;

	return true;
}


// Called before quitting
bool j1Main_Menu::CleanUp()
{
	BROFILER_CATEGORY("Main Menu CleanUp", Profiler::Color::PeachPuff);

	App->audio->UnloadMus(mus);

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