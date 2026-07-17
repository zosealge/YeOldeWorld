#pragma once
#include"raylib.h"
#include"assets.hpp"

class EditTools
{
    public:
    const int tilesize=16;
    const int pngarray=10;
    const int pointer_tile=0;
    int cur_x;
    int cur_y;
    bool is_spawn_set;
    int which_spawn_set;
    EditTools();

    void MovePointer(Vector2 &pos);
    void DrawPointer(Game_Assets &OldeAssets,float &d_time,bool debug_mode);

    private:
    int current_tile;
    int current_draw_offset;
    Rectangle tiles[100]; // always it is better to have more
    Vector2 pointer_pos;

    void DrawDebugPointer();
};