#pragma once
#include"raylib.h"
#include<iostream>
#include<fstream>
#include<string>
#include<cstdint>

class Game_Data
{
    public:
    const uint8_t max_avatars=6;
    int main_screen_width;
    int main_screen_height;
    int set_max_fps;

    std::string current_map;
    std::string ip_address;
    bool editor_new_map;
    bool connection_to_host;
    bool cfg_file_loaded;

    int8_t local_player_num;
    uint8_t local_player_avatar;
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

    bool IsConfigValid();

    private:
    size_t GetLengthOfEntry(std::string &str,size_t start);
};