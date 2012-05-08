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

/**
 * Struct for holding layers
 */
struct MapLayer {
    std::vector<int> map; /**< vector which holds the tile numbers for the layer */
};

/**
 * Class to represent a tiled map
 */
class TileMap {
public:
    /**
     * Constructor
     * @param gc Current CL_GraphicContext
     * @param resmgr The Current CL_ResourceManager
     * @param tileset CL_String which is the name of the tileset
     */
    TileMap(CL_GraphicContext& gc, CL_ResourceManager& resmgr, const CL_String& tileset);
    
    /**
     * Function to draw the current viewable area of the TileMap
     * @param gc The GraphicContext to draw to
     */
    void draw(CL_GraphicContext& gc);
    int get_height() const { return map_height*tile_height; } /**< @return the height of the map */
    int get_width() const { return map_width*tile_width; } /**< @return the width of the map */
    /**
     * Used to find the top left X coordinate of the map, relative to the absolute map
     * @return the value of the top left x coordinate relative to the map
     */
    int get_cur_x() const { return cur_map_x; }
    /**
     * Used to find the top left Y coordinate of the map, relative to the absolute map
     * @return the value of the top left y coordinate relative to the map
     */
    int get_cur_y() const { return cur_map_y; }
    
    void set_cur_x(CL_GraphicContext& gc, int x); /**< Set the value of the top left x coordinate */
    void set_cur_y(CL_GraphicContext& gc, int y); /**< Set the value of the top left y coordinate */
private:
    int map_width, map_height; /**< The number of tiles to make up the height and width of the map */
    CL_Sprite tiles; /**< The Actual representation of the tiles image */
    int tile_width, tile_height; /**< The pixel value height and width of a single tile */
    std::vector<MapLayer> layers; /**< vector to hold the layers of the map */
    int cur_map_x, cur_map_y; /**< The current top left coordinate of the map relative to the screen */
    CL_String levelname; /**< The levelname attribute of the TileMap */
};

#endif	/* TILEMAP_H */

