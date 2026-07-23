                                                                                                                 
// YeOldeWorld Editor


#define ENET_IMPLEMENTATION
#include"inc/enet.h"
#include"inc/raylib.h"

#include<iostream>
#include<string>
#include<vector>
#include<cstdint>
#include<fstream>
#include<cstring>

#include"inc/network.hpp"
#include"inc/data.hpp"
#include"inc/assets.hpp"
#include"inc/player.hpp"
#include"inc/menu.hpp"
#include"inc/camera.hpp"
#include"inc/gui.hpp"
#include"inc/edittools.hpp"
#include"inc/maptools.hpp"

struct Vector2_int
{
    int x;
    int y;
};

int Set_Editor(Game_Assets &OldeAssets,Game_Data &OldeSettings,float &d_time);

//  M   M AA  III N   N     M   M EEEE N   N U   U
//  MM MMA  A  I  NN  N     MM MM E    NN  N U   U
//  M M MAAAA  I  N N N     M M M EEE  N N N U   U
//  M   MA  A  I  N  NN     M   M E    N  NN U   U
//  M   MA  A III N   N     M   M EEEE N   N  UUU 

int main()
{
    const std::string OldeTitle="Olde";
    Game_Data OldeSettings;
    InitWindow(OldeSettings.main_screen_width,OldeSettings.main_screen_height,OldeTitle.c_str());
    Game_Menu OldeMenu(0);
    Game_Assets OldeAssets;
    SetTargetFPS(OldeSettings.set_max_fps);
    bool main_editor=true;
    bool debug_mode=false;
    bool load_selected_map=false;
    float d_time=0.0f;

    int rc_menu=0;

    while(main_editor)
    {
        d_time+=GetFrameTime();

        OldeMenu.Control();

        BeginDrawing();
            ClearBackground(BLACK);
            OldeMenu.DrawMenu(OldeAssets,OldeSettings,d_time,debug_mode);
        EndDrawing();

        rc_menu=OldeMenu.ProcessMenu();

        switch(rc_menu)
        {
            case -2: // -2 is "continue" return
                if(d_time>3.0) d_time=0.0;
            break;

            case -1:
                if(OldeMenu.public_display_main_menu) main_editor=false;
                if(OldeMenu.public_display_load_menu)
                {
                    OldeMenu.public_display_main_menu=true;
                    OldeMenu.public_display_load_menu=false;
                    rc_menu=-2;
                }
                if(OldeMenu.public_display_options_menu)
                {
                    OldeMenu.public_display_options_menu=false;
                    OldeMenu.public_display_main_menu=true;
                }
            break;

            case 0:
                if(OldeMenu.public_display_load_menu)
                {
                    OldeMenu.TypeNewMapName(OldeAssets,OldeSettings); // and accept
                    OldeMenu.public_display_load_menu=false;
                    OldeMenu.public_display_main_menu=true;
                    OldeSettings.editor_new_map=true;
                    load_selected_map=true;
                    rc_menu=-2;
                }
                if(OldeMenu.public_display_main_menu)
                {
                    OldeMenu.public_display_main_menu=false;
                    OldeMenu.public_display_load_menu=true;
                    rc_menu=-2;
                }
            break;

            case 1:
                if(OldeMenu.public_display_main_menu)
                {
                    OldeMenu.public_display_main_menu=false;
                    OldeMenu.public_display_options_menu=true;
                    break;
                }
                if(OldeMenu.public_display_options_menu) break;
                
            case 2:
                if(OldeMenu.public_display_options_menu)
                {
                    if(!IsWindowFullscreen()) OldeSettings.ChangeResolution();
                    break;
                }

            case 3:
                if(OldeMenu.public_display_options_menu)
                {
                    OldeSettings.SetScreenMode();
                    break;
                }

            default:
                if(OldeMenu.public_display_load_menu)
                {
                    OldeMenu.AcceptMap(OldeSettings,rc_menu);
                    printf("OLDE: accepted map > %s\n",OldeSettings.current_map.c_str());
                    OldeSettings.editor_new_map=false;
                    OldeMenu.public_display_load_menu=false;
                    OldeMenu.public_display_main_menu=true;
                    load_selected_map=true;
                    rc_menu=-2;
                }
            break;
        }

        if(load_selected_map)
        {
            OldeMenu.ShowLoading(OldeAssets,OldeSettings,"Loading");
            Set_Editor(OldeAssets,OldeSettings,d_time);
            OldeMenu.public_display_load_menu=false;
            OldeMenu.public_display_main_menu=true;
            load_selected_map=false;
        }
    }
    return 0;
}

//  EEEE DDD  III TTTTTT  OOO  RRRR 
//  E    D  D  I    TT   O   O R   R
//  EEE  D  D  I    TT   O   O RRRR 
//  E    D  D  I    TT   O   O R R  
//  EEEE DDD  III   TT    OOO  R  RR                                                      

int Set_Editor(Game_Assets &OldeAssets,Game_Data &OldeSettings,float &d_time)
{
    const int tilesize=16;
    

    enum Palette_types:uint8_t
    {
        PaletteMap=0
    };

    // constexpr float start_point_x=7*tilesize;
    // constexpr float start_point_y=5*tilesize;
    // constexpr float start_point_z=20*tilesize;
    // constexpr float start_point_w=11*tilesize;

    float speed_pointer_move=1.0f;
    Vector2 edit_pointer_pos{};
    // Vector2 camera_pointer_pos{};
    Vector2 mouse_pos{}; // ?
    Vector4 render{};
    int16_t palette_num=0;
    uint8_t palette_type=PaletteMap;
    // for now - only one map layer

    Game_Player OldePlayer;
    PlayerCamera OldeCam;
    PlayerGui OldeGui;
    Game_Menu OldeMenu(1);
    MapTools OldeMap(OldeSettings.current_map,OldeSettings.editor_new_map);
    EditTools OldeEdit;
    OldeMenu.SetResizeValues(OldeMap.EditorGetMaxX(),OldeMap.EditorGetMaxY());
    bool editor_window=true;
    bool show_menu=false;
    bool debug_mode=false;
    bool set_save_map=false;
    // bool set_load_map=false;
    bool reloadAssets=false;
    bool draw_only_selected_palette=false;
    bool map_resize=false;
    int secret_rc=0;



    //Initialize part



    // Vector4 for rendering window size
    // using floats!!!
    // spawn points loaded from map are int tiled map points! - always multipy those by tilesize!!!
    // at size 4.0 rendering x13 and y9
    // allows for filling whole screen
    // lets load this point from Oldemap.spawn_point0_x and _y


    render.x=OldeSettings.xrender*tilesize;
    render.y=OldeSettings.yrender*tilesize;
    render.z=OldeSettings.zrender*tilesize;
    render.w=OldeSettings.wrender*tilesize;

    edit_pointer_pos.x=OldeMap.spawn_point0_x*tilesize;
    edit_pointer_pos.y=OldeMap.spawn_point0_y*tilesize;

    // camera_pointer_pos.x=(OldeMap.spawn_point0_x-tilesize)*tilesize;
    // camera_pointer_pos.y=(OldeMap.spawn_point0_y-tilesize)*tilesize;

    OldeCam.ChangeSpeed(1.0f);

    while(editor_window)
    {
        mouse_pos=GetMousePosition();
        d_time+=GetFrameTime(); // d_time for animations
        if(IsKeyPressed(KEY_ESCAPE)) show_menu=true;

        //TILE palette CURSOR

        if(IsKeyPressed(KEY_W) && !show_menu)
        {
            palette_num-=10;
            if(palette_num<0) palette_num=0;
        }

        if(IsKeyPressed(KEY_S) && !show_menu)
        {
            palette_num+=10;
            if(palette_num>=(OldeMap.tile_png_y*OldeMap.tile_png_x)-1) palette_num=(OldeMap.tile_png_y*OldeMap.tile_png_x)-1;
        }

        if(IsKeyPressed(KEY_A) && !show_menu)
        {
            palette_num--;
            if(palette_num<0) palette_num=0;
        }

        if(IsKeyPressed(KEY_D) && !show_menu)
        {
            palette_num++;
            if(palette_num>=(OldeMap.tile_png_y*OldeMap.tile_png_x)-1) palette_num=(OldeMap.tile_png_y*OldeMap.tile_png_x)-1;
        }

        // if(IsKeyPressed(KEY_KP_8) && IsKeyDown(KEY_LEFT_CONTROL) && !show_menu)
        // {
        //     OldeSettings.wrender++;
        // }

        // if(IsKeyPressed(KEY_KP_2) && IsKeyDown(KEY_LEFT_CONTROL) && !show_menu)
        // {
        //     OldeSettings.wrender--;
        // }

        // if(IsKeyPressed(KEY_KP_4) && IsKeyDown(KEY_LEFT_CONTROL) && !show_menu)
        // {
        //     OldeSettings.zrender++;
        // }

        // if(IsKeyPressed(KEY_KP_6) && IsKeyDown(KEY_LEFT_CONTROL) && !show_menu)
        // {
        //     OldeSettings.zrender--;
        // }

        // // **********************************************************************

        // if(IsKeyPressed(KEY_KP_8) && IsKeyUp(KEY_LEFT_CONTROL)  && !show_menu)
        // {
        //     OldeSettings.yrender++;
        // }

        // if(IsKeyPressed(KEY_KP_2) && IsKeyUp(KEY_LEFT_CONTROL)  && !show_menu)
        // {
        //     OldeSettings.yrender--;
        // }

        // if(IsKeyPressed(KEY_KP_4) && IsKeyUp(KEY_LEFT_CONTROL)  && !show_menu)
        // {
        //     OldeSettings.xrender++;
        // }

        // if(IsKeyPressed(KEY_KP_6) && IsKeyUp(KEY_LEFT_CONTROL)  && !show_menu)
        // {
        //     OldeSettings.xrender--;
        // }

        if(IsKeyDown(KEY_SPACE) && !show_menu)
        {
            OldeMap.EditorPlaceTile(OldeEdit,palette_num,palette_type);
        }

        if(IsKeyDown(KEY_BACKSPACE) && !show_menu)
        {
            OldeMap.EditorPlaceTile(OldeEdit,0,palette_type);
        }

        if(IsKeyDown(KEY_BACKSPACE) && IsKeyPressed(KEY_B) && !show_menu)
        {
            OldeMap.EditorClearLayer(PaletteMap);
        }

        // if(IsKeyDown(KEY_BACKSPACE) && IsKeyPressed(KEY_N) && !show_menu)
        // {
        //     OldeMap.EditorClearLayer(PaletteObjects);
        // }

        // if(IsKeyDown(KEY_BACKSPACE) && IsKeyPressed(KEY_M) && !show_menu)
        // {
        //     OldeMap.EditorClearLayer(PaletteRoofs);
        // }

        // if(IsKeyPressed(KEY_ONE) && !show_menu)
        // {
        //     palette_type=PaletteMap;
        // }
        // if(IsKeyPressed(KEY_TWO) && !show_menu)
        // {
        //     palette_type=PaletteObjects;
        // }
        // if(IsKeyPressed(KEY_THREE) && !show_menu)
        // {
        //     palette_type=PaletteRoofs;
        // }

        if(IsKeyPressed(KEY_U) && !show_menu)
        {
            OldeMap.SetMapSpawnPoint(OldeEdit,0);
        }

        if(IsKeyPressed(KEY_I) && !show_menu)
        {
            OldeMap.SetMapSpawnPoint(OldeEdit,1);
        }

        if(IsKeyPressed(KEY_O) && !show_menu)
        {
            OldeMap.SetMapSpawnPoint(OldeEdit,2);
        }

        if(IsKeyPressed(KEY_P) && !show_menu)
        {
            OldeMap.SetMapSpawnPoint(OldeEdit,3);
        }

        if(IsKeyPressed(KEY_H) && !show_menu)
        {
            OldeMap.JumpToSpawnPoint(edit_pointer_pos,0);
        }

        if(IsKeyPressed(KEY_J) && !show_menu)
        {
            OldeMap.JumpToSpawnPoint(edit_pointer_pos,1);
        }

        if(IsKeyPressed(KEY_K) && !show_menu)
        {
            OldeMap.JumpToSpawnPoint(edit_pointer_pos,2);
        }

        if(IsKeyPressed(KEY_L) && !show_menu)
        {
            OldeMap.JumpToSpawnPoint(edit_pointer_pos,3);
        }

        if(IsKeyPressed(KEY_DELETE) && !show_menu)
        {
            if(draw_only_selected_palette==true) draw_only_selected_palette=false;
            if(draw_only_selected_palette==false) draw_only_selected_palette=true;
        }

        //POINTER
        // float speed_pointer_move=1.0f;
        // float speed_camera_movement=1.0f;
        if(IsKeyDown(KEY_UP) && !show_menu)
        {
            edit_pointer_pos.y-=speed_pointer_move;
            if(edit_pointer_pos.y<0.0) edit_pointer_pos.y=0.0;
        }
        if(IsKeyDown(KEY_DOWN) && !show_menu) 
        {
            edit_pointer_pos.y+=speed_pointer_move;
            if(edit_pointer_pos.y>(OldeMap.EditorGetMaxY()-1)*tilesize) edit_pointer_pos.y=(OldeMap.EditorGetMaxY()-1)*tilesize;
        }
        if(IsKeyDown(KEY_LEFT) && !show_menu)
        {
            edit_pointer_pos.x-=speed_pointer_move;
            if(edit_pointer_pos.x<0.0) edit_pointer_pos.x=0.0;
        }
        if(IsKeyDown(KEY_RIGHT) && !show_menu)
        {
            edit_pointer_pos.x+=speed_pointer_move;
            if(edit_pointer_pos.x>(OldeMap.EditorGetMaxX()-1)*tilesize) edit_pointer_pos.x=(OldeMap.EditorGetMaxX()-1)*tilesize;
        }

        // CAMERA

        OldeCam.Move(edit_pointer_pos);

        // RENDERING FRAME

        render.x=OldeCam.cam.target.x-(OldeSettings.xrender*tilesize);
        render.y=OldeCam.cam.target.y-(OldeSettings.yrender*tilesize);
        render.z=OldeCam.cam.target.x+(OldeSettings.zrender*tilesize);
        render.w=OldeCam.cam.target.y+(OldeSettings.wrender*tilesize);

        // RELOAD BUTTON

        // ONLY DEBUG

        if(IsKeyPressed(KEY_R) && !show_menu && debug_mode)
        {
            reloadAssets=true;
        }

        if(IsKeyPressed(KEY_EQUAL) && !show_menu) OldeCam.cam.zoom+=0.5f;
        if(IsKeyPressed(KEY_MINUS) && !show_menu) OldeCam.cam.zoom-=0.5f;

        if(OldeCam.cam.zoom<1.0) OldeCam.cam.zoom=1.0;
        if(OldeCam.cam.zoom>4.0) OldeCam.cam.zoom=4.0;

        OldeEdit.MovePointer(edit_pointer_pos);

        if(show_menu) OldeMenu.Control();

        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode2D(OldeCam.cam);
                OldeMap.DrawMap(OldeAssets,d_time,render,debug_mode);
                // OldeMap.DrawObjects(OldeAssets,d_time,render,debug_mode);
                OldeEdit.DrawPointer(OldeAssets,d_time,debug_mode);
            EndMode2D();

            // OldeGui.DrawGui();
            OldeMap.EditorDrawPalette(OldeAssets,palette_num,palette_type);
            if(debug_mode)
            {
                DrawText(TextFormat("editor pointer x%.2f y%.2f",edit_pointer_pos.x,edit_pointer_pos.y),16,0,16,WHITE);
                DrawText(TextFormat("zoom%.1f time%.2f",OldeCam.cam.zoom,d_time),16,tilesize,16,WHITE);
                DrawText(TextFormat("visible tile from x%1.f y%1.f",render.x,render.y),16,tilesize*2,16,WHITE);
                DrawText(TextFormat("visible tile to   x%1.f y%1.f",render.z,render.w),16,tilesize*3,16,WHITE);
                DrawText(TextFormat("camera target x%.2f y%.2f",OldeCam.cam.target.x,OldeCam.cam.target.y),16,tilesize*4,16,WHITE);
                DrawText(TextFormat("max x%d y%d",OldeMap.EditorGetMaxX(),OldeMap.EditorGetMaxY()),16,tilesize*5,16,WHITE);
                DrawText(TextFormat("map %s",OldeSettings.current_map.c_str()),16,tilesize*6,16,WHITE);
                DrawText(TextFormat("tileset: %d",palette_num),16,tilesize*7,16,WHITE);
                DrawText(TextFormat("current: layer=%d tileset=%d",palette_type,OldeMap.EditorWhatTilesetOn(OldeEdit,palette_type)),16,tilesize*8,16,WHITE);
                DrawText(TextFormat("x-render %d",OldeSettings.xrender),16,tilesize*9,16,WHITE);
                DrawText(TextFormat("y-render %d",OldeSettings.yrender),16,tilesize*10,16,WHITE);
                DrawText(TextFormat("z-render %d",OldeSettings.zrender),16,tilesize*11,16,WHITE);
                DrawText(TextFormat("w-render %d",OldeSettings.wrender),16,tilesize*12,16,WHITE);
            }
            if(show_menu) OldeMenu.DrawMenu(OldeAssets,OldeSettings,d_time,debug_mode);
            if(set_save_map) DrawText("Saving map... please wait",100,100,40,WHITE); 
            if(OldeMenu.public_display_editor_show_help) OldeMenu.ShowHelp(OldeAssets);
        EndDrawing();

        if(d_time>=3.0) d_time=0;

        // 
        // d_time needs rework
        // 

        if(show_menu)
        {
            switch(OldeMenu.ProcessMenu())
            {
                case -1:
                if(OldeMenu.public_display_editor_resize)
                {
                    OldeMenu.SetResizeValues(OldeMap.EditorGetMaxX(),OldeMap.EditorGetMaxY());
                    OldeMenu.public_display_editor_resize=false;
                    OldeMenu.public_display_main_menu=true;
                }
                else
                {
                    editor_window=false;
                    secret_rc=1;
                }
                break;

                case 0:
                if(OldeMenu.public_display_main_menu) show_menu=false;
                break;

                case 1:
                if(OldeMenu.public_display_main_menu) set_save_map=true;
                break;

                case 2:
                if(debug_mode && OldeMenu.public_display_main_menu) debug_mode=false;
                else if(!debug_mode && OldeMenu.public_display_main_menu) debug_mode=true;
                if(OldeMenu.public_display_editor_resize)
                {
                    OldeMap.EditorResizeMap(OldeMenu.GetResizedMapX(),OldeMenu.GetResizedMapY());
                    OldeMenu.public_display_editor_resize=false;
                    OldeMenu.public_display_main_menu=true;
                    show_menu=-2;
                }
                break;

                case 3:
                if(OldeMenu.public_display_main_menu)
                {
                    OldeMenu.public_display_main_menu=false;
                    OldeMenu.public_display_editor_resize=true;
                }
                break;

                default: // gets -2 when not pressed enter
                show_menu=true;
                break;
            }
        }

        if(reloadAssets)
        {
            WaitTime(2.0f);
            OldeAssets.ReloadAssets();
            OldeMap.ReloadMap();

            edit_pointer_pos.x=render.x;
            edit_pointer_pos.y=render.y;

            // camera_pointer_pos.x=render.x;
            // camera_pointer_pos.y=render.x;
            reloadAssets=false;
        }

        if(set_save_map)
        {
            // WaitTime(2.0f);
            if(OldeMenu.public_save_map)
            {
                OldeMenu.ShowLoading(OldeAssets,OldeSettings,"Saving");
                OldeMap.SaveMap(OldeSettings);
            }
            OldeMenu.public_save_map=false;
            set_save_map=false;
        }

        if(map_resize)
        {

            map_resize=false;
        }
    }

    return secret_rc;
}