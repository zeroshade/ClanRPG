#include "TileMap.h"
#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl1.h>

class GameSprite {
public:
    GameSprite(CL_DisplayWindow& win, CL_ResourceManager& res,const CL_StringRef& sprname, int len) 
        : sprite(win.get_gc(),sprname,&res),anim_length(len),move_dir(0),prev_move_dir(0),
                anim_pos(0),anim_dir(1), cur_x(0), cur_y(0),
                key_down(win.get_ic().get_keyboard().sig_key_down().connect(this, &GameSprite::on_key_down)),
                key_up(win.get_ic().get_keyboard().sig_key_up().connect(this, &GameSprite::on_key_up))
    {
            for (int i = 0; i < 4; ++i)
                keys[i] = false;
            last_update = CL_System::get_time();
    }
    void set_map(TileMap* tile) { map = tile; }
    void set_coords(int x, int y) { cur_x = x; cur_y = y; }
    int update(CL_GraphicContext& gc, int time_elapsed = -1) { 
        unsigned cur_time = CL_System::get_time();
        if (time_elapsed == -1)
            time_elapsed = cur_time - last_update;
        if (keys[move_dir]) {
            int frame = anim_pos+anim_length*move_dir;
            int speed = sprite.get_frame_delay(frame);
            if (time_elapsed >= speed) {
                last_update = cur_time;
                anim_pos += anim_dir;
                if (anim_pos < 0 || anim_pos >= anim_length) {
                    anim_pos -= 2 * anim_dir;
                    anim_dir *= -1;
                }
                frame = anim_pos+anim_length*move_dir;
                sprite.set_frame(frame);
                int screen_width = gc.get_width();
                int screen_height = gc.get_height();
                
                auto gety = [&](){ return map->get_cur_y(); };
                auto getx = [&](){ return map->get_cur_x(); };
                auto setx = [&](int val) { map->set_cur_x(gc,map->get_cur_x() + val);};
                auto sety = [&](int val) { map->set_cur_y(gc, map->get_cur_y() + val);};

                switch (move_dir) {
                    case 0:
                        moving(cur_y, -5, ((cur_y - 5 < screen_height / 2) && (gety()>0)), sety);
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
                        moving(cur_x, -10, ((cur_x - 10 < screen_width /2) && 
                                (getx()>0)), setx);
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
    void draw(CL_GraphicContext& gc) { return draw(gc, cur_x, cur_y); }
    void draw(CL_GraphicContext& gc, int x, int y) { sprite.draw(gc, x - sprite.get_width() / 2, y - sprite.get_height() / 2); }
    void draw(CL_GraphicContext& gc, float x, float y) { sprite.draw(gc, x - sprite.get_width() / 2, y - sprite.get_height() / 2); }
    int get_height() const { return sprite.get_height(); }
    int get_width() const { return sprite.get_width(); } 
    int get_x() { return cur_x; }
    int get_y() { return cur_y; }

private: 
    template <typename SetFunc>
    void moving (int& cur, const int val, const bool cond, SetFunc set) {
        if (cond)
            set(val);
        else
            cur += val;
    }
    void on_key_down(const CL_InputEvent& event, const CL_InputState& state) {        
        CL_String k;
        int key = key_type(event.id,k);
        
        if (key != -1 && !keys[key]) {            
            CL_Console::write_line("Key down: " + k);
            keys[key] = true;
            prev_move_dir = move_dir;
            move_dir = key;
        }
    }
    void on_key_up(const CL_InputEvent& event, const CL_InputState& state) { 
        CL_String k;
        int key = key_type(event.id,k);
        
        if (key != -1 && keys[key]) {
            CL_Console::write_line("Key up: " + k);
            keys[key] = false;
            move_dir = prev_move_dir;
        }
    }
    
    int key_type(int id, CL_String& k) {
        switch(id) {
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

class PrimitivesExample
{
private:
   bool quit;

public:

    void run() {
        quit = false;
        // Set the window
        CL_DisplayWindowDescription desc;
        desc.set_title("Sunset");
        desc.set_size(CL_Size(640, 480), true);
        desc.set_allow_resize(true);

        CL_DisplayWindow window(desc);

        CL_Slot slot_quit = window.sig_window_close().connect(this, &PrimitivesExample::on_window_close);

        CL_GraphicContext gc = window.get_gc();
        CL_InputDevice keyboard = window.get_ic().get_keyboard();

        CL_FontDescription system_font_desc;
        system_font_desc.set_typeface_name("tahoma");
        system_font_desc.set_height(30);
        CL_Font_System system_font(gc, system_font_desc);

        CL_ResourceManager resources("resources/resources.xml");
	CL_Sprite boat_sprite(gc, "Boat", &resources);
        
        GameSprite tonberry_sprite(window, resources, "Tonberry",3);
        //tonberry_sprite.AttachKeyboard(window);
        tonberry_sprite.set_coords(320, 240);
        TileMap map(gc, resources, "cliff");
        tonberry_sprite.set_map(&map);
        
        while (!quit) {
            if (keyboard.get_keycode(CL_KEY_ESCAPE) == true)
                quit = true;

            draw_sunset(gc);
            boat_sprite.draw(gc, 70, 252);
            map.draw(gc);
            tonberry_sprite.draw(gc);
            system_font.draw_text(gc, 146, 50, "A Quiet Evening on the Pacific...");
            boat_sprite.update();
            tonberry_sprite.update(gc);
            
            
            window.flip();
            CL_KeepAlive::process();

            CL_System::sleep(10);
       }
   }

   void draw_sunset(CL_GraphicContext &gc)
   {
       CL_Colorf red(155/255.0f, 60/255.0f, 68/255.0f);
       CL_Colorf yellow(255/255.0f, 234/255.0f, 117/255.0f);
       CL_Colorf blue(13/255.0f, 75/255.0f, 74/255.0f);
       CL_Colorf lightblue(16/255.0f, 91/255.0f, 90/255.0f);

       // Draw top of sunset
       CL_Gradient gradient1(CL_Colorf::black, red);
       CL_Draw::gradient_fill(gc, CL_Rectf(0,0,640,160), gradient1);

       // Draw second part of sunset
       CL_Gradient gradient2(red, yellow);
       CL_Draw::gradient_fill(gc, CL_Rectf(0,160,640,240), gradient2);

       // Draw the sun
       CL_Draw::circle(gc, CL_Pointf(320, 240), 15, CL_Colorf::white);

       // Draw the ground
       CL_Draw::fill(gc, CL_Rectf(0, 240, 640, 480), blue);

       // Draw some distance lines
       for(int y = 241, ydelta = 2; y < 480; y += ydelta, ydelta += ydelta)
       {
           CL_Draw::line(gc, 0, y, 640, y, lightblue);
       }
   }

   void on_window_close()
   {
       quit = true;
   }
};

class ConsoleProgram
{
public:

    static int main(const std::vector<CL_String> &args) {
        
        try {

            CL_SetupCore setup_core;
            CL_SetupDisplay setup_display;
            CL_SetupGL1 setup_gl;

            PrimitivesExample primitives;
            primitives.run();
                    
        } catch (CL_Exception &exception) {
            // Create a console window for text-output if not available
            CL_ConsoleWindow console("Console", 80, 160);

            CL_Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
            console.display_close_message();
            return -1;
        }
        return 0;
    }
};

// Create global application object, you MUST include this line or
// the application start-up will fail to locate your application object.
CL_ClanApplication app(&ConsoleProgram::main);