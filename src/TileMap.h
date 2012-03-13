/* 
 * File:   TileMap.h
 * Author: zeroshade
 *
 * Created on March 13, 2012, 1:01 AM
 */

#ifndef TILEMAP_H
#define	TILEMAP_H

#include <vector>
#include <ClanLib/display.h>

class CL_GraphicContext;
class CL_ResourceManager;
class CL_Sprite;

struct MapLayer {
    std::vector<int> map;
};

class TileMap {
public:
    TileMap(CL_GraphicContext& gc, CL_ResourceManager& resmgr, const CL_String& tileset);
    
    void draw(CL_GraphicContext& gc);
    int get_height() const { return map_height*tile_height; }
    int get_width() const { return map_width*tile_width; }
    int get_cur_x() const { return cur_map_x; }
    int get_cur_y() const { return cur_map_y; }
    
    void set_cur_x(CL_GraphicContext& gc, int x);
    void set_cur_y(CL_GraphicContext& gc, int y);
private:
    int map_width, map_height;
    CL_Sprite tiles;
    int tile_width, tile_height;
    std::vector<MapLayer> layers;
    int cur_map_x, cur_map_y;
    CL_String levelname;
};

#endif	/* TILEMAP_H */

