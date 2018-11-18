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
	// Create walkability map 
/*
	uchar* flag = NULL;
	int w, h; 
	
	if (App->map->CreateWalkabilityMap(w, h, &flag)) {
		App->pathfinding->SetMap(w,h, flag);
		RELEASE_ARRAY(flag);
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
		LOG("---------------------------LVEL 1 LOADED------------------------");
		Map_Loaded = true;
	}

	
	
	
	//if (!App->player->Alive) {
	//	App->player->Enable(); 
	//	App->player->Pos.x = 15; //TODO: parche para que el player no desaparezca
	//	LOG("PLAYER RESPAWNED"); 
	//}
	////App->player->Enable();


	
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
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene Update", Profiler::Color::LightSteelBlue);
	PERF_START(ptimer); 

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 40;

	else if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 40;

	else if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 40;

	else if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 40;
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN || Player_Win) {
		/*Disable();*/
		/*App->player->Disable(); */          // disable player before swapping maps
		MapSwap(1);
		Player_Win = false;
		

	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN || !Player_Alive) {
		/*Disable();*/
		/*App->player->Disable();    */       // disable player before swapping maps
		MapSwap(0);
		

	}

	

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
	
	// draw everything
		
	App->map->Draw();

	App->entity_manager->Draw(); 


	int x, y;
	App->input->GetMousePosition(x, y);


	PERF_PEEK(ptimer);           // update takes more than 100 ms :/


	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("Scene PostUpdate", Profiler::Color::LightYellow);
	bool ret = true;
	Player_act_pos = App->entity_manager->GetPlayerPos();

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	BROFILER_CATEGORY("Scene CleanUp", Profiler::Color::Lime);
	LOG("Freeing scene");
	App->audio->UnloadMus(mus);                        // Clean Up Music
	App->audio->UnloadMus(mus2); 
	return true;
}


bool j1Scene::MapSwap(int Mapsw)
{
	BROFILER_CATEGORY("Scene MapSwap", Profiler::Color::LimeGreen);
	bool ret = true;
	
	App->entity_manager->CleanUp();
	
	
	if (Mapsw == 0)
	{
		App->fade->FadeToBlack(this, this, 0.5f);
		App->collision->CleanWallDeath();
		App->map->CleanUp();
	
		App->map->Load("Level_1.tmx");
		this->SwitchM = 0;
		
		Mus_Id = 1; 
		
	}
	else if (Mapsw == 1)
	{
		App->fade->FadeToBlack(this, this, 0.5f);
		App->collision->CleanWallDeath();
		App->map->CleanUp();
	
		App->map->Load("Level_2.tmx");
		Mus_Id = 2; 
		this->SwitchM = 1;
	}
	
	// enable player after swapping maps
	
	App->entity_manager->restart = true;

	


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