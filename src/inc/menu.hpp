#pragma once
#include"raylib.h"
#include<vector>
#include<cstdint>
#include<string>
#include<cstring>
#include"assets.hpp"
#include"data.hpp"

class Game_Menu
{
    public:
    const int menu_tilesize=64;
    const int max_map_x=4096;
    const int max_map_y=4096;
    const int min_map_x=16;
    const int min_map_y=16;

    // const std::string maps_folder="maps/";
    std::vector<std::string> map_list;
    bool public_enter_was_pressed;
    bool public_display_main_menu;
    bool public_display_options;
    bool public_display_load_menu;
    bool public_display_connect_menu;
    bool public_display_options_menu;
    bool public_display_editor_resize;
    bool public_display_editor_show_help;
    bool public_save_map;

    Game_Menu(int what_menu);
    ~Game_Menu();
    void Control();
    void DrawMenu(Game_Assets &OldeAssets,Game_Data &OldeSettings,float &d_time,bool debug_mode);
    void DrawSprite(Game_Assets&OldeAssets,float &d_time);
    void DrawChoiceSprite(Game_Assets &OldeAssets,float &d_time,Vector2 &pos_v);
    // void DrawLoadMenu(Game_Assets &OldeAssets,Game_Data &OldeSettings);
    int ProcessMenu();
    void SetResizeValues(uint16_t x,uint16_t y);

    // use ShowLoading() only after EndDrawing() 
    void ShowLoading(Game_Assets &OldeAssets,Game_Data &OldeSettings,std::string wait_for);

    // client menus
    void ShowHostInfo(Game_Assets &OldeAssets,Game_Data &OldeSettings);
    void PassIpAddress(std::string ip_a);

    // editor menus
    void TypeNewMapName(Game_Assets &OldeAssets,Game_Data &OldeSettings);
    // void ShowSaving(Game_Assets &OldeAssets,Game_Data &OldeSettings);
    void ShowHelp(Game_Assets &OldeAssets); 

    // common menus
    void AcceptMap(Game_Data &OldeSettings,int sel);

    int IsExit();

    uint16_t GetResizedMapX();
    uint16_t GetResizedMapY();
    
    private:
    int which_menu;
    int which_pos;
    // 0 - main menu
    // 1 - in_editor menu
    // 2 - game client main menu
    // 3 - in_game client menu

    int option_attr[8];

    int cursor_pos;
    int how_many_entries;
    float y_spanning;
    float font_size;
    bool enter_was_pressed;
    bool debug_mode;
    int rc_enter;

    std::vector<std::string> menu_names;
    std::vector<std::string> options_list;
    std::vector<std::string> connection_list;
    std::vector<std::string> resize_list;
    Vector2 menu_pos_v;
    Vector2 sprite_pos_v;
    Vector2 roofs_pos_v;
    Vector2 editor_help_v;
    // Rectangle menu_background_square;
    FilePathList maps_dir;
    float menu_pos_y_set;

    void LoadMapList();
    // void SetHeight(int set);

    void TypeIPAddr(Game_Data &OldeSettings);
    // char ip_addr[16];
    std::string ip_addr_new;
    int ip_name_char_num;

    void ChangeResX();
    void ChangeResY();
    int res_x_char_num;
    int res_y_char_num;

    int sprite_num;
    int res_max_x;
    int res_max_y;
};