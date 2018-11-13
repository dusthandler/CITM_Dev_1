#include "j1Player.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Enemies.h"
#include "p2Log.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "SDL/include/SDL_timer.h"
#include <math.h>
#include "j1Window.h"



j1Enemies::j1Enemies() : j1Module()

{
	name.create("enemies"); // aun no se poruqe


}

// Destructor
j1Enemies::~j1Enemies()
{

}
// Called before render is available
bool j1Enemies::Awake(pugi::xml_node&)
{
	EnemyFly_Animation = &EnemyFly_Idle;

	EnemyFly_Idle.PushBack({ 55, 2, ENEMY_FLY_WIDTH, ENEMY_FLY_HEIGHT });       // do this in tiled 
	Enemy_Flying_Right.PushBack({ 3, 96, 44, 45 });
	Enemy_Flying_Left.PushBack({ 341, 95, 44, 45 });
	

	Enemy_Fly_Death.PushBack({ 149, 1, ENEMY_FLY_WIDTH, ENEMY_FLY_HEIGHT });


	return true;


}

// Called before the first frame
bool j1Enemies::Start()
{
	Set_Enemies_Info();

	return true;
}

// Called each loop iteration
bool j1Enemies::PreUpdate()
{
	//Recorremos la lista de enmigos de este nivel y actualizamos sus colliders
	/*for (int i = 0; i < enemies.Count(); i++) {

		enemies[i].Enemy_Collider->SetPos(enemies[i].Pos.x, enemies[i].Pos.y);
	}*/
	
	return true;

}

void j1Enemies::Set_Enemies_Info() {


	//App->audio->LoadFx("Sound/Fx/jump.wav");          // FXs
	//App->audio->LoadFx("Sound/Fx/death.wav");

	pugi::xml_node InitEnemiesPos = App->map->map_file.child("map");

	// we need to load this from tiled 
	//TODO: edit xml map to add enemies!! And Load it!!
	//while (enemigos en mapa) {
	//	Enemy enemigoActual;
	//	enemigoActual.Pos.x = InitEnemiesPos.child("tileset").child("terraintypes").child("terrain").child("properties").child("property").attribute("value").as_float();
	//	enemigoActual.Pos.y = InitEnemiesPos.child("tileset").child("terraintypes").child("terrain").child("properties").child("property").next_sibling("property").attribute("value").as_float();
	//	enemigoActual.Alive = true;
	//	enemigoActual.Flying = XML.fly; //Fly enemies
	//	enemigoActual.Onplat = XML.Onplat;//Ground enemies
	//	enemigoActual.Enemy_Texture = App->tex->Load("Maps/Enemies/FLyer/Duck.png");

	//	enemigoActual.Enemy_Collider = App->collision->AddCollider({ (int)Pos.x, (int)Pos.y, 35, 45 }, COLLIDER_PLAYER, this);

	//	enemies.Insert(enemigoActual)
	//}
	


}
