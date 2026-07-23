                                                                                                                                  
// YeOldeWorld Game Client

// there is no enums or something like that yet
// directions are lower case char 'w' 's' 'a' 'd' letters - everybody knows whats up
// actions are 'i'dle 'w'alk 'a'ttack 'u'se - lower case char

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
#include"inc/networkclient.hpp"
#include"inc/data.hpp"
#include"inc/assets.hpp"
#include"inc/menu.hpp"
#include"inc/camera.hpp"
#include"inc/gui.hpp"
#include"inc/player.hpp"
#include"inc/maptools.hpp"

enum draw_layers:int
{
    ground=0,
    above_heads=1
};

int Set_Game(Game_Assets &OldeAssets,Game_Data &OldeSettings,NetworkClient &OldeNet,float &d_time);

//  M   M EEEE N   N U   U
//  MM MM E    NN  N U   U
//  M M M EEE  N N N U   U
//  M   M E    N  NN U   U
//  M   M EEEE N   N  UUU 

int main()
{
    const std::string   OldeTitle="Olde";
    Game_Data           OldeSettings;
    if(!OldeSettings.IsConfigValid())
    {
        return -1;
    }
    NetworkClient       OldeNet;
    // Because YeOldeWorlde is build as a multiplayer game
    // every function needs to be considered as relevant to host communication
    InitWindow(OldeSettings.main_screen_width,OldeSettings.main_screen_height,OldeTitle.c_str());
    Game_Menu           OldeMenu(2);
    OldeMenu.PassIpAddress(OldeSettings.ip_address);
    Game_Assets         OldeAssets;
    SetTargetFPS(OldeSettings.set_max_fps);
    bool main_game=true;
    bool debug_mode=false;
    bool connect_to_host=false;
    // bool test_map=false; // will be deleted later
    float d_time=0.0f;


    bool net=OldeNet.NetworkInitialize();
    if(!net) return -1;

    int rc_menu=-2;

    while(main_game)
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

            case -1: // ALWAYS AN EXIT BUTTON
                if(OldeMenu.public_display_main_menu) main_game=false;
                if(OldeMenu.public_display_connect_menu)
                {
                    OldeMenu.public_display_main_menu=true;
                    OldeMenu.public_display_connect_menu=false;
                    rc_menu=-2;
                }
                if(OldeMenu.public_display_options_menu)
                {
                    OldeMenu.public_display_options_menu=false;
                    OldeMenu.public_display_main_menu=true;
                }
            break;

            case 0:
                if(OldeMenu.public_display_connect_menu)
                {
                    OldeMenu.public_display_connect_menu=false;
                    OldeMenu.public_display_main_menu=true;
                    connect_to_host=true;
                    rc_menu=-2;
                }

                if(OldeMenu.public_display_main_menu)
                {
                    OldeMenu.public_display_main_menu=false;
                    OldeMenu.public_display_connect_menu=true;
                    rc_menu=-2;
                }
            break;

            case 1:
                if(OldeMenu.public_display_main_menu)
                {
                    OldeMenu.public_display_main_menu=false;
                    OldeMenu.public_display_options_menu=true;
                }
                // if(OldeMenu.public_display_connect_menu)
                // {
                //     OldeMenu.public_display_connect_menu=false;
                //     OldeMenu.public_display_main_menu=true;
                //     connect_to_host=false;
                //     test_map=true;
                //     rc_menu=-2;
                // }
            break;

            case 2:
                if(OldeMenu.public_display_options_menu)
                {
                    if(!IsWindowFullscreen()) OldeSettings.ChangeResolution();
                }
            break;

            case 3:
                if(OldeMenu.public_display_options_menu)
                {
                    OldeSettings.SetScreenMode();
                }
            break;

            default:
                if(OldeMenu.public_display_load_menu)
                {
                    // OldeMenu.AcceptMap(OldeSettings,rc_menu);
                    // printf("OLDE: accepted map > %s\n",OldeSettings.current_map.c_str());
                    OldeSettings.editor_new_map=false;
                    OldeMenu.public_display_load_menu=false;
                    OldeMenu.public_display_main_menu=true;
                    connect_to_host=true;
                    rc_menu=-2;
                }
            break;
        }

        if(connect_to_host)
        {
            if(OldeSettings.IsAddrIPNotEmpty())
            {
                uint8_t selected_avatar=OldeSettings.local_player_avatar;
                OldeSettings.connection_to_host=true;
                OldeMenu.ShowLoading(OldeAssets,OldeSettings,"Connecting");
                if(OldeNet.NetworkConnectToHost(OldeSettings.GetAddrIP(),selected_avatar))
                {
                    Set_Game(OldeAssets,OldeSettings,OldeNet,d_time);
                    OldeMenu.public_display_connect_menu=false;
                    OldeMenu.public_display_main_menu=true;
                }
                OldeSettings.connection_to_host=false;
            }
            // OldeNet.EndConnection();
            connect_to_host=false;
        }

        // if(test_map)
        // {
        //     OldeSettings.current_map="test.map";
        //     OldeMenu.ShowLoading(OldeAssets,OldeSettings,"Testing");
        //     Set_Game(OldeAssets,OldeSettings,OldeNet,d_time);
        //     OldeMenu.public_display_connect_menu=false;
        //     OldeMenu.public_display_main_menu=true;
        //     test_map=false;
        // }
    }
    OldeNet.NetworkDeinitialize();
    return 0;
}

//   CCC L    III EEEE N   N TTTTTT
//  C    L     I  E    NN  N   TT  
//  C    L     I  EEE  N N N   TT  
//  C    L     I  E    N  NN   TT  
//   CCC LLLL III EEEE N   N   TT                                                  

int Set_Game(Game_Assets &OldeAssets,Game_Data &OldeSettings,NetworkClient &OldeNet,float &d_time)
{
    const int tilesize=16;
    // const int buffersize=256;

    Game_Player OldePlayer;
    
    float speed_player_move=0.5f; // move this to player class
    // float speed_camera_movement=0.2f;
    Vector2 localplayer_pointer_pos{};
    // Vector2 camera_localplayer_pos{};
    Vector2 mouse_pos{};
    Vector4 render{};

    PlayerCamera OldeCam;
    PlayerGui OldeGui;
    Game_Menu OldeMenu(3);
    MapTools OldeMap(OldeSettings.current_map,true); // load new map
    
    bool game_window=true;
    bool show_menu=false;
    bool debug_mode=false;
    // bool set_load_map=false;
    bool print_states=false; // TEMPORARY DEBUG
    int print_states_debug=0;
    // bool reloadAssets=false;

    bool local_player_action=false;
    int secret_rc=0;
    int local_player_id=-1; // this number will be assigned by server
                            // for testing purposes
                            // same value will be used
                            // in OldeNet.LocalPlayerId
                            // and
                            // OldePlayer.player_num 

    // Vector4 for rendering window size
    // using floats!!!
    // spawn points loaded from map are int tiled map points! - always multipy those by tilesize!!!
    // at size 4.0 rendering x13 and y9
    // allows for filling whole screen
    // lets load this point from Oldemap.spawn_point0_x and _y


    while(OldeNet.LocalPlayerId==-1)
    {
        // check all the incoming packets and dipatch
        // network commands before game loop starts
        OldeNet.Update(GetTime(),GetFrameTime(),OldeMap,OldePlayer);
    }
    
    for(int i=0;i<max_clients;i++)
    {
        if(i==local_player_id) continue;
        else
        {
            OldePlayer.pl_active[i]=OldeNet.Clients[i].Active;
            OldePlayer.pl_pointer_pos[i]=OldeNet.Clients[i].Position;
            OldePlayer.pl_dir[i]=OldeNet.Clients[i].direction;
            OldePlayer.pl_act[i]=OldeNet.Clients[i].action;
            OldePlayer.pl_avatar[i]=OldeNet.Clients[i].avatar;
            OldePlayer.pl_side[i]=0;
        }
    }



    local_player_id=OldeNet.LocalPlayerId;
    OldePlayer.SetLocalPlayerNumber(OldeNet.LocalPlayerId,OldeNet.LocalPlayerAvatar);


    render.x=OldeSettings.xrender*tilesize;
    render.y=OldeSettings.yrender*tilesize;
    render.z=OldeSettings.zrender*tilesize;
    render.w=OldeSettings.wrender*tilesize;

    OldePlayer.pl_pointer_pos[local_player_id].x=(float)OldeNet.local_spawn_point_x*tilesize;
    OldePlayer.pl_pointer_pos[local_player_id].y=(float)OldeNet.local_spawn_point_y*tilesize;

    OldePlayer.MoveLocalPlayer(OldePlayer.pl_pointer_pos[local_player_id]);
    // OldePlayer.MoveLocalPlayer(localplayer_pointer_pos);

    OldeCam.cam.target.x=OldePlayer.pl_pointer_pos[local_player_id].x; // -32
    OldeCam.cam.target.y=OldePlayer.pl_pointer_pos[local_player_id].y-32;

    // OldeCam.cam.target.x=localplayer_pointer_pos.x; // -32
    // OldeCam.cam.target.y=localplayer_pointer_pos.y-32;

    // Initialize network ahead

    bool auto_move=false;

    while(game_window)
    {
        // OldeNet.ReceivePackets(OldePlayer);
        mouse_pos=GetMousePosition();
        d_time+=GetFrameTime();
        if(IsKeyPressed(KEY_ESCAPE)) show_menu=true;



        //POINTER
        // float speed_player_move=1.0f;
        // float speed_camera_movement=1.0f;
        if(IsKeyDown(KEY_UP) && !show_menu)
        {
            OldePlayer.pl_pointer_pos[local_player_id].y-=speed_player_move;
            OldePlayer.pl_dir[local_player_id]='w';
            OldePlayer.pl_act[local_player_id]='w';
            local_player_action=true;
            if(OldePlayer.pl_pointer_pos[local_player_id].y<0.0) OldePlayer.pl_pointer_pos[local_player_id].y=0.0;
        }
        if(IsKeyDown(KEY_DOWN) && !show_menu) 
        {
            OldePlayer.pl_pointer_pos[local_player_id].y+=speed_player_move;
            OldePlayer.pl_dir[local_player_id]='x';
            OldePlayer.pl_act[local_player_id]='w';
            local_player_action=true;
            if(OldePlayer.pl_pointer_pos[local_player_id].y>(OldeMap.EditorGetMaxY()-1)*tilesize) OldePlayer.pl_pointer_pos[local_player_id].y=(OldeMap.EditorGetMaxY()-1)*tilesize;
        }
        if(IsKeyDown(KEY_LEFT) && !show_menu)
        {
            OldePlayer.pl_pointer_pos[local_player_id].x-=speed_player_move;
            OldePlayer.pl_dir[local_player_id]='a';
            OldePlayer.pl_act[local_player_id]='w';
            local_player_action=true;
            if(OldePlayer.pl_pointer_pos[local_player_id].x<0.0) OldePlayer.pl_pointer_pos[local_player_id].x=0.0;
        }
        if(IsKeyDown(KEY_RIGHT) && !show_menu)
        {
            OldePlayer.pl_pointer_pos[local_player_id].x+=speed_player_move;
            OldePlayer.pl_dir[local_player_id]='d';
            OldePlayer.pl_act[local_player_id]='w';
            local_player_action=true;
            if(OldePlayer.pl_pointer_pos[local_player_id].x>(OldeMap.EditorGetMaxX()-1)*tilesize) OldePlayer.pl_pointer_pos[local_player_id].x=(OldeMap.EditorGetMaxX()-1)*tilesize;
        }

        if(IsKeyDown(KEY_UP) && IsKeyDown(KEY_LEFT) && !show_menu)
        {
            OldePlayer.pl_dir[local_player_id]='q';
        }
        if(IsKeyDown(KEY_UP) && IsKeyDown(KEY_RIGHT) && !show_menu)
        {
            OldePlayer.pl_dir[local_player_id]='e';
        }
        if(IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_LEFT) && !show_menu)
        {
            OldePlayer.pl_dir[local_player_id]='z';
        }
        if(IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_RIGHT) && !show_menu)
        {
            OldePlayer.pl_dir[local_player_id]='c';
        }

        if(IsKeyDown(KEY_LEFT_SHIFT) && !show_menu)
        {
            speed_player_move=1.0f;
        }

        if(IsKeyUp(KEY_LEFT_SHIFT))
        {
            speed_player_move=0.5f;
        }

        if(IsKeyPressed(KEY_SPACE) && !show_menu)
        {
            OldePlayer.pl_act[local_player_id]='t';
            local_player_action=true;
        }

        if(IsKeyPressed(KEY_LEFT_CONTROL) && !show_menu)
        {
            OldePlayer.pl_act[local_player_id]='a';
            local_player_action=true;
        }


        if(IsKeyPressed(KEY_INSERT))
        {
            auto_move=true;
        }

        if(auto_move) // TEST ONLY
        {
            OldePlayer.pl_pointer_pos[local_player_id].x+=speed_player_move;
            OldePlayer.pl_dir[local_player_id]='d';
            OldePlayer.pl_act[local_player_id]='w';
            local_player_action=true;
            if(OldePlayer.pl_pointer_pos[local_player_id].y<0.0) OldePlayer.pl_pointer_pos[local_player_id].y=0.0;
        }

        OldePlayer.MoveLocalPlayer(OldePlayer.pl_pointer_pos[local_player_id]);

        // CAMERA

        OldeCam.Move(OldePlayer.pl_pointer_pos[local_player_id]);

        // RENDERING FRAME

        render.x=OldeCam.cam.target.x-(OldeSettings.xrender*tilesize);
        render.y=OldeCam.cam.target.y-(OldeSettings.yrender*tilesize);
        render.z=OldeCam.cam.target.x+(OldeSettings.zrender*tilesize);
        render.w=OldeCam.cam.target.y+(OldeSettings.wrender*tilesize);

        if(show_menu) OldeMenu.Control();

        // UPDATE NETWORK

        OldeNet.PrepareLocalClient(OldePlayer.pl_pointer_pos[local_player_id],
                                  OldePlayer.pl_dir[local_player_id],
                                  OldePlayer.pl_act[local_player_id],
                                  OldePlayer.cur_x,OldePlayer.cur_y,
                                  OldePlayer.pl_avatar[local_player_id]);

                                  // will be reworked into a struct when prototyping will be done
                            
        for(int i=0;i<max_clients;i++)
        {
            if(i==local_player_id) continue;
            else
            {
                OldePlayer.pl_active[i]=OldeNet.Clients[i].Active;
                OldePlayer.pl_pointer_pos[i]=OldeNet.Clients[i].Position;
                OldePlayer.pl_dir[i]=OldeNet.Clients[i].direction;
                OldePlayer.pl_act[i]=OldeNet.Clients[i].action;
                OldePlayer.pl_avatar[i]=OldeNet.Clients[i].avatar;
            }
        }
                            
        OldeNet.Update(GetTime(),GetFrameTime(),OldeMap,OldePlayer);
        
        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode2D(OldeCam.cam);
                OldeMap.DrawMapLayer(OldeAssets,d_time,render,debug_mode,ground);
                OldeMap.GameCheckCollision(OldePlayer,debug_mode,speed_player_move);
                // OldePlayer.DrawLocalPlayer(OldeAssets,d_time,debug_mode);
                OldePlayer.DrawPlayers(OldeAssets,d_time);
                OldeMap.DrawMapLayer(OldeAssets,d_time,render,debug_mode,above_heads);

                // if(!debug_hide_objects) OldeMap.DrawObjects(OldeAssets,d_time,render,debug_mode);
                // OldeMap.DrawRoofs(OldeAssets,OldePlayer.pl_pointer_pos[local_player_id],d_time,render,debug_mode);
            EndMode2D();

            // OldeGui.DrawGui();
            // will be reworked into drawn ones

            if(debug_mode)
            {
                DrawText(TextFormat("connected to %s:%d",OldeSettings.ip_address.c_str(),network_port),16,tilesize,16,WHITE);
                DrawText(TextFormat("visible tile from x%1.f y%1.f",render.x,render.y),16,tilesize*2,16,WHITE);
                DrawText(TextFormat("visible tile to   x%1.f y%1.f",render.z,render.w),16,tilesize*3,16,WHITE);
                DrawText(TextFormat("camera target x%.2f y%.2f",OldeCam.cam.target.x,OldeCam.cam.target.y),16,tilesize*4,16,WHITE);
                DrawText(TextFormat("time %0.3f",d_time),16,tilesize*5,16,WHITE);
                DrawText(TextFormat("map %s",OldeSettings.current_map.c_str()),16,tilesize*6,16,WHITE);
                DrawText(TextFormat("int   local player x=%d y=%d",     OldePlayer.cur_x,OldePlayer.cur_y),16,tilesize*7,16,WHITE);
                DrawText(TextFormat("float local player x=%0.2f y=%0.2f",localplayer_pointer_pos.x,localplayer_pointer_pos.y),16,tilesize*8,16,WHITE);
                if(OldePlayer.local_collide) DrawText("Collision!",16,tilesize*9,16,WHITE);
                DrawText(TextFormat("network player number=%d",local_player_id),16,tilesize*10,16,WHITE);
                DrawText(TextFormat("network map x=%d y=%d",OldeMap.EditorGetMaxX(),OldeMap.EditorGetMaxY()),16,tilesize*11,16,WHITE);
            }
            if(show_menu) OldeMenu.DrawMenu(OldeAssets,OldeSettings,d_time,debug_mode);
        EndDrawing();

        if(OldePlayer.local_collide)
        {
            if(OldePlayer.pl_dir[local_player_id]=='w') OldePlayer.pl_pointer_pos[local_player_id].y+=speed_player_move;
            if(OldePlayer.pl_dir[local_player_id]=='x') OldePlayer.pl_pointer_pos[local_player_id].y-=speed_player_move;
            if(OldePlayer.pl_dir[local_player_id]=='a') OldePlayer.pl_pointer_pos[local_player_id].x+=speed_player_move;
            if(OldePlayer.pl_dir[local_player_id]=='d') OldePlayer.pl_pointer_pos[local_player_id].x-=speed_player_move;

            if(OldePlayer.pl_dir[local_player_id]=='q')
            {
                OldePlayer.pl_pointer_pos[local_player_id].x+=speed_player_move;
                OldePlayer.pl_pointer_pos[local_player_id].y+=speed_player_move;
            }
            if(OldePlayer.pl_dir[local_player_id]=='e')
            {
                OldePlayer.pl_pointer_pos[local_player_id].x-=speed_player_move;
                OldePlayer.pl_pointer_pos[local_player_id].y+=speed_player_move;
            }
            if(OldePlayer.pl_dir[local_player_id]=='z')
            {
                OldePlayer.pl_pointer_pos[local_player_id].x+=speed_player_move;
                OldePlayer.pl_pointer_pos[local_player_id].y-=speed_player_move;
            }
            if(OldePlayer.pl_dir[local_player_id]=='c')
            {
                OldePlayer.pl_pointer_pos[local_player_id].x-=speed_player_move;
                OldePlayer.pl_pointer_pos[local_player_id].y-=speed_player_move;
            }
        }
        OldePlayer.local_collide=false;
        OldePlayer.local_use=false;
        if(d_time>=3.0) d_time=0;

        // 
        // d_time needs rework
        // 

        if(show_menu)
        {
            switch(OldeMenu.ProcessMenu())
            {
                case -1:
                game_window=false;
                secret_rc=1;
                break;

                case 0:
                show_menu=false;
                break;

                case 1:
                if(debug_mode) debug_mode=false;
                else if(!debug_mode) debug_mode=true;
                break;

                default: // gets -2 when not pressed enter
                show_menu=true;
                break;
            }
        }

        if(OldeNet.server_disconnect)
        {
            game_window=false;
            secret_rc=0;
        }

        if(local_player_action)
        {
            OldePlayer.pl_act[local_player_id]='i';
            local_player_action=false;
        }

        if(print_states)
        {
            // struct NetworkRemoteClientInfo
            // {
            //     bool Active;
            //     Vector2 Position;
            //     // Vector2 Direction;
            //     double UpdateTime;
            //     Vector2 ExtrapolatedPosition;
            //     uint8_t direction;
            //     uint8_t action;
            //     uint8_t hands;
            //     uint8_t backpack_slot0;
            //     uint8_t backpack_slot1;
            //     uint8_t backpack_slot2;
            // };
            printf("#######################\n");
            printf("####new print below####\n");
            printf("####      %d       ####\n",print_states_debug);
            for(int i=0;i<max_clients;i++)
            {
                printf("\n");
                printf("client %d bool active: %d\n",i,OldeNet.Clients[i].Active);
                printf("client %d position:    x%f y%f\n",i,OldeNet.Clients[i].Position.x,OldeNet.Clients[i].Position.y);
                printf("client %d direction:   %d\n",i,OldeNet.Clients[i].direction);
                printf("client %d action       %d\n",i,OldeNet.Clients[i].action);
                printf("client %d hands        %d\n",i,OldeNet.Clients[i].hands);
                printf("client %d bp slot 0    %d\n",i,OldeNet.Clients[i].backpack_slot0);
                printf("client %d bp slot 1    %d\n",i,OldeNet.Clients[i].backpack_slot1);
                printf("client %d bp slot 2    %d\n",i,OldeNet.Clients[i].backpack_slot2);
                printf("\n");
            }
            print_states=false;
            print_states_debug++;
        }
    }

    if(!OldeNet.server_disconnect) OldeNet.Disconnect();
    OldeMap.CleanUp();
    OldeNet.LocalPlayerId=-1;
    return secret_rc;
}