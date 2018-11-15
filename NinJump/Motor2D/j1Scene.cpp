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
#include "j1Entity_Manager.h"
#include "j1Enemy_Flying.h"

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
	// --------------------- Create entities here -------------------- 

	j1Enemy_Flying* fly = (j1Enemy_Flying*)App->entity_manager->CreateEntity(Type::ENEMY_FLYING, iPoint(40, 50));

	// we should order this
	
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

	
	
	
	
	if (!App->player->Alive) {
		App->player->Enable(); 
		App->player->Pos.x = 15; //TODO: parche para que el player no desaparezca
		LOG("PLAYER RESPAWNED"); 
	}
	//App->player->Enable();


	
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


	else {
		App->render->camera.y = (int)(App->player->Pos.y - 300) * (-1) * App->win->GetScale();
		App->render->camera.x = (int)(App->player->Pos.x - 300) * (-1) * App->win->GetScale();

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
	
		
	App->map->Draw();
	App->player->Draw();
	


	int x, y;
	App->input->GetMousePosition(x, y);


	PERF_PEEK(ptimer);           // update takes more than 100 ms :/


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
	App->audio->UnloadMus(mus2); 
	return true;
}


bool j1Scene::MapSwap(int SwitchM)
{
	bool ret = true;
	App->player->Disable();
	if (SwitchM == 0)
	{
		App->fade->FadeToBlack(this, this, 0.5f);
		App->collision->CleanWallDeath();
		App->map->CleanUp();
	
		App->map->Load("Level_1.tmx");
		
		Mus_Id = 1; 
		
	}
	else if (SwitchM == 1)
	{
		App->fade->FadeToBlack(this, this, 0.5f);
		App->collision->CleanWallDeath();
		App->map->CleanUp();
	
		App->map->Load("Level_2.tmx");
		Mus_Id = 2; 
	}
	this->SwitchM = SwitchM;
	LOG("Player Dead! x: %i", App->player->Pos.x);

	App->player->Enable();  // enable player after swapping maps

	return ret;
}