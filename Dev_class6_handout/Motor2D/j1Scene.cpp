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
#include "j1Player.h"
#include "j1Player.h"
#include "j1FadeBlack.h"
#include "j1Collision.h"


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
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	
	mus = App->audio->LoadMus("Sound/Music/level_1.ogg");    
	App->audio->PlayMus(mus); 

	if (!Map_Loaded) {
		App->map->Load("Level_1.tmx");
		LOG("---------------------------LVEL 1 LOADED------------------------");
		Map_Loaded = true; 
		SwitchM = 0;
	}
	
	if (!App->player->Alive) {
		App->player->Enable(); 
		LOG("PLAYER RESPAWNED"); 
	}

	//App->map->Load("iso.tmx");
	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 40;

	else if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 40;

	else if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 40;

	else if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 40;


	else {
		App->render->camera.x = (int)(App->player->Pos.x - 300) * (-1) * App->win->GetScale();
		App->render->camera.y = (int)(App->player->Pos.y - 300) * (-1) * App->win->GetScale();
	}
	/*if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) {
		App->fade->FadeToBlack(App->scene, App->scene, 2.5f);
	};*/
		
	
	App->map->Draw();
	App->player->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	//iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	/*p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);*/

	/*App->win->SetTitle(title.GetString());*/
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->audio->UnloadMus(mus);                        // Clean Up Music
	return true;
}



bool j1Scene::MapSwap(int SwitchM)
{
	bool ret = true;

	if (SwitchM == 0)
	{
		App->fade->FadeToBlack(this, this, 0.5f);
		
		App->map->CleanUp();
		App->collision->CleanWallDeath();
		App->map->Load("Level_1.tmx");
		
	}
	else if (SwitchM == 1)
	{
		App->fade->FadeToBlack(this, this, 0.5f);
		
		App->map->CleanUp();
		App->collision->CleanWallDeath();
		App->map->Load("Level_2.tmx");
	}
	this->SwitchM = SwitchM;

	return ret;
}