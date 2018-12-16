#include "p2Defs.h"
#include "p2Log.h"
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
#include "j1PathFinding.h"
#include "j1Gui.h"
#include "j1Main_Menu.h"

#include "Brofiler/Brofiler.h"
j1Scene::j1Scene() : j1Module()
{
	name.create("scene");

 
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	BROFILER_CATEGORY("Scene Awake", Profiler::Color::LightSkyBlue);
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	BROFILER_CATEGORY("Scene Start", Profiler::Color::LightSlateGray);
	debug_tex = App->tex->Load("maps/Textures/path_tex.png");
	// Create walkability map 
/*
	uchar* flag = NULL;
	int w, h; 
	
	if (App->map->CreateWalkabilityMap(w, h, &flag)) {
		App->pathfinding->SetMap(w,h, flag);
		RELEASE_ARRAY(flag);
	}*/
	LOG("------------------------------------------- SCENE START ---------------------"); 

	/*App->main_menu->active_menu = Active_Menu::NONE; 
	
	App->gui->create_menu_GUI.Do = false; */

	App->gui->create_level_GUI = true;

	if (!App->entity_manager->active) {    // first time, init entity manager
		App->entity_manager->Activate();
	}
	/*else if(!App->entity_manager->IsEnabled()){
		App->entity_manager->Start();        // start entites 
		App->scene->Map_Loaded = false; 
	}*/

	// load info
	mus = App->audio->LoadMus("Sound/Music/level_1.ogg");    
	mus2 = App->audio->LoadMus("Sound/Music/level_2.ogg");

	if (Mus_1) {
		App->audio->PlayMus(mus);
		Mus_1 = false; 
	}
	else {
		if (Mus_Id == 1) {
			App->audio->PlayMus(mus);
		}
		else {
			App->audio->PlayMus(mus2);
		}

	}

	if (!Map_Loaded) {
		App->map->Load("Level_1.tmx");
		map_active = 0; 
		LOG("---------------------------LVEL 1 LOADED------------------------");
		Map_Loaded = true;
		/*int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);*/
	}


	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);

	RELEASE_ARRAY(data);

	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	if (Restart && Second_Start) {
		App->LoadGame("save_game.xml");
		Restart = false;
		Second_Start = false;
	}
	
	Second_Start = true;



	/*static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}*/
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene Update", Profiler::Color::LightSteelBlue);
	PERF_START(ptimer); 

	if (dt != 0) {
		if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) {  // f5
			App->LoadGame("save_game.xml");
			LOG("::::::::::::::::::::::::   Game loaded");
		}

		if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN) { // f6
			App->SaveGame("save_game.xml");
			LOG("::::::::::::::::::::::::   Game saved");
		}

		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			App->render->camera.y += 40;

		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			App->render->camera.y -= 40;

		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			App->render->camera.x += 40;

		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			App->render->camera.x -= 40;

		else {

			App->render->camera.y = (int)(Player_act_pos.y - 300) * (-1) * App->win->GetScale();
			App->render->camera.x = (int)(Player_act_pos.x - 300) * (-1) * App->win->GetScale();

			if (App->render->camera.y <= -310) { //Bottom Limit
				App->render->camera.y = -320;
			}

			else if (App->render->camera.y >= 0) { //Top Limit
				App->render->camera.y = 0;
			}
			if (App->render->camera.x >= 0) { //Left Limit
				App->render->camera.x = 0;
			}
			else if (App->render->camera.x <= -4401) { //Right Limit
				App->render->camera.x = -4401;
			}

		}


		if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN || Player_Win) {
			/*Disable();*/
			/*App->player->Disable(); */          // disable player before swapping maps
			MapSwap(1);
			Player_Win = false;


		}

		else if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN || !Player_Alive) {      // if playeralive, this condition is repeated all the time. 
			/*Disable();*/
			/*App->player->Disable();    */       // disable player before swapping maps
			if (map_active == 0) {
				MapSwap(0);
			}
			else {
				MapSwap(1);
			}

		}

		else if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN || App->entity_manager->player_live_count == 0) {
			MapSwap(2);
		    App->entity_manager->player_live_count = 3;
		}

		else if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN ) {

			if (!App->gui->create_menu_GUI.Do)
				App->gui->create_menu_GUI.Do = true;

			App->gui->create_menu_GUI.next_menu = Next_Menu::SETTINGS_NEXT;
			App->gui->settings_from_level = true; 	                 // flag for settings variant 2

			game_paused = true;
		}

		// draw everything
		if (Map_Loaded) {
			App->map->Draw();
		}
		

		App->entity_manager->Draw();


		int x, y;
		App->input->GetMousePosition(x, y);

		//if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		//	App->LoadGame("save_game.xml");

		//if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		//	App->SaveGame("save_game.xml");

		//// TODO 6: Make the camera movement independent of framerate
		//if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		//	App->render->camera.y += 20;

		//if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		//	App->render->camera.y -= 20;

		//if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		//	App->render->camera.x += 20;

		//if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		//	App->render->camera.x -= 20;

		//App->map->Draw();

		//int x, y;
		//App->input->GetMousePosition(x, y);
		//iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
		//p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
		//	App->map->data.width, App->map->data.height,
		//	App->map->data.tile_width, App->map->data.tile_height,
		//	App->map->data.tilesets.count(),
		//	map_coordinates.x, map_coordinates.y);

		////App->win->SetTitle(title.GetString());

		//// Debug pathfinding ------------------------------
		////int x, y;
		//App->input->GetMousePosition(x, y);
		//iPoint p = App->render->ScreenToWorld(x, y);
		//p = App->map->WorldToMap(p.x, p.y);
		//p = App->map->MapToWorld(p.x, p.y);

		//App->render->Blit(debug_tex, p.x, p.y);

		//const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

		//for (uint i = 0; i < path->Count(); ++i)
		//{
		//	iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		//	App->render->Blit(debug_tex, pos.x, pos.y);
		//}
	}
	PERF_PEEK(ptimer);           // update takes more than 100 ms :/


	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("Scene PostUpdate", Profiler::Color::LightYellow);
	bool ret = true;
	Player_act_pos = App->entity_manager->GetPlayerPos();


/*	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;*/

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	BROFILER_CATEGORY("Scene CleanUp", Profiler::Color::Lime);
	LOG("Freeing scene");

	App->pathfinding->CleanUp(); 

	App->audio->UnloadMus(mus);                        // Clean Up Music
	App->audio->UnloadMus(mus2); 
	return true;
}


bool j1Scene::MapSwap(int Mapsw,bool init)                        // Method to handle level cleaning and Fades
{
	BROFILER_CATEGORY("Scene MapSwap", Profiler::Color::LimeGreen);
	bool ret = true;
	LOG(".................... map swap ................. map swap .................... map swap "); 
	App->entity_manager->CleanUp();
	
	
	if (Mapsw == 0)                                           
	{
		App->fade->FadeToBlack(this, this, 0.5f);
		App->collision->CleanWallDeath();
		App->map->CleanUp();
	
		App->map->Load("Level_1.tmx");
		map_active = 0; 
		this->SwitchM = 0;
		Mus_Id = 1; 
		
		if (!init) {
			App->entity_manager->restart = true;
		}
		// App->entity_manager->restart = true;
	}
	else if (Mapsw == 1)
	{
		App->fade->FadeToBlack(this, this, 0.5f);
		App->collision->CleanWallDeath();
		App->map->CleanUp();
	
		App->map->Load("Level_2.tmx");
		map_active = 1;
		Mus_Id = 2; 
		this->SwitchM = 1;
		
        // App->entity_manager->restart = true;

		if (!init) {
			App->entity_manager->restart = true;
		}
	}

	else if (Mapsw == 2) {     
		
		if(!App->gui->create_menu_GUI.Do)
		App->gui->create_menu_GUI.Do = true; 

		App->gui->create_menu_GUI.next_menu = Next_Menu::MAIN_NEXT; 

		App->fade->FadeToBlack(this, App->main_menu, 0.1f);
		

		App->collision->CleanWallDeath();                           // from level to main menu (press 'M')
		App->map->CleanUp();
		App->scene->Map_Loaded = false;

		// do not restart entities now, menu is active: 
		// restart when swapping to level again

	}


	App->gui->first = false;

	

	
	if (!active) {
		App->scene->Activate();
	}


	return ret;
}


bool j1Scene::Load(pugi::xml_node& node) {
	bool ret = true;



	SwitchM = node.child("Scene_vars").attribute("Switch").as_int(0);
	Player_Alive = node.child("Scene_vars").attribute("Alive").as_bool(true);
	Player_Win = node.child("Scene_vars").attribute("Win").as_bool(false);
	Restart = true;
	MapSwap(SwitchM);
	


	return ret;
}






bool j1Scene::Save(pugi::xml_node& node) const
{
	bool ret = true;

	pugi::xml_node posi = node.append_child("Scene_vars");

	posi.append_attribute("Switch") = SwitchM;
	posi.append_attribute("Alive") = Player_Alive;
	posi.append_attribute("Win") = Player_Win;

	return ret;
}