/* 
 * File:   TileMap.cpp
 * Author: zeroshade
 * 
 * Created on March 13, 2012, 1:01 AM
 */

#include "TileMap.h"
#include <ClanLib/core.h>

TileMap::TileMap(CL_GraphicContext& gc, CL_ResourceManager& resmgr, const CL_String& tileset)
: map_width(0), map_height(0), cur_map_x(0), cur_map_y(0) {
    CL_Resource res = resmgr.get_resource(tileset);

    if (res.get_type() != "tilemap")
        throw CL_Exception(cl_format("Resource %1 is not a tilemap", tileset));

    CL_DomElement element = res.get_element();
    levelname = element.get_attribute("name");
    CL_String resource_name = element.get_attribute("resource");
    map_width = element.get_attribute_int("width");
    map_height = element.get_attribute_int("height");

    tiles = CL_Sprite(gc, resource_name, &resmgr);
    float scalex, scaley;
    tiles.get_scale(scalex, scaley);
    tile_width = tiles.get_width() * scalex;
    tile_height = tiles.get_height() * scaley;

    auto layer_nodes = element.select_nodes("layer");
    for (CL_DomNode& idx : layer_nodes) {
        CL_DomElement layer_element = idx.to_element();

        CL_String layer_tiles = layer_element.get_first_child().get_node_value();
        std::vector<CL_String> tile_indices = CL_StringHelp::split_text(layer_tiles, ",");

        MapLayer layer;
        layer.map.reserve(tile_indices.size());
        for (auto& tile : tile_indices)
            layer.map.push_back(CL_StringHelp::text_to_int(tile));

        layers.push_back(layer);
    }
}

void TileMap::draw(CL_GraphicContext& gc) {
    int screen_width = gc.get_width();
    int screen_height = gc.get_height();
    int horiz_tiles = screen_width / tile_width + 1;
    int vert_tiles = screen_height / tile_height + 1;

    // avoid half tiles by increasing by one
    horiz_tiles++;
    vert_tiles++;

    int start_tilex = cl_max(0, cur_map_x / tile_width);
    int start_tiley = cl_max(0, cur_map_y / tile_height);

    int scroll_offsetx = cur_map_x - start_tilex * tile_width;
    int scroll_offsety = cur_map_y - start_tiley * tile_width;

    if (horiz_tiles + start_tilex > map_width)
        horiz_tiles = map_width - start_tilex;

    if (vert_tiles + start_tiley > map_height)
        vert_tiles = map_height - start_tiley;

    for (MapLayer& layer : layers) {
        for (int cur_y = 0; cur_y < vert_tiles; ++cur_y) {
            for (int cur_x = 0; cur_x < horiz_tiles; ++cur_x) {
                int idx = (start_tiley + cur_y) * map_width + start_tilex + cur_x;
                int sprite_idx = layer.map[idx];
                int tile_xpos = cur_x * tile_width - scroll_offsetx;
                int tile_ypos = cur_y * tile_height - scroll_offsety;

                tiles.set_frame(sprite_idx);
                tiles.draw(gc, tile_xpos, tile_ypos);
            }
        }
    }
}

void TileMap::set_cur_x(CL_GraphicContext& gc, int x) {
    if (x < 0)
        x = 0;
    else if (get_width() - x < gc.get_width())
        x = get_width() - gc.get_width();
    cur_map_x = x;
}

void TileMap::set_cur_y(CL_GraphicContext& gc, int y) {
    if (y < 0)
        y = 0;
    else if (get_height() - y < gc.get_height())
        y = get_height() - gc.get_height();
    cur_map_y = y;
}
