#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Collision.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;
	
	MapLayer* layer = data.layers.start->data; 
	TileSet* tileset = data.tilesets.start->data;


	

	p2List_item<TileSet*>* item_tile = data.tilesets.end;//Painters rule application.
	p2List_item<MapLayer*>* layers_lay = data.layers.start;


	while (item_tile != NULL) {

		layers_lay = data.layers.start;

		while (layers_lay != NULL) {

			for (uint x = 0; x < layers_lay->data->width; x++) {

				for (uint y = 0; y < layers_lay->data->height; y++) {

					SDL_Rect rect = item_tile->data->GetTileRect(layers_lay->data->Get(x, y));
					iPoint world_coords = MapToWorld(x, y);
					App->render->Blit(item_tile->data->texture, world_coords.x, world_coords.y, &rect, layers_lay->data->Parallaxspeed);
			
					

				}

			}
			layers_lay = layers_lay->next;
		}
		item_tile = item_tile->prev;
	}



}





iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret(0,0);

	if (data.type == MapTypes::MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
	}


	return ret;
}


iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0,0);
	
	if (data.type == MapTypes::MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}
	

	if (data.type == MapTypes::MAPTYPE_ORTHOGONAL) {
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	};
	


	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = {0, 0, 0, 0};

	int relative_id = id - firstgid;

	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;


	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* Tileset_item;
	Tileset_item = data.tilesets.start;

	while(Tileset_item != NULL)
	{
		App->tex->UnLoad(Tileset_item->data->texture);   // TODO: check the importance of this line 
		Tileset_item->data->texture = nullptr; 
		RELEASE(Tileset_item->data);
		
		Tileset_item = Tileset_item->next;
	}
	data.tilesets.clear();


	// Remove all layers
	p2List_item<MapLayer*>* Layer_item;
	Layer_item = data.layers.start;

	while(Layer_item != NULL)
	{
		RELEASE(Layer_item->data);
		Layer_item = Layer_item->next;
	}
	data.layers.clear();


	// Remove all objects
	p2List_item<MapObject*>* Object_item;
	Object_item = data.objects.start; 

	while (Object_item != NULL)
	{
		LOG("Liberando objeto %s", Object_item->data->name.GetString());
		// TODO : A veces al intentar liberar un objeto del mapa da error, como si la lista contuviese un objeto nulo o lago as�. 
		RELEASE(Object_item->data);
		Object_item = Object_item->next;

	}

	data.objects.clear();
	
	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load all objects info ----------------------------------------------
	
	
	for (pugi::xml_node Object_Group = map_file.child("map").child("objectgroup"); Object_Group; Object_Group = Object_Group.next_sibling("objectgroup")) {
		for (pugi::xml_node object = Object_Group.child("object"); object;  object = object.next_sibling("object")) {

			MapObject* Map_Object = new MapObject();

			ret = Load_Object(object, Map_Object);

			if (ret == true)
				data.objects.add(Map_Object);

		}
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for(layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if(ret == true)
			data.layers.add(lay);
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);
		LOG("X %f y %f", data.Posi.x, data.Posi.y);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		data.Posi.x = map.child("tileset").child("terraintypes").child("terrain").child("properties").child("property").child("PlayerX").attribute("value").as_float();
		data.Posi.y = map.child("tileset").child("terraintypes").child("terrain").child("properties").child("property").child("PlayerY").attribute("value").as_float();
	
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::Load_Object(pugi::xml_node& node, MapObject* MapObject) {

	// LOADING
	bool ret = true; 

	MapObject->name = node.attribute("name").as_string();
	MapObject->id = node.attribute("id").as_uint();
	MapObject->X_Pos = node.attribute("x").as_int();
	MapObject->Y_Pos = node.attribute("y").as_int();
	MapObject->width = node.attribute("width").as_uint();
	MapObject->height = node.attribute("height").as_uint();

	Set_Colliders(node, MapObject);


	if (node.empty())
	{
		LOG("Error parsing map xml file: Cannot find 'object/data' tag.");
		ret = false;
		RELEASE(MapObject);
	}

	return ret; 
}

bool j1Map::Set_Colliders(pugi::xml_node& node, MapObject* MapObject) {

	// COLLIDERS 

	bool ret = true; 

	MapObject->name = node.attribute("name").as_string();

	if (MapObject->name == "Floor" || MapObject->name == "BGFloor") {
	
	App->collision->AddCollider({ MapObject->X_Pos, MapObject->Y_Pos, MapObject->width, MapObject->height }, COLLIDER_WALL, this);

	}
	else if(MapObject->name == "Die"){
		App->collision->AddCollider({ MapObject->X_Pos, MapObject->Y_Pos, MapObject->width, MapObject->height }, COLLIDER_DEATH, this); 
	}

	else if (MapObject->name == "Win") {
		App->collision->AddCollider({ MapObject->X_Pos, MapObject->Y_Pos, MapObject->width, MapObject->height }, COLLIDER_WIN, this);
	}

	return ret; 
}


bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();

	

	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	layer->Parallaxspeed = node.child("properties").child("property").attribute("value").as_float();
	pugi::xml_node layer_data = node.child("data");

	if(layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width*layer->height];
		memset(layer->data, 0, layer->width*layer->height);

		int i = 0;
		for(pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}