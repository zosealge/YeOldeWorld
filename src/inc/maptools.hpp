#pragma once
#include"raylib.h"
#include<cstdint>
#include<string>
#include<fstream>
#include"data.hpp"
#include"assets.hpp"
#include"edittools.hpp"
#include"player.hpp"
#include"network.hpp"

class MapTools
{
    public:
    const int tilesize=16;
    const int half_tilesize=8;
    const int quarter_tilesize=4;
    const int max_x=4096;
    const int max_y=4096; // maximum size of map array allocated in heap memory
    // map array uses uint16_t size as a block for map x/y info
    // in theory maps can be as large as 65535 x 65535
    const int min_x=16;
    const int min_y=16;
    const uint16_t tile_png_x=10;
    const uint16_t tile_png_y=30;

    const size_t rw_signature64=sizeof(uint64_t);
    const size_t rw_value16=sizeof(uint16_t);
    const std::string maps_folder="maps/";
    const std::string yow_head=".yow";
    // const std::string map_head=".map";
    std::string loaded_filename;

        bool alloc_error;
        bool valid_map;

    uint16_t spawn_point0_x;
    uint16_t spawn_point0_y;

    uint16_t spawn_point1_x;
    uint16_t spawn_point1_y;

    uint16_t spawn_point2_x;
    uint16_t spawn_point2_y;

    uint16_t spawn_point3_x;
    uint16_t spawn_point3_y;

    MapTools(std::string want_to_load_this_map,bool new_map);
    ~MapTools();

    void DrawMap(Game_Assets &OldeAssets,float &d_time,Vector4 &render,bool debug_mode);
    // void DrawObjects(Game_Assets &OldeAssets,float &d_time,Vector4 &render,bool debug_mode);
    // void DrawRoofs(Game_Assets &OldeAssets,Vector2 &pointer_pos,float &d_time,Vector4 &render,bool debug_mode);
    // void DrawEditorRoofs(Game_Assets &OldeAssets,float &d_time,Vector4 &render,bool debug_mode);
    void ReloadMap();
    void NewMap();

    int SaveMap(Game_Data &OldeSettings);
    int LoadMap(std::string what_map);
    void SetMapSpawnPoint(EditTools &OldeEdit,int spawn_num);
    void JumpToSpawnPoint(Vector2 &edit_pointer_pos,int spawn_num);

    void EditorDrawPalette(Game_Assets &OldeAssets,int pallete_sel,int palette_type);
    void EditorPlaceTile(EditTools &OldeEdit,int pallete_sel,int palette_type);
    uint16_t EditorWhatTilesetOn(EditTools &OldeEdit,int palette_type);

    // NETWORKING WITH MAP
    uint16_t GetTileInfoAt(uint16_t x, uint16_t y);
    void PlaceTileInfoAt(uint16_t type,uint16_t x,uint16_t y);
    void PlaceTileByMemPos(uint16_t tile,uint16_t tile_mem);
    void MemcpyTile(uint8_t *buffer,uint16_t type,uint16_t x,uint16_t y,const int frame_size);

    void EditorClearLayer(int layer_num);

    void EditorResizeMap(uint16_t x,uint16_t y);

    uint16_t EditorGetMaxX();
    uint16_t EditorGetMaxY();

    void GameSetMapSize(uint16_t setx,uint16_t sety);
    void GameCheckCollision(Game_Player &OldePlayer,bool debug_mode,bool pl_action);

    bool IsMapValid();

    private:
    uint16_t max_size_x;
    uint16_t max_size_y;

    uint16_t editor_max_palette_y;
    // Rectangle R_Walls[520];
    // Rectangle R_Objects[520];
    // Rectangle R_Roofs[520];
    Rectangle R_Map[520];
    // Rectangle Collisions[32];
    Rectangle P_Collision;
    // Rectangle O_Collision;
    // Rectangle W_Collision;
    Vector2 tileset_curr;
    Vector2 draw_pos;
    Vector4 draw_frame;
    Vector4 draw_tile;
    // Vector4 draw_frame_obj;
    Vector4 draw_object;

    // THIS IS THE MAP ARRAY POINTER
    uint16_t *map_arr;
    // uint16_t *obj_arr;
    // uint16_t *roofs_arr;
    // USE WITH CARE
    
    void FirstSet();
    void Wipe();
    void ZeroMap();
};