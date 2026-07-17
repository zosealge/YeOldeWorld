#define ENET_IMPLEMENTATION
#include"inc/enet.h"
#include"inc/raylib.h"
#include<iostream>
#include<cstring>
#include<csignal>
#include"inc/network.hpp"
#include"inc/networkhost.hpp"

bool host_shutdown=false;

void exit_signal(int sig)
{
    if(sig==SIGINT)
    {
        host_shutdown=true;
    }
}

int main()
{
    std::signal(SIGINT,exit_signal); // using std::signal because this host will block Your terminal
                                     // exit host peacefuly using Ctrl+C
                                     // and then host will disconnect all the clients, shutdown in proper way
    NetworkHost OldeHost;
    bool host_loop=OldeHost.NetworkInitialize();
    if(!host_loop) return -1;

    while(host_loop)
    {
        OldeHost.NetworkHostService();

        if(host_shutdown)
        {
            printf("\n SIGINT signal received\n");
            host_loop=false;
        }

    }
    OldeHost.NetworkDisconnectAllPeers();
    OldeHost.NetworkDeinitialize();
    return 0;
}