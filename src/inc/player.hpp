#pragma once
#include"raylib.h"
#include"assets.hpp"
#include"network.hpp"

class Game_Player
{
    public:
    const int tilesize=16;
    const int pngarraywidth=10;
    const int pngarrayheight=60;
    int player_num;
    char pl_dir[max_clients]{};
    char pl_act[max_clients]{};
    Vector2 pl_pointer_pos[max_clients]{};
    bool pl_active[max_clients]{};
    Rectangle local_collision;
    bool local_collide;
    bool local_use;
    

    int cur_x;
    int cur_y;

    Game_Player();


    void MoveLocalPlayer(Vector2 &pos);
    // void MovePlayers();
    void DrawLocalPlayer(Game_Assets &OldeAssets,float &d_time,bool debug_mode);
    void DrawNetworkPlayer(Game_Assets &OldeAssets,float &d_time); // no debug for network players?

    void Collide(float speed);
    void SetPlayerNumber(int num);

    private:
    int current_tile_left;
    int current_tile_right;
    int current_draw_offset;
    Vector2 local_player_pos;
    Rectangle tiles[700]; // 60x10=600 but with extra space just in case
};