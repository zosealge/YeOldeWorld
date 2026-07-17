#pragma once
#include"raylib.h"
#include"enet.h"
#include"network.hpp"
#include<string>
#include<iostream>

class NetworkHost
{
    public:

    ENetHost    *server;
    ENetAddress address{};

    NetworkClientInfo Clients[max_clients]{};
    int who_is_talking_now=0;

    bool NetworkInitialize();
    void NetworkDeinitialize();
    void NetworkHostService();
    void NetworkDisconnectAllPeers();

    private:

    uint8_t GetPlayerId(ENetPeer *peer);
    void SendToOtherClients(ENetPacket *packet,uint8_t exceptPlayerId,enet_uint8 what_channel);
};

// New connection using slot: 1/3 -- this is NOT
// sent to 1 client
// who created this packet: 1
// Client 1/3 disconnecting -- this is OK
// sent to 0 client
// who created this packet: 1
// ^C
