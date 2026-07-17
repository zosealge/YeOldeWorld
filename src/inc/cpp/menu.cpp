#include"../menu.hpp"

Game_Menu::Game_Menu(int what_menu)
{
    which_menu=what_menu;
    which_pos=0;
    cursor_pos=0;
    y_spanning=25.0f;
    font_size=20.0f;
    enter_was_pressed=false;
    public_enter_was_pressed=false;
    rc_enter=0;
    menu_pos_v.x=150;
    menu_pos_v.y=150;
    sprite_pos_v.x=90;
    sprite_pos_v.y=140;
    roofs_pos_v.x=(menu_pos_v.x)-menu_tilesize;
    roofs_pos_v.y=(menu_pos_v.y)-menu_tilesize;
    editor_help_v.x=400;
    editor_help_v.y=100;
    menu_pos_y_set=menu_pos_v.y;
    ip_name_char_num=0;
    res_x_char_num=0;
    res_y_char_num=0;
    memset(ip_addr,0,sizeof(ip_addr));

    switch(what_menu)
    {
        case 0: // MAIN MENU
            menu_names.push_back("Load Map");
            menu_names.push_back("Options");
            menu_names.push_back("Exit Editor");
        break;
        case 1: // EDITOR LOOP
            menu_names.push_back("Continue");
            menu_names.push_back("Save Map");
            menu_names.push_back("Debug Mode");
            menu_names.push_back("Resize Map");
            menu_names.push_back("Help");
            menu_names.push_back("Quit");
        break;
        case 2: // GAME CLIENT MAIN MENU
            menu_names.push_back("Connect");
            menu_names.push_back("Options");
            menu_names.push_back("Exit Game");
        break;
        case 3: // GAME CLIENT LOOP
            menu_names.push_back("Continue");
            menu_names.push_back("Debug Mode");
            menu_names.push_back("Help");
            menu_names.push_back("Disconnect");
        break;
    }
    options_list.push_back("Debug");
        option_attr[0]=1;
    options_list.push_back("Zoom");
        option_attr[1]=1;
    options_list.push_back("Resolution");
        option_attr[2]=3;
    options_list.push_back("Fullscreen");
        option_attr[3]=1;
    options_list.push_back("Exit");
    connection_list.push_back("Connect to:");
    // connection_list.push_back("Offline Mode");
    connection_list.push_back("Exit");
    resize_list.push_back("Map X:");
    resize_list.push_back("Map Y:");
    resize_list.push_back("Resize");
    resize_list.push_back("Cancel");
    public_display_main_menu=true;
    public_display_options=false;
    public_display_load_menu=false;
    public_display_connect_menu=false;
    public_display_options_menu=false;
    public_display_editor_resize=false;
    public_display_editor_show_help=false;
    public_save_map=false;

    LoadMapList();
    sprite_num=0;
    res_max_x=0;
    res_max_y=0;
}

Game_Menu::~Game_Menu()
{
    menu_names.clear();
    options_list.clear();
    map_list.clear();
    UnloadDirectoryFiles(maps_dir);
}

void Game_Menu::Control()
{
    rc_enter=0;
    enter_was_pressed=false;
    public_enter_was_pressed=false;
    if(IsKeyPressed(KEY_UP))
    {
        cursor_pos--;
        sprite_num=GetRandomValue(0,4);
        switch(sprite_num)
        {
            case 1: sprite_num=3; break;
            case 2: sprite_num=6; break;
            case 3: sprite_num=9; break;
            case 4: sprite_num=12; break;
            default: break;
        }

    }
    if(IsKeyPressed(KEY_DOWN))
    {
        cursor_pos++;
        sprite_num=GetRandomValue(0,4);
        switch(sprite_num)
        {
            case 1: sprite_num=3; break;
            case 2: sprite_num=6; break;
            case 3: sprite_num=9; break;
            case 4: sprite_num=12; break;
            default: break;
        }
    }
    if(IsKeyPressed(KEY_LEFT))
    {
        which_pos--;
        // sprite_num--;
        if(which_pos<0) which_pos=0;
    }
    if(IsKeyPressed(KEY_RIGHT))
    {
        which_pos++;
        // sprite_num++;
        if(which_pos>4) which_pos=4;
    }
    if(IsKeyPressed(KEY_ENTER))
    {
        enter_was_pressed=true;
        rc_enter=cursor_pos;
    }

    if(cursor_pos<0) cursor_pos=0;
}
void Game_Menu::DrawMenu(Game_Assets &OldeAssets,Game_Data &OldeSettings,float &d_time,bool debug_mode)
{
    if(public_display_main_menu)
    {
        Vector2 choice_pos_v{};

        how_many_entries=menu_names.size()-1;
        menu_pos_v.y=menu_pos_y_set;
        for(int y=0;y<=menu_names.size()-1;y++)
        {
            // DrawTextPro(Font font, const char *text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint);
            if(y==cursor_pos)
            {
                DrawSprite(OldeAssets,d_time);
                DrawTextEx(OldeAssets.GameFont,TextFormat("%s",menu_names[y].c_str()),menu_pos_v,font_size,1.0f,WHITE);
                if(which_menu==1 && y==1)
                {
                    choice_pos_v=menu_pos_v;
                    choice_pos_v.x+=200;
                    if(which_pos>1) which_pos=1;
                    switch(which_pos)
                    {
                        case 0:
                        DrawChoiceSprite(OldeAssets,d_time,choice_pos_v);
                        DrawTextEx(OldeAssets.GameFont,TextFormat("NO"),choice_pos_v,font_size,1.0f,WHITE);
                        choice_pos_v.x+=100;
                        DrawTextEx(OldeAssets.GameFont,TextFormat("YES"),choice_pos_v,font_size,1.0f,GREEN);
                        break;

                        case 1:
                        DrawTextEx(OldeAssets.GameFont,TextFormat("NO"),choice_pos_v,font_size,1.0f,GREEN);
                        choice_pos_v.x+=100;
                        DrawChoiceSprite(OldeAssets,d_time,choice_pos_v);
                        DrawTextEx(OldeAssets.GameFont,TextFormat("YES"),choice_pos_v,font_size,1.0f,WHITE);
                        break;
                    }
                }
                if(which_menu==1 && y==4) ShowHelp(OldeAssets);
            }
            else
            {
                DrawTextEx(OldeAssets.GameFont,TextFormat("%s",menu_names[y].c_str()),menu_pos_v,font_size,1.0f,GREEN);
            }
            menu_pos_v.y+=y_spanning;
        }

        if(cursor_pos>menu_names.size()-1) cursor_pos=menu_names.size()-1;
    }

    if(public_display_load_menu)
    {
        how_many_entries=map_list.size()-1;
        menu_pos_v.y=menu_pos_y_set;
        for(int y=0;y<=map_list.size()-1;y++)
        {
            // DrawTextPro(Font font, const char *text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint);
            if(y==cursor_pos)
            {
                DrawSprite(OldeAssets,d_time);
                DrawTextEx(OldeAssets.GameFont,TextFormat("%s",map_list[y].c_str()),menu_pos_v,font_size,1.0f,WHITE);
            }
            else
            {
                DrawTextEx(OldeAssets.GameFont,TextFormat("%s",map_list[y].c_str()),menu_pos_v,font_size,1.0f,GREEN);
            }
            menu_pos_v.y+=y_spanning;
        }
        if(cursor_pos>map_list.size()-1) cursor_pos=map_list.size()-1;
    }

    if(public_display_connect_menu)
    {
        Vector2 ip_pos_v
        {
            (menu_pos_v.x)+250,
            menu_pos_y_set
        };

        how_many_entries=connection_list.size()-1;
        menu_pos_v.y=menu_pos_y_set;
        for(int y=0;y<=connection_list.size()-1;y++)
        {
            if(y==cursor_pos)
            {
                DrawSprite(OldeAssets,d_time);
                DrawTextEx(OldeAssets.GameFont,TextFormat("%s",connection_list[y].c_str()),menu_pos_v,font_size,1.0f,WHITE);
            }
            else
            {
                DrawTextEx(OldeAssets.GameFont,TextFormat("%s",connection_list[y].c_str()),menu_pos_v,font_size,1.0f,GREEN);
            }
            if(y==0)
            {
                TypeIPAddr(OldeSettings);
            }
            if(d_time>0.0 && d_time<0.5) DrawTextEx(OldeAssets.GameFont,TextFormat("%s_",ip_addr),ip_pos_v,font_size,1.0f,WHITE);
            if(d_time>0.5 && d_time<1.0) DrawTextEx(OldeAssets.GameFont,TextFormat("%s",ip_addr),ip_pos_v,font_size,1.0f,WHITE);
            if(d_time>1.0 && d_time<1.5) DrawTextEx(OldeAssets.GameFont,TextFormat("%s_",ip_addr),ip_pos_v,font_size,1.0f,WHITE);
            if(d_time>1.5 && d_time<2.0) DrawTextEx(OldeAssets.GameFont,TextFormat("%s",ip_addr),ip_pos_v,font_size,1.0f,WHITE);
            if(d_time>2.0 && d_time<2.5) DrawTextEx(OldeAssets.GameFont,TextFormat("%s_",ip_addr),ip_pos_v,font_size,1.0f,WHITE);
            if(d_time>2.5)               DrawTextEx(OldeAssets.GameFont,TextFormat("%s",ip_addr),ip_pos_v,font_size,1.0f,WHITE);
            // DrawTextEx(OldeAssets.GameFont,TextFormat("%s",ip_addr),ip_pos_v,font_size,1.0f,WHITE);
            menu_pos_v.y+=y_spanning;
        }
        if(cursor_pos>connection_list.size()-1) cursor_pos=connection_list.size()-1;
    }
    
    if(public_display_options_menu)
    {
        how_many_entries=options_list.size()-1;
        menu_pos_v.y=menu_pos_y_set;
        Vector2 static_res
        {
            menu_pos_v.x,
            (menu_pos_v.y)-40
        };
        Vector2 attribute_pos_v
        {
            (menu_pos_v.x)+256,
            menu_pos_y_set
        };
        DrawTextEx(OldeAssets.GameFont,TextFormat("Current Resolution: %dx%d",GetScreenWidth(),GetScreenHeight()),static_res,font_size,1.0f,GREEN);
        for(int y=0;y<=options_list.size()-1;y++)
        {
            // DrawTextPro(Font font, const char *text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint);
            if(y==cursor_pos)
            {
                DrawSprite(OldeAssets,d_time);
                DrawTextEx(OldeAssets.GameFont,TextFormat("%s",options_list[y].c_str()),menu_pos_v,font_size,1.0f,WHITE);
                // DrawTextEx(OldeAssets.GameFont,TextFormat("%s",options_list[y].c_str()),menu_pos_v,font_size,1.0f,WHITE);
                if(y==2)
                {
                    attribute_pos_v.y=menu_pos_v.y;
                    DrawTextEx(OldeAssets.GameFont,TextFormat("%d",OldeSettings.resolution_selection),attribute_pos_v,font_size,1.0f,RED);
                }
                if(y==3)
                {
                    attribute_pos_v.y=menu_pos_v.y;
                    DrawTextEx(OldeAssets.GameFont,TextFormat("%d",OldeSettings.fullscreen_selection),attribute_pos_v,font_size,1.0f,RED);
                }
            }
            else
            {
                DrawTextEx(OldeAssets.GameFont,TextFormat("%s",options_list[y].c_str()),menu_pos_v,font_size,1.0f,GREEN);
            }
            menu_pos_v.y+=y_spanning;
        }
        if(cursor_pos>options_list.size()-1) cursor_pos=options_list.size()-1;
    }

    if(public_display_editor_resize)
    {
        Vector2 res_pos_v_x
        {
            (menu_pos_v.x)+150,
            menu_pos_y_set
        };
        Vector2 res_pos_v_y
        {
            (menu_pos_v.x)+150,
            menu_pos_y_set+(y_spanning)
        };

        how_many_entries=resize_list.size()-1;
        menu_pos_v.y=menu_pos_y_set;
        for(int y=0;y<=resize_list.size()-1;y++)
        {
            if(y==cursor_pos)
            {
                DrawSprite(OldeAssets,d_time);
                DrawTextEx(OldeAssets.GameFont,TextFormat("%s",resize_list[y].c_str()),menu_pos_v,font_size,1.0f,WHITE);
            }
            else
            {
                DrawTextEx(OldeAssets.GameFont,TextFormat("%s",resize_list[y].c_str()),menu_pos_v,font_size,1.0f,GREEN);
            }
            if(cursor_pos==0)
            {
                ChangeResX();
            }
            if(d_time>0.0 && d_time<0.5) DrawTextEx(OldeAssets.GameFont,TextFormat("%d_",res_max_x),res_pos_v_x,font_size,1.0f,WHITE);
            if(d_time>0.5 && d_time<1.0) DrawTextEx(OldeAssets.GameFont,TextFormat("%d",res_max_x),res_pos_v_x,font_size,1.0f,WHITE);
            if(d_time>1.0 && d_time<1.5) DrawTextEx(OldeAssets.GameFont,TextFormat("%d_",res_max_x),res_pos_v_x,font_size,1.0f,WHITE);
            if(d_time>1.5 && d_time<2.0) DrawTextEx(OldeAssets.GameFont,TextFormat("%d",res_max_x),res_pos_v_x,font_size,1.0f,WHITE);
            if(d_time>2.0 && d_time<2.5) DrawTextEx(OldeAssets.GameFont,TextFormat("%d_",res_max_x),res_pos_v_x,font_size,1.0f,WHITE);
            if(d_time>2.5)               DrawTextEx(OldeAssets.GameFont,TextFormat("%d",res_max_x),res_pos_v_x,font_size,1.0f,WHITE);
            if(cursor_pos==1)
            {
                ChangeResY();
            }
            if(d_time>0.0 && d_time<0.5) DrawTextEx(OldeAssets.GameFont,TextFormat("%d_",res_max_y),res_pos_v_y,font_size,1.0f,WHITE);
            if(d_time>0.5 && d_time<1.0) DrawTextEx(OldeAssets.GameFont,TextFormat("%d",res_max_y),res_pos_v_y,font_size,1.0f,WHITE);
            if(d_time>1.0 && d_time<1.5) DrawTextEx(OldeAssets.GameFont,TextFormat("%d_",res_max_y),res_pos_v_y,font_size,1.0f,WHITE);
            if(d_time>1.5 && d_time<2.0) DrawTextEx(OldeAssets.GameFont,TextFormat("%d",res_max_y),res_pos_v_y,font_size,1.0f,WHITE);
            if(d_time>2.0 && d_time<2.5) DrawTextEx(OldeAssets.GameFont,TextFormat("%d_",res_max_y),res_pos_v_y,font_size,1.0f,WHITE);
            if(d_time>2.5)               DrawTextEx(OldeAssets.GameFont,TextFormat("%d",res_max_y),res_pos_v_y,font_size,1.0f,WHITE);

            menu_pos_v.y+=y_spanning;
        }
        if(cursor_pos>resize_list.size()-1) cursor_pos=connection_list.size()-1;
    }
}

void Game_Menu::DrawSprite(Game_Assets &OldeAssets,float &d_time)
{
    sprite_pos_v=menu_pos_v;
    sprite_pos_v.x-=50;
    sprite_pos_v.y-=32;
    // 
    if(d_time>0.0 && d_time<0.5) DrawTextureRec(OldeAssets.MenuSprite,OldeAssets.MenuRectangle[sprite_num],sprite_pos_v,WHITE);
    if(d_time>0.5 && d_time<1.0) DrawTextureRec(OldeAssets.MenuSprite,OldeAssets.MenuRectangle[sprite_num+1],sprite_pos_v,WHITE);
    if(d_time>1.0 && d_time<1.5) DrawTextureRec(OldeAssets.MenuSprite,OldeAssets.MenuRectangle[sprite_num+2],sprite_pos_v,WHITE);
    if(d_time>1.5 && d_time<2.0) DrawTextureRec(OldeAssets.MenuSprite,OldeAssets.MenuRectangle[sprite_num],sprite_pos_v,WHITE);
    if(d_time>2.0 && d_time<2.5) DrawTextureRec(OldeAssets.MenuSprite,OldeAssets.MenuRectangle[sprite_num+1],sprite_pos_v,WHITE);
    if(d_time>2.5)               DrawTextureRec(OldeAssets.MenuSprite,OldeAssets.MenuRectangle[sprite_num+2],sprite_pos_v,WHITE);
}

void Game_Menu::DrawChoiceSprite(Game_Assets &OldeAssets,float &d_time,Vector2 &pos_v)
{
    if(d_time>0.0 && d_time<0.5) DrawTextureRec(OldeAssets.MenuSprite,OldeAssets.MenuRectangle[sprite_num],pos_v,WHITE);
    if(d_time>0.5 && d_time<1.0) DrawTextureRec(OldeAssets.MenuSprite,OldeAssets.MenuRectangle[sprite_num+1],pos_v,WHITE);
    if(d_time>1.0 && d_time<1.5) DrawTextureRec(OldeAssets.MenuSprite,OldeAssets.MenuRectangle[sprite_num+2],pos_v,WHITE);
    if(d_time>1.5 && d_time<2.0) DrawTextureRec(OldeAssets.MenuSprite,OldeAssets.MenuRectangle[sprite_num],pos_v,WHITE);
    if(d_time>2.0 && d_time<2.5) DrawTextureRec(OldeAssets.MenuSprite,OldeAssets.MenuRectangle[sprite_num+1],pos_v,WHITE);
    if(d_time>2.5)               DrawTextureRec(OldeAssets.MenuSprite,OldeAssets.MenuRectangle[sprite_num+2],pos_v,WHITE);
}


int Game_Menu::ProcessMenu()
{
    if(enter_was_pressed)
    {
        if(which_pos>0 && which_menu==1)
        {
            public_save_map=true;
            which_pos=0;
        }
        public_enter_was_pressed=true;
        // printf("how %d\n",rc_enter);
        // printf("man %d\n",how_many_entries);
        if(how_many_entries==rc_enter)
        {
            which_pos=0;
            return -1;
        }
        else
        {
            // printf("ret %d\n",rc_enter);
            which_pos=0;
            return rc_enter;
        }

    }
    public_enter_was_pressed=false;
    //            -1 is exit game
    return -2; // return noting important
}

void Game_Menu::SetResizeValues(uint16_t x,uint16_t y)
{
    res_max_x=x;
    res_max_y=x;
}

void Game_Menu::ShowLoading(Game_Assets &OldeAssets,Game_Data &OldeSettings,std::string wait_for)
{
    bool loading=true;
    float d_time=0.0f;
    while(loading)
    {
        d_time+=GetFrameTime();
        BeginDrawing();
            ClearBackground(BLACK);
            if(d_time>0.0 && d_time<0.3) DrawTextEx(OldeAssets.GameFont,TextFormat("%s ...",wait_for.c_str()),(Vector2){100,100},20,1.0f,RED);
            if(d_time>0.3 && d_time<0.6) DrawTextEx(OldeAssets.GameFont,TextFormat("%s ..",wait_for.c_str()),(Vector2){100,100},20,1.0f,RED);
            if(d_time>0.6 && d_time<1.0) DrawTextEx(OldeAssets.GameFont,TextFormat("%s .",wait_for.c_str()),(Vector2){100,100},20,1.0f,RED);
            DrawTextEx(OldeAssets.GameFont,TextFormat("map - %s",OldeSettings.current_map.c_str()),(Vector2){100,140},20,1.0f,DARKBROWN);
            if(OldeSettings.connection_to_host) DrawTextEx(OldeAssets.GameFont,TextFormat("IP - %s",OldeSettings.ip_address.c_str()),(Vector2){100,160},20,1.0f,DARKBROWN);

        EndDrawing();

        if(d_time>1)
        {
            d_time=0;
            loading=false;
        }
    }
}

void Game_Menu::ShowHostInfo(Game_Assets &OldeAssets,Game_Data &OldeSettings)
{
// 
// 
// 
// 
// 
// 
// ADD FOR ENET
}

void Game_Menu::TypeNewMapName(Game_Assets &OldeAssets,Game_Data &OldeSettings)
{
    std::string s_buf;
    s_buf.clear();
    const int max_type_chars=24;
    bool typing=true;
    bool enter_pressed=false;
    char map_name[max_type_chars]{0};
    int map_name_char_num=0;
    char buf;
    float d_time=0.0f;
    while(typing)
    {
        d_time+=GetFrameTime();

        if(IsKeyPressed(KEY_ENTER) && map_name_char_num>0)
        {
            enter_pressed=true;
            typing=false;
            s_buf=map_name;
            s_buf+=".map";
            OldeSettings.current_map=s_buf;
        }

        if(IsKeyPressed(KEY_BACKSPACE))
        {
            map_name[map_name_char_num-1]=0;
            map_name_char_num--;
            if(map_name_char_num<0) map_name_char_num=0;
        }

        buf=GetCharPressed();

        if(buf>41 && buf<172)
        {
            if(map_name_char_num<max_type_chars)
            {
                map_name[map_name_char_num]=buf;
                map_name_char_num++;
                if(map_name_char_num>16) map_name_char_num=16;
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);
            if(d_time>0.0 && d_time<0.5) DrawTextEx(OldeAssets.GameFont,TextFormat("> %s_map",map_name),(Vector2){100,100},20,1.0f,RED);
            if(d_time>0.5              ) DrawTextEx(OldeAssets.GameFont,TextFormat("> %s.map",map_name),(Vector2){100,100},20,1.0f,RED);
            DrawTextEx(OldeAssets.GameFont,"Please provide new map name and press enter",(Vector2){100,140},20,1.0f,RED);
        EndDrawing();

        if(d_time>1.0) d_time=0;
    }
}

void Game_Menu::ShowHelp(Game_Assets &OldeAssets)
{
    DrawTextureV(OldeAssets.EditorHelpPanel,editor_help_v,WHITE);
}

void Game_Menu::AcceptMap(Game_Data &OldeSettings,int sel)
{
    printf("OLDE: selected map is %d > %s\n",sel,map_list[sel].c_str());
    OldeSettings.current_map=map_list[sel];
}

int Game_Menu::IsExit()
{
    return map_list.size()-1;
}

void Game_Menu::LoadMapList()
{
    std::vector<std::string> load_list;
    maps_dir=LoadDirectoryFilesEx("maps",".map",false);
    map_list.push_back("New Map");
    for(int i=0;i<maps_dir.count;i++)
    {
        load_list.push_back(maps_dir.paths[i]);
        load_list[i].erase(0,5);
        map_list.push_back(load_list[i]);
    }
    map_list.push_back("Exit...");
}

void Game_Menu::TypeIPAddr(Game_Data &OldeSettings)
{
    char buf=GetCharPressed();
    if(buf>=46 && buf<=57)
    {
        ip_addr[ip_name_char_num]=buf;
        ip_name_char_num++;
        if(ip_name_char_num>16) ip_name_char_num=16;
    }
    if(IsKeyPressed(KEY_BACKSPACE))
    {
        if(ip_name_char_num>0) ip_addr[ip_name_char_num-1]=0;
        ip_name_char_num--;
        if(ip_name_char_num<0) ip_name_char_num=0;
    }

    if(IsKeyPressed(KEY_ENTER))
    {
        OldeSettings.ip_address=ip_addr;
    }
}

void Game_Menu::ChangeResX()
{
    if(IsKeyPressed(KEY_LEFT))
    {
        res_max_x--;
        if(res_max_x<min_map_x) res_max_x=min_map_x;
    }

    if(IsKeyPressed(KEY_RIGHT))
    {
        res_max_x++;
        if(res_max_x>max_map_x) res_max_x=max_map_x;
    }
}

void Game_Menu::ChangeResY()
{
    if(IsKeyPressed(KEY_LEFT))
    {
        res_max_y--;
        if(res_max_y<min_map_y) res_max_y=min_map_y;
    }

    if(IsKeyPressed(KEY_RIGHT))
    {
        res_max_y++;
        if(res_max_y>max_map_y) res_max_x=max_map_y;
    }
}

uint16_t Game_Menu::GetResizedMapX(){return res_max_x;}
uint16_t Game_Menu::GetResizedMapY(){return res_max_y;}