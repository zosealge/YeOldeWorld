#pragma once
#include<iostream>
#include<fstream>
#include<string>

class Network_Data
{
    public:
    bool cfg_file_loaded;
    int port;
    std::string map;
    std::string host_port;

    Network_Data();
    bool IsConfigValid();

    private:
    size_t GetLengthOfEntry(std::string &str,size_t start);
};