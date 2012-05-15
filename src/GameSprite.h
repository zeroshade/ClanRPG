/* 
 * File:   GameSprite.h
 * Author: zeroshade
 *
 * Created on May 7, 2012, 9:54 PM
 */
#ifndef GAMESPRITE_H
#define GAMESPRITE_H

#include <ClanLib/display.h>
#include <ClanLib/gl1.h>

class CL_DisplayWindow;
class CL_ResourceManager;
class TileMap;

class GameSprite {
public:
    GameSprite(CL_DisplayWindow& win, CL_ResourceManager& res,const CL_StringRef& sprname, int len);
    
    inline void set_map(TileMap* tile) { map = tile; }
    inline void set_coords(int x, int y) { cur_x = x; cur_y = y; }
    
    int update(CL_GraphicContext& gc, int time_elapsed = -1);
    inline void draw(CL_GraphicContext& gc) { return draw(gc, cur_x, cur_y); }
    inline void draw(CL_GraphicContext& gc, int x, int y) { sprite.draw(gc, x - sprite.get_width() / 2, y - sprite.get_height() / 2); }
    inline void draw(CL_GraphicContext& gc, float x, float y) { sprite.draw(gc, x - sprite.get_width() / 2, y - sprite.get_height() / 2); }
    inline int get_height() const { return sprite.get_height(); }
    inline int get_width() const { return sprite.get_width(); } 
    inline int get_x() { return cur_x; }
    inline int get_y() { return cur_y; }

private: 
    template <typename SetFunc>
    void moving (int& cur, const int val, const bool cond, SetFunc set); 
    void on_key_down(const CL_InputEvent& event, const CL_InputState& state);
    void on_key_up(const CL_InputEvent& event, const CL_InputState& state);
    int key_type(int id, CL_String& k);
    
    int anim_length;
    int move_dir;
    int prev_move_dir;
    int anim_pos;
    int anim_dir;
    CL_Sprite sprite;
    CL_Slot key_down;
    CL_Slot key_up;
    bool keys[4];
    unsigned last_update;
    int cur_x, cur_y;
    TileMap* map;
};

#endif