/* 
 * File:   GameSprite.cpp
 * Author: zeroshade
 *
 * Created on May 7, 2012, 9:54 PM
 */

#include "GameSprite.h"
#include "TileMap.h"
#include <ClanLib/core.h>


GameSprite::GameSprite(CL_DisplayWindow& win, CL_ResourceManager& res, const CL_StringRef& sprname, int len)
: sprite(win.get_gc(), sprname, &res), anim_length(len), move_dir(0), prev_move_dir(0),
  anim_pos(0), anim_dir(1), cur_x(0), cur_y(0), key_down(win.get_ic().get_keyboard().sig_key_down().connect(this, &GameSprite::on_key_down)),
  key_up(win.get_ic().get_keyboard().sig_key_up().connect(this, &GameSprite::on_key_up)) 
{
    for (int i = 0; i < 4; ++i)
        keys[i] = false;
    last_update = CL_System::get_time();
}

int GameSprite::update(CL_GraphicContext& gc, int time_elapsed) {
    unsigned cur_time = CL_System::get_time();
    if (time_elapsed == -1)
        time_elapsed = cur_time - last_update;
    if (keys[move_dir]) {
        int frame = anim_pos + anim_length*move_dir;
        int speed = sprite.get_frame_delay(frame);
        if (time_elapsed >= speed) {
            last_update = cur_time;
            anim_pos += anim_dir;
            if (anim_pos < 0 || anim_pos >= anim_length) {
                anim_pos -= 2 * anim_dir;
                anim_dir *= -1;
            }
            frame = anim_pos + anim_length*move_dir;
            sprite.set_frame(frame);
            int screen_width = gc.get_width();
            int screen_height = gc.get_height();

            auto gety = [&](){return map->get_cur_y();};
            auto getx = [&](){return map->get_cur_x();};
            auto setx = [&](int val){map->set_cur_x(gc, map->get_cur_x() + val);};
            auto sety = [&](int val){map->set_cur_y(gc, map->get_cur_y() + val);};

            switch (move_dir) {
                case 0:
                    moving(cur_y, -5, ((cur_y - 5 < screen_height / 2) && (gety() > 0)), sety);
                    break;
                case 1:
                    moving(cur_x, 10, ((cur_x + 10 > screen_width / 2) &&
                            ((map->get_width() - getx()) > screen_width)), setx);
                    break;
                case 2:
                    moving(cur_y, 5, ((cur_y + 5 > screen_height / 2) &&
                            ((map->get_height() - gety()) > screen_height)), sety);
                    break;
                case 3:
                    moving(cur_x, -10, ((cur_x - 10 < screen_width / 2) &&
                            (getx() > 0)), setx);
                    break;
            }
            int xcheck = sprite.get_width() / 2;
            int ycheck = sprite.get_height() / 2;
            if (cur_x + xcheck > screen_width)
                cur_x = screen_width - xcheck;
            else if (cur_x - xcheck < 0)
                cur_x = xcheck;
            if (cur_y + ycheck > screen_height)
                cur_y = screen_height - ycheck;
            else if (cur_y - ycheck < 0)
                cur_y = ycheck;
        }
    }
    return 1;
}

template <typename SetFunc>
void GameSprite::moving(int& cur, const int val, const bool cond, SetFunc set) {
    if (cond)
        set(val);
    else
        cur += val;
}

void GameSprite::on_key_down(const CL_InputEvent& event, const CL_InputState& state) {
    CL_String k;
    int key = key_type(event.id, k);

    if (key != -1 && !keys[key]) {
        CL_Console::write_line("Key down: " + k);
        keys[key] = true;
        prev_move_dir = move_dir;
        move_dir = key;
    }
}

void GameSprite::on_key_up(const CL_InputEvent& event, const CL_InputState& state) {
    CL_String k;
    int key = key_type(event.id, k);

    if (key != -1 && keys[key]) {
        CL_Console::write_line("Key up: " + k);
        keys[key] = false;
        move_dir = prev_move_dir;
    }
}

int GameSprite::key_type(int id, CL_String& k) {
    switch (id) {
        case CL_KEY_RIGHT:
            k = "Right";
            return 1;
        case CL_KEY_UP:
            k = "Up";
            return 0;
        case CL_KEY_LEFT:
            k = "Left";
            return 3;
        case CL_KEY_DOWN:
            k = "Down";
            return 2;
        default:
            return -1;
    }
}
