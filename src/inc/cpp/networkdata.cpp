#include"../networkdata.hpp"

Network_Data::Network_Data()
{
    cfg_file_loaded=false;
    std::ifstream cfg_file("server.cfg");
    std::string line;
    
    if(cfg_file.is_open())
    {
        cfg_file_loaded=true;
        printf("server.cfg - found!\n");
        while(std::getline(cfg_file,line))
        {
            if(line.compare(0,4,"port")==0)
            {
                size_t start=line.find('=')+1;
                size_t end=line.find(';')-4;
                host_port=line.substr(start,end);
                printf("Port is set to %s\n",host_port.c_str());
            }

            if(line.compare(0,3,"map")==0)
            {
                size_t start=line.find('=')+1;
                size_t end=line.find(';')-4;
                map=line.substr(start,end);
                printf("Map is set to %s\n",map.c_str());
            }
        }
    }
    else
    {
        printf("server.cfg - not found! exiting...\n");
    }
}