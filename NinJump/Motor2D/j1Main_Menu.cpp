#include "p2Defs.h"
#include "p2Log.h"
#include "j1Main_Menu.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1FadeBlack.h"
#include "j1Collision.h"
#include "j1Entity_Manager.h"
#include "j1Player_Entity.h"
#include "j1Gui.h"

#include "Brofiler/Brofiler.h"

j1Main_Menu::j1Main_Menu() : j1Module()
{
	name.create("main menu");

	
}

// Destructor
j1Main_Menu::~j1Main_Menu()
{}

// Called before render is available
bool j1Main_Menu::Awake()
{
	BROFILER_CATEGORY("Main Menu Awake", Profiler::Color::PaleGoldenRod);
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Main_Menu::Start()
{
	BROFILER_CATEGORY("Main Menu Start", Profiler::Color::PaleGreen);


	mus = App->audio->LoadMus("Sound/Music/main_menu.ogg");
	App->audio->PlayMus(mus);

	bg_image_tex = App->tex->Load("Maps/Textures/bg_menu.png");

	bg_image = App->gui->Create_Image(bg_image_tex, iPoint(0, 0), SDL_Rect{ 0, 0, 800, 800 });

	return true;
}

// Called each loop iteration
bool j1Main_Menu::PreUpdate()
{
	BROFILER_CATEGORY("Main Menu PreUpdate", Profiler::Color::PaleTurquoise);

	return true;
}

// Called each loop iteration
bool j1Main_Menu::Update(float dt)
{
	BROFILER_CATEGORY("Main Menu Update", Profiler::Color::PaleVioletRed);

	

	return true;
}

// Called each loop iteration
bool j1Main_Menu::PostUpdate()
{
	BROFILER_CATEGORY("Main Menu Postpdate", Profiler::Color::PapayaWhip); 
	bool ret = true; 
	return ret;
}

// Called before quitting
bool j1Main_Menu::CleanUp()
{
	BROFILER_CATEGORY("Main Menu CleanUp", Profiler::Color::PeachPuff);
	App->audio->UnloadMus(mus);

	if(bg_image_tex != nullptr)
	App->tex->UnLoad(bg_image_tex); 

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