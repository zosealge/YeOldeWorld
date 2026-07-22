#include"../data.hpp"

Game_Data::Game_Data()
{
    cfg_file_loaded=false;
    std::ifstream cfg_file("client.cfg");
    std::string line;

    if(cfg_file.is_open())
    {
        cfg_file_loaded=true;
        printf("OLDE: client.cfg - found!\n");
        while(std::getline(cfg_file,line))
        {
            if(line.compare(0,2,"ip")==0)
            {
                size_t start=line.find('=')+1;
                size_t length=GetLengthOfEntry(line,start);
                //  (size_type pos = 0, size_type len = npos)
                ip_address=line.substr(start,length);
            }

            if(line.compare(0,10,"fullscreen")==0)
            {
                size_t start=line.find('=')+1;
                size_t length=GetLengthOfEntry(line,start);
                std::string grab=line.substr(start,length);
                // printf("fullscreen option is: %s\n",grab.c_str());
            }

            if(line.compare(0,3,"res")==0)
            {
                size_t start=line.find('=')+1;
                size_t length=GetLengthOfEntry(line,start);
                std::string grab=line.substr(start,length);
                // printf("res option is: %s\n",grab.c_str());
            }

            if(line.compare(0,3,"fps")==0)
            {
                size_t start=line.find('=')+1;
                size_t length=GetLengthOfEntry(line,start);
                std::string grab=line.substr(start,length);
                // printf("fps option is: %s\n",grab.c_str());
            }
        }
    }
    else
    {
        cfg_file_loaded=false;
        printf("OLDE: client.cfg - not found! - 0001\n");
        // cfg_file_loaded==false will cause game exit
    }
    editor_max_map_x=0;
    editor_max_map_y=0;
    resolution_selection=0;
    fullscreen_selection=0;
    local_player_num=-1;
    main_screen_width=1280;
    main_screen_height=720;
    set_max_fps=60;
    editor_new_map=false;
    connection_to_host=false;
    SetRender720p();
}

void Game_Data::ChangeResolution()
{
    resolution_selection++;
    if(resolution_selection>1) resolution_selection=0;

    switch(resolution_selection)
    {
        case 0:
        main_screen_width=1280;
        main_screen_height=720;
        break;

        case 1:
        main_screen_width=1920;
        main_screen_height=1080;
        break;
    }
    SetWindowSize(main_screen_width,main_screen_height);
}

void Game_Data::SetScreenMode()
{
    fullscreen_selection++;
    if(fullscreen_selection>1) fullscreen_selection=0;

    ToggleFullscreen();

    switch(fullscreen_selection)
    {
        case 0:
        SetRender720p();
        break;

        case 1:
        SetRender1080p();
        break;
    }
}

void Game_Data::SetRender720p()
{
    xrender=11;
    yrender=7;
    zrender=15;
    wrender=9;
}

void Game_Data::SetRender1080p()
{
    xrender=16;
    yrender=10;
    zrender=17;
    wrender=11;
}

bool Game_Data::IsAddrIPNotEmpty()
{
    if(!ip_address.empty()) return true;
    else return false;
};

std::string Game_Data::GetAddrIP()
{
    return ip_address;
}

bool Game_Data::IsConfigValid()
{
    return cfg_file_loaded;
}

size_t Game_Data::GetLengthOfEntry(std::string &str,size_t start)
{
    // size_t start=line.find('=')+1;
    size_t length=0;
    for(size_t i=0;;i++)
    {
        if(str[i]==';')
        {
            length=i;
            break;
        }
    }
    length-=start;
    return length;
}