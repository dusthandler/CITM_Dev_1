/*#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene2.h"
#include "j1Player.h"
#include "j1Player.h"
#include "j1FadeBlack.h"
#include "j1Scene.h"

j1Scene2::j1Scene2() : j1Module()
{
	name.create("scene");
	

}

// Destructor
j1Scene2::~j1Scene2()
{}

// Called before render is available
bool j1Scene2::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene2::Start()
{
	

	if (!Map_Loaded) {
		App->map->Load("Level_2.tmx");
		LOG("---------------------------CANCER");
		Map_Loaded = true;
	}

	if (!App->player->Alive) {
		App->player->Enable();
		LOG("PLAYER RESPAWNED");
	}

	//App->map->Load("iso.tmx");

	return true;
}

// Called each loop iteration
bool j1Scene2::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene2::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 40;

	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 40;

	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 40;

	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 40;


	else {
		App->render->camera.x = (int)(App->player->Pos.x - 300) * (-1) * App->win->GetScale();
		App->render->camera.y = (int)(App->player->Pos.y - 300) * (-1) * App->win->GetScale();
	}
	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) {
		App->fade->FadeToBlack(App->scene2, App->scene2, 2.5f);
	};


	App->map->Draw();
	App->player->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);

	return true;
}

// Called each loop iteration
bool j1Scene2::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene2::CleanUp()
{
	LOG("Freeing scene");

	return true;
}*/
