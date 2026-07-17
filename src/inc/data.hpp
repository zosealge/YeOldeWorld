#pragma once
#include"raylib.h"
#include<string>
#include<cstdint>

class Game_Data
{
    public:
    int main_screen_width;
    int main_screen_height;
    int set_max_fps;

    std::string current_map;
    std::string ip_address;
    bool editor_new_map;
    bool connection_to_host;

    int8_t local_player_num;
    int resolution_selection;
    int fullscreen_selection;
    int xrender;
    int yrender;
    int zrender;
    int wrender;

    int editor_max_map_x;
    int editor_max_map_y;


    Game_Data();
    void ChangeResolution();
    void SetScreenMode();
    void SetRender720p();
    void SetRender1080p();
    bool IsAddrIPNotEmpty();
    std::string GetAddrIP();
};