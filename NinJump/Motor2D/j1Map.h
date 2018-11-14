#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

struct Collider;

enum LayerType {
	LAYER_NONE = -1,
	LAYER_MAIN,
	LAYER_DW,
	LAYER_PARA_1,
	LAYER_PARA_2,
	LAYER_PARA_3,
	LAYER_PARA_4,
	LAYER_BG_1,
	LAYER_BG_2,

};
// ----------------------------------------------------
struct MapLayer
{
	p2SString	name;
	int			width;
	int			height;
	float		Parallaxspeed;
	uint*		data;
	LayerType	type1 = LAYER_NONE;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[y*width + x];
	}

};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};


struct MapObject {

	p2SString			name;
	uint			    id;
	int				X_Pos;
	int				Y_Pos;
	int				width;
	int				height;

};


enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};


// ----------------------------------------------------
struct MapData
{
	int					width;
	fPoint				Posi;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*>	layers;
	p2List<MapObject*>	objects;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	// Coordinate translation methods
	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;

	bool Load_Object(pugi::xml_node& node, MapObject* MapObject);
	bool Set_Colliders(pugi::xml_node& node, MapObject* MapObject);

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);



public:

	MapData data;
	pugi::xml_document	map_file;

private:

	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__