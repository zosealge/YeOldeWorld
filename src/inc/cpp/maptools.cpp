// hello!
    
    // seems You are courious about saving/loading map section
    
    // map files are always in two files: *.yow and *.map,
    // *.yow format keeps map signature, size, calendar,
    // player spawn points and other needed information are stored in binary format.

    // memory written to file in this order:
    // uint64_t signature;
    // uint16_t max_size_x;
    // uint16_t max_size_y;
    // uint16_t spawn_point0_x;
    // uint16_t spawn_point0_x;
    // uint16_t spawn_point1_x;
    // uint16_t spawn_point1_x;
    // uint16_t spawn_point2_x;
    // uint16_t spawn_point2_x;
    // uint16_t spawn_point3_x;
    // uint16_t spawn_point3_x;

    // spawn point in pixel values - these will be casted to float

    // *.map format keeps the map_arr information in uint16_t block of memory,
    // constructed as max_size_x*max_size_y uint16_t grid.

    // map memory written in map file in this order
    // uint64_t signature;
    // uint16_t t; - map       - as map tile

    // add layer in save and loading:
    // uint16_t l; - layer     - another layer
    // etc etc....

    // use map with y*max_y+x  

#include"../maptools.hpp"

MapTools::MapTools(std::string want_to_load_this_map,bool new_map)
{
    alloc_error=false;
    map_arr=new(std::nothrow)       uint16_t[max_x*max_y];
    if(map_arr==nullptr)
    {
        printf("MAPTOOLS: map array allocation error.\n");
        alloc_error=true;
    }
    loaded_filename=want_to_load_this_map;

    if(!alloc_error)
    {
            // memset in the future ???
            for(int y=0;y<max_y;y++)
            {
                for(int x=0;x<max_x;x++)
                {
                    map_arr[y*max_y+x]=0;
                }
            }

            P_Collision.x=0;
            P_Collision.y=0;
            P_Collision.width=tilesize;
            P_Collision.height=tilesize;

            tileset_curr.x=0;
            tileset_curr.y=0;
            int curr_tile=0;
            for(int y=0;y<tile_png_y;y++)
            {
                for(int x=0;x<tile_png_x;x++)
                {
                    R_Map[curr_tile].x=tileset_curr.x;
                    R_Map[curr_tile].y=tileset_curr.y;
                    R_Map[curr_tile].width=tilesize;
                    R_Map[curr_tile].height=tilesize;
                    tileset_curr.x+=tilesize;
                    curr_tile++;
                }
                tileset_curr.y+=tilesize;
            }

            if(new_map)
            {
                FirstSet();
                max_size_x=32;
                max_size_y=32;
            }
            else
            {
                LoadMap(loaded_filename);
            }

    }
}

MapTools::~MapTools()
{
    if(!alloc_error)
    {
        // will be changed in future
        for(int y=0;y<max_y;y++)
        {
            for(int x=0;x<max_x;x++)
            {
                map_arr[y*max_y+x]=0;
            }
        }
        Wipe();
    }
    delete []map_arr;
}

// DRAW

void MapTools::DrawMap(Game_Assets &OldeAssets,float &d_time,Vector4 &render,bool debug_mode)
{   
    draw_frame.x=render.x;
    draw_frame.y=render.y;
    draw_frame.z=render.z;
    draw_frame.w=render.w;

    draw_tile.x=render.x/tilesize;
    draw_tile.y=render.y/tilesize;
    draw_tile.z=render.z/tilesize;
    draw_tile.w=render.w/tilesize;

    int t=0;
    for(int y=(int)draw_tile.y;y<(int)draw_tile.w;y++)
    {
        for(int x=(int)draw_tile.x;x<(int)draw_tile.z;x++)
        {
            draw_pos.x=(float)x*tilesize;
            draw_pos.y=(float)y*tilesize;
            if(x < 0 || y < 0 || x > max_size_x-1 || y > max_size_y-1)
            {
                DrawTextureRec(OldeAssets.MapSprite,R_Map[0],draw_pos,WHITE);
            }
            else
            {
                t=map_arr[y*max_y+x];
                switch(t)
                {
                    case 0:
                        // DrawTextureRec(OldeAssets.GroundSprite,R_Walls[9],draw_pos,WHITE);
                        // do nothing for now
                    break;

                    // case 40:
                    // case 50: // first tile of water
                    // case 60:
                    // case 70:
                    // case 80:
                    // case 90:
                    // case 100:
                    // case 140:
                    //     int t_ani;
                    //     if(d_time>0.0 && d_time<0.5) t_ani=0;
                    //     if(d_time>0.5 && d_time<1.0) t_ani=1;
                    //     if(d_time>1.0 && d_time<1.5) t_ani=2;
                    //     if(d_time>1.5 && d_time<2.0) t_ani=3;
                    //     if(d_time>2.0 && d_time<2.5) t_ani=4;
                    //     if(d_time>2.5)               t_ani=5;
                    //     DrawTextureRec(OldeAssets.GroundSprite,R_Walls[t+t_ani],draw_pos,WHITE);
                    // break;

                    default:
                        DrawTextureRec(OldeAssets.MapSprite,R_Map[t],draw_pos,WHITE);
                    break;
                }
            }
        }
    }


    // DrawTextureRec(OldeAssets.GroundSprite,R_Walls[21],(Vector2){(float)draw_tile.x,(float)draw_tile.y},WHITE);

    // DrawRectangle(draw_tile.x,draw_tile.y,tilesize,tilesize,YELLOW);

    if(debug_mode)
    {
        DrawText(TextFormat("%1.f",draw_tile.x),draw_frame.x,draw_frame.y,8,RED);
        DrawText(TextFormat("%1.f",draw_tile.y),draw_frame.x,draw_frame.y+8,8,RED);

        DrawText(TextFormat("%1.f",draw_tile.z),draw_frame.z,draw_frame.w,8,RED);
        DrawText(TextFormat("%1.f",draw_tile.w),draw_frame.z,draw_frame.w+8,8,RED);

        DrawText(TextFormat("%1.f",draw_tile.z),draw_frame.z,draw_frame.y,8,RED);
        DrawText(TextFormat("%1.f",draw_tile.y),draw_frame.z,draw_frame.y+8,8,RED);

        DrawText(TextFormat("%1.f",draw_tile.x),draw_frame.x,draw_frame.w,8,RED);
        DrawText(TextFormat("%1.f",draw_tile.w),draw_frame.x,draw_frame.w+8,8,RED);

        DrawLine((int)draw_tile.x*tilesize,(int)draw_tile.y*tilesize,(int)draw_tile.z*tilesize,(int)draw_tile.y*tilesize,WHITE);
        DrawLine((int)draw_tile.z*tilesize,(int)draw_tile.y*tilesize,(int)draw_tile.z*tilesize,(int)draw_tile.w*tilesize,WHITE);
        DrawLine((int)draw_tile.z*tilesize,(int)draw_tile.w*tilesize,(int)draw_tile.x*tilesize,(int)draw_tile.w*tilesize,WHITE);
        DrawLine((int)draw_tile.x*tilesize,(int)draw_tile.w*tilesize,(int)draw_tile.x*tilesize,(int)draw_tile.y*tilesize,WHITE);

        DrawLineDashed((Vector2){draw_tile.x*tilesize,draw_tile.y*tilesize},(Vector2){draw_tile.z*tilesize,draw_tile.y*tilesize},8,4,YELLOW);
        DrawLineDashed((Vector2){draw_tile.z*tilesize,draw_tile.y*tilesize},(Vector2){draw_tile.z*tilesize,draw_tile.w*tilesize},8,4,YELLOW);
        DrawLineDashed((Vector2){draw_tile.z*tilesize,draw_tile.w*tilesize},(Vector2){draw_tile.x*tilesize,draw_tile.w*tilesize},8,4,YELLOW);
        DrawLineDashed((Vector2){draw_tile.x*tilesize,draw_tile.w*tilesize},(Vector2){draw_tile.x*tilesize,draw_tile.y*tilesize},8,4,YELLOW);
    }
}

void MapTools::GameCheckCollision(Game_Player &OldePlayer,bool debug_mode,bool pl_action)
{
    uint16_t what_on_map=0;
    for(int y=(OldePlayer.cur_y-1);y<(OldePlayer.cur_y+3);y++)
    {
        for(int x=(OldePlayer.cur_x-1);x<(OldePlayer.cur_x+3);x++)
        {
            what_on_map=map_arr[y*max_y+x];
            switch(what_on_map)
            {
                case 21:
                case 22:
                case 23:
                case 24:
                case 25:
                case 30:
                case 32:
                case 34:
                case 35:
                case 40:
                case 46:
                case 50:
                case 60:
                case 100:
                case 140:
                    P_Collision.x=x*tilesize;
                    P_Collision.y=y*tilesize;
                    if(debug_mode)
                    {
                        if(CheckCollisionRecs(OldePlayer.local_collision,P_Collision)) DrawRectangleRec(P_Collision,RED);
                        else
                        {
                            // if(what_on_map==77) DrawRectangleRec(P_Collision,GREEN);
                            DrawRectangleRec(P_Collision,WHITE);
                        }
                    }
                break;

                default:
                    P_Collision.x=-16.0f;
                    P_Collision.y=-16.0f;
                break;
            }
            if(CheckCollisionRecs(OldePlayer.local_collision,P_Collision))
            {
                OldePlayer.local_collide=true;
            }





            // if(debug_mode)
            // {
            //     if(CheckCollisionRecs(OldePlayer.local_collision,O_Collision) || CheckCollisionRecs(OldePlayer.local_collision,W_Collision))
            //     {
            //         DrawRectangleRec(O_Collision,RED);
            //         DrawRectangleRec(W_Collision,RED);
            //     } 
            //     else
            //     {
            //         DrawRectangleRec(O_Collision,WHITE);
            //         DrawRectangleRec(W_Collision,WHITE);
            //     }
            // }
            // if(CheckCollisionRecs(OldePlayer.local_collision,O_Collision))
            // {
            //     OldePlayer.local_collide=true;
            // }
            // if(CheckCollisionRecs(OldePlayer.local_collision,W_Collision))
            // {
            //     OldePlayer.local_collide=true;
            // }
        }
    }
}

void MapTools::ReloadMap()
{
    for(int y=0;y<max_y;y++)
    {
        for(int x=0;x<max_x;x++)
        {
            map_arr[y*max_y+x]=0;            
        }
    }

    tileset_curr.x=0;
    tileset_curr.y=0;
    int curr_tile=0;
    for(int y=0;y<tile_png_y;y++)
    {
        for(int x=0;x<tile_png_x;x++)
        {
            R_Map[curr_tile].x=tileset_curr.x;
            R_Map[curr_tile].y=tileset_curr.y;
            R_Map[curr_tile].width=tilesize;
            R_Map[curr_tile].height=tilesize;
            tileset_curr.x+=tilesize;
            curr_tile++;
        }
        tileset_curr.y+=tilesize;
    }
}

//   _________                       _____                    ___ ___                      ._.
//  /   _____/____ ___  __ ____     /     \ _____  ______    /   |   \   ___________   ____| |
//  \_____  \\__  \\  \/ // __ \   /  \ /  \\__  \ \____ \  /    ~    \_/ __ \_  __ \_/ __ \ |
//  /        \/ __ \\   /\  ___/  /    Y    \/ __ \|  |_> > \    Y    /\  ___/|  | \/\  ___/\|
// /_______  (____  /\_/  \___  > \____|__  (____  /   __/   \___|_  /  \___  >__|    \___  >_
//         \/     \/          \/          \/     \/|__|            \/       \/            \/\/

int MapTools::SaveMap(Game_Data &OldeSettings)
{
    std::string map_header=maps_folder+OldeSettings.current_map;
    std::string map_file=maps_folder+OldeSettings.current_map;

    for(int i=0;i<4;i++)
    {
        map_header.pop_back();
    }
    map_header+=yow_head;
    uint64_t signature=199999999;
    
    std::fstream save_header{map_header,save_header.binary | save_header.out};
    if(!save_header.is_open())
    {
        printf("MAPTOOLS: map header save error.\n");
        return -1;
    }
    else
    {
        save_header.write(reinterpret_cast<char*>(&signature),rw_signature64);

        save_header.write(reinterpret_cast<char*>(&max_size_x),rw_value16);
        save_header.write(reinterpret_cast<char*>(&max_size_y),rw_value16);
            printf("MAPTOOLS: save size x%d y%d\n",max_size_x,max_size_y);
        save_header.write(reinterpret_cast<char*>(&spawn_point0_x),rw_value16);
        save_header.write(reinterpret_cast<char*>(&spawn_point0_y),rw_value16);
            printf("MAPTOOLS: save espawn 0 x%d y%d\n",spawn_point0_x,spawn_point0_y);
        save_header.write(reinterpret_cast<char*>(&spawn_point1_x),rw_value16);
        save_header.write(reinterpret_cast<char*>(&spawn_point1_y),rw_value16);
            printf("MAPTOOLS: save spawn 1 x%d y%d\n",spawn_point1_x,spawn_point1_y);
        save_header.write(reinterpret_cast<char*>(&spawn_point2_x),rw_value16);
        save_header.write(reinterpret_cast<char*>(&spawn_point2_y),rw_value16);
            printf("MAPTOOLS: save spawn 2 x%d y%d\n",spawn_point2_x,spawn_point3_y);
        save_header.write(reinterpret_cast<char*>(&spawn_point3_x),rw_value16);
        save_header.write(reinterpret_cast<char*>(&spawn_point3_y),rw_value16);
            printf("MAPTOOLS: save spawn 3 x%d y%d\n",spawn_point3_x,spawn_point3_y);
        save_header.close();
        printf("MAPTOOLS: header %s saved ok!\nMAPTOOLS: map array set to x%d y%d\n",map_header.c_str(),max_size_x,max_size_y);
    }

    std::fstream save_map{map_file,save_map.binary | save_map.out};
    if(!save_map.is_open())
    {
        printf("MAPTOOLS: map array save error.\n");
        return -1;
    }
    else
    {
        uint16_t t;
        uint64_t tilescount=0;
        save_map.write(reinterpret_cast<char*>(&signature),rw_signature64);
        for(int y=0;y<max_size_y;y++)
        {
            for(int x=0;x<max_size_x;x++)
            {
                t=map_arr[y*max_y+x];
                save_map.write(reinterpret_cast<char*>(&t),rw_value16);
                tilescount++;
            }
        }
        save_map.close();
        printf("MAPTOOLS: map %s saved ok!\nMAPTOOLS: written %ld tiles, %ld bytes\n",map_file.c_str(),tilescount,tilescount*2);
    }
    return 0;
}


// .____                     .___    _____                    ___ ___                      ._.
// |    |    _________     __| _/   /     \ _____  ______    /   |   \   ___________   ____| |
// |    |   /  _ \__  \   / __ |   /  \ /  \\__  \ \____ \  /    ~    \_/ __ \_  __ \_/ __ \ |
// |    |__(  <_> ) __ \_/ /_/ |  /    Y    \/ __ \|  |_> > \    Y    /\  ___/|  | \/\  ___/\|
// |_______ \____(____  /\____ |  \____|__  (____  /   __/   \___|_  /  \___  >__|    \___  >_
//         \/         \/      \/          \/     \/|__|            \/       \/            \/\/

int MapTools::LoadMap(std::string what_map)
{
    std::string map_file=maps_folder+what_map;
    std::string map_header=maps_folder+what_map;

    for(int i=0;i<4;i++)
    {
        map_header.pop_back();
    }

    map_header+=yow_head;

    uint64_t check_header_signature=0;
    uint64_t check_map_signature=0;
    Wipe();

    std::fstream load_header{map_header,load_header.binary | load_header.in};
    if(!load_header.is_open())
    {
        printf("MAPTOOLS: map header load error.\n");
        return -1;
    }
    else
    {
        load_header.read(reinterpret_cast<char*>(&check_header_signature),rw_signature64);
        // -======================================================================-
        load_header.read(reinterpret_cast<char*>(&max_size_x),rw_value16);
        load_header.read(reinterpret_cast<char*>(&max_size_y),rw_value16);
            printf("MAPTOOLS: load size x%d y%d\n",max_size_x,max_size_y);
        load_header.read(reinterpret_cast<char*>(&spawn_point0_x),rw_value16);
        load_header.read(reinterpret_cast<char*>(&spawn_point0_y),rw_value16);
            printf("MAPTOOLS: load spawn 0 x%d y%d\n",spawn_point0_x,spawn_point0_y);
        load_header.read(reinterpret_cast<char*>(&spawn_point1_x),rw_value16);
        load_header.read(reinterpret_cast<char*>(&spawn_point1_y),rw_value16);
            printf("MAPTOOLS: load spawn 1 x%d y%d\n",spawn_point1_x,spawn_point1_y);
        load_header.read(reinterpret_cast<char*>(&spawn_point2_x),rw_value16);
        load_header.read(reinterpret_cast<char*>(&spawn_point2_y),rw_value16);
            printf("MAPTOOLS: load spawn 2 x%d y%d\n",spawn_point2_x,spawn_point3_y);
        load_header.read(reinterpret_cast<char*>(&spawn_point3_x),rw_value16);
        load_header.read(reinterpret_cast<char*>(&spawn_point3_y),rw_value16);
            printf("MAPTOOLS: load spawn 3 x%d y%d\n",spawn_point3_x,spawn_point3_y);
        load_header.close();
        printf("MAPTOOLS: header %s read ok!\nMAPTOOLS: map array set to x%d y%d\n",map_header.c_str(),max_size_x,max_size_y);
    }
    std::fstream load_map{map_file,load_map.binary | load_map.in};
    if(!load_map.is_open())
    {
        printf("MAPTOOLS: map array load error.\n");
        return -1;
    }
    else
    {
        load_map.read(reinterpret_cast<char*>(&check_map_signature),rw_signature64);
        if(check_header_signature==check_map_signature)
        {
            printf("MAPTOOLS: header recognized map array... reading map array...\n");
        }
        else
        {
            printf("MAPTOOLS: map signature mismatch.\n");
            return -1;
        }

        int read_x=0;
        int read_y=0;
        int tilescount=read_x;
        uint16_t t_read=0;
        while(!load_map.eof())
        {
            load_map.read(reinterpret_cast<char*>(&t_read),rw_value16);
            map_arr[read_y*max_y+read_x]=t_read;
            // earlier was max_map_y
            read_x++;
            tilescount++;
            if(read_x>=max_size_x)
            {
                read_x=0;
                read_y++;
            }
        }
        load_map.close();
        printf("MAPTOOLS: map %s loaded ok!\nMAPTOOLS: loaded %d tiles,%d bytes.\n",map_file.c_str(),tilescount,tilescount*2);
        // loader loads EOF symbol aswell and it count +1 to tilescount - watch out!
    }
    return 0;
}

void MapTools::SetMapSpawnPoint(EditTools &OldeEdit,int spawn_num)
{
    OldeEdit.is_spawn_set=true;
    OldeEdit.which_spawn_set=spawn_num;
    switch(spawn_num)
    {
        case 0:
            spawn_point0_x=static_cast<uint16_t>(OldeEdit.cur_x);
            spawn_point0_y=static_cast<uint16_t>(OldeEdit.cur_y);
        break;
        case 1:
            spawn_point1_x=static_cast<uint16_t>(OldeEdit.cur_x);
            spawn_point1_y=static_cast<uint16_t>(OldeEdit.cur_y);
        break;
        case 2:
            spawn_point2_x=static_cast<uint16_t>(OldeEdit.cur_x);
            spawn_point2_y=static_cast<uint16_t>(OldeEdit.cur_y);
        break;
        case 3:
            spawn_point3_x=static_cast<uint16_t>(OldeEdit.cur_x);
            spawn_point3_y=static_cast<uint16_t>(OldeEdit.cur_y);
        break;
    }
}
void MapTools::JumpToSpawnPoint(Vector2 &edit_pointer_pos,int spawn_num)
{
    switch(spawn_num)
    {
        case 0:
            edit_pointer_pos.x=spawn_point0_x*tilesize;
            edit_pointer_pos.y=spawn_point0_y*tilesize;
        break;
        case 1:
            edit_pointer_pos.x=spawn_point1_x*tilesize;
            edit_pointer_pos.y=spawn_point1_y*tilesize;
        break;
        case 2:
            edit_pointer_pos.x=spawn_point2_x*tilesize;
            edit_pointer_pos.y=spawn_point2_y*tilesize;
        break;
        case 3:
            edit_pointer_pos.x=spawn_point3_x*tilesize;
            edit_pointer_pos.y=spawn_point3_y*tilesize;
        break;
    }
}

void MapTools::EditorDrawPalette(Game_Assets &OldeAssets,int pallete_sel,int palette_type)
{
    Vector2 pallete_pos
    {
        (float)(GetScreenWidth()/4)*3,
        (float)(tilesize)*4
    };
    Vector2 pallete_pos_circle
    {
        (float)(GetScreenWidth()/4)*3+8,
        (float)(tilesize)*4+8
    };
    int pallete_num=0;
    for(int gy=0;gy<tile_png_y;gy++)
    {
        for(int gx=0;gx<tile_png_x;gx++)
        {
            pallete_pos.x+=tilesize;
            pallete_pos_circle.x+=tilesize;
            // DrawRectangleRec((Rectangle){pallete_pos.x,pallete_pos.y,(float)tilesize,(float)tilesize},YELLOW);
            if(pallete_num==pallete_sel)
            {
                switch(palette_type)
                {
                    case 0: DrawTextureRec(OldeAssets.MapSprite,R_Map[pallete_num],pallete_pos,WHITE);
                            DrawCircleLinesV(pallete_pos_circle,8.0f,RED);
                    break;

                    case 1: // DrawTextureRec(OldeAssets.ObjectsSprite,R_Objects[pallete_num],pallete_pos,WHITE);
                            DrawCircleLinesV(pallete_pos_circle,8.0f,RED);
                    break;

                    case 2: // DrawTextureRec(OldeAssets.RoofsSprite,R_Roofs[pallete_num],pallete_pos,WHITE);
                            DrawCircleLinesV(pallete_pos_circle,8.0f,RED);
                    break;
                }
                
            }
            else
            {
                switch(palette_type)
                {
                    case 0: DrawTextureRec(OldeAssets.MapSprite,R_Map[pallete_num],pallete_pos,WHITE);
                    break;

                    case 1: // DrawTextureRec(OldeAssets.ObjectsSprite,R_Objects[pallete_num],pallete_pos,WHITE);
                    break;

                    case 2: // DrawTextureRec(OldeAssets.RoofsSprite,R_Roofs[pallete_num],pallete_pos,WHITE);
                    break;
                }
            }
            pallete_num++;
        }
        pallete_pos.y+=tilesize;
        pallete_pos_circle.y+=tilesize;
        pallete_pos.x=(float)(GetScreenWidth()/4)*3;
        pallete_pos_circle.x=(float)(GetScreenWidth()/4)*3+8;
    }
}

void MapTools::EditorPlaceTile(EditTools &OldeEdit,int pallete_sel,int palette_type)
{
    int x=OldeEdit.cur_x;
    int y=OldeEdit.cur_y;
    switch(palette_type)
    {
        case 0:
        map_arr[y*max_y+x]=pallete_sel;
        break;

        default:
        break;

        // in case if layers will be needed to implement
    }
}

uint16_t MapTools::EditorWhatTilesetOn(EditTools &OldeEdit,int palette_type)
{
    int x=OldeEdit.cur_x;
    int y=OldeEdit.cur_y;
    switch(palette_type)
    {
        // case 0: return map_arr[y*max_size_y+x]; // bug?
        case 0: return map_arr[y*max_y+x];

        default:
        break;
    }
    return 0;
}

uint16_t MapTools::GetTileInfoAt(uint16_t x, uint16_t y)
{
    // uint16_t tile=map_arr[y*max_size_y+x];
    // return tile;

    // WATCH OUT FOR CONVERSION FROM int16_t
    // host drawing map and securing it in packet uses int16_t values
    // to be able to seek below x 0 and y 0

    return (map_arr[y*max_y+x]);
}

void MapTools::PlaceTileInfoAt(uint16_t type,uint16_t x,uint16_t y)
{
    map_arr[y*max_y+x]=type;
}

void MapTools::PlaceTileByMemPos(uint16_t tile,uint16_t tile_mem)
{
    map_arr[tile_mem]=tile;
}

void MapTools::MemcpyTile(uint8_t *buffer,uint16_t num,uint16_t x,uint16_t y,const int frame_size)
{
    size_t offset=frame_size*2;
    int mem_map=y*max_y+x;

    // provide packet pointer pls
}

void MapTools::EditorClearLayer(int layer_num)
{
    switch(layer_num)
    {
        case 0:
        for(int y=0;y<max_y;y++)
        {
            for(int x=0;x<max_x;x++)
            {
                map_arr[y*max_y+x]=0;
            }
        } 
        break;

        default:
        break;
    }
}

void MapTools::EditorResizeMap(uint16_t x,uint16_t y)
{
    max_size_x=x;
    max_size_y=y;
}

uint16_t MapTools::EditorGetMaxX(){return max_size_x;}
uint16_t MapTools::EditorGetMaxY(){return max_size_y;}

void MapTools::GameSetMapSize(uint16_t setx,uint16_t sety)
{
    max_size_x=setx;
    max_size_y=sety;
}

void MapTools::FirstSet()
{
    max_size_x=32;
    max_size_y=32;
    spawn_point0_x=1;
    spawn_point0_y=1;

    spawn_point1_x=2;
    spawn_point1_y=1;

    spawn_point2_x=3;
    spawn_point2_y=1;

    spawn_point3_x=4;
    spawn_point3_y=1;
}

void MapTools::Wipe()
{
    max_size_x=0;
    max_size_y=0;
    spawn_point0_x=0;
    spawn_point0_y=0;

    spawn_point1_x=0;
    spawn_point1_y=0;

    spawn_point2_x=0;
    spawn_point2_y=0;

    spawn_point3_x=0;
    spawn_point3_y=0;
}

void MapTools::ZeroMap()
{
    for(int y=0;y<max_y;y++)
    {
        for(int x=0;x<max_x;x++)
        {
            map_arr[y*max_y+x]=0;
        }
    }
}