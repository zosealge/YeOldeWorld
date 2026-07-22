#include"../networkdata.hpp"

Network_Data::Network_Data()
{
    cfg_file_loaded=false;
    std::ifstream cfg_file("server.cfg");
    std::string line;
    
    if(cfg_file.is_open())
    {
        cfg_file_loaded=true;
        printf("HOST: server.cfg - found!\n");
        while(std::getline(cfg_file,line))
        {
            if(line.compare(0,4,"port")==0)
            {
                size_t start=line.find('=')+1;
                size_t length=GetLengthOfEntry(line,start);
                host_port=line.substr(start,length);
                //printf("Port is set to %s\n",host_port.c_str());
            }

            if(line.compare(0,3,"map")==0)
            {
                size_t start=line.find('=')+1;
                size_t length=GetLengthOfEntry(line,start);
                map=line.substr(start,length);
                printf("HOST: Map is set to %s\n",map.c_str());
            }
        }
    }
    else
    {
        printf("HOST: server.cfg - not found! exiting...\n");
        cfg_file_loaded=false;
    }
}

bool Network_Data::IsConfigValid()
{
    return cfg_file_loaded;
}

size_t Network_Data::GetLengthOfEntry(std::string &str,size_t start)
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