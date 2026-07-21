#pragma once
#include"raylib.h"
#include"enet.h"
#include"network.hpp"
#include"maptools.hpp"
#include<string>
#include<iostream>

class NetworkClient
{
    public:

    bool server_disconnect;

    ENetHost    *client{};
    ENetAddress address{};
    ENetPeer    *server{};

    ClientInfo Clients[max_clients]{};

    uint16_t local_spawn_point_x;
    uint16_t local_spawn_point_y;

    double LastNow;
    double LastInputSend;
    double InputUpdateInterval;

    int LocalPlayerId=-1;

    bool NetworkInitialize();
    void NetworkDeinitialize();

    // 
    // 

    bool NetworkConnectToHost(std::string what_address);
    void Update(double now,float deltaT,MapTools &OldeMap);
    void Disconnect();
    int GetLocalPlayerId();
    bool Connected();
    void PrepareLocalClient(const Vector2 &pos,const uint8_t dir,const uint8_t act,uint16_t posx,uint16_t posy);

    private:

    void UpdateRemoteClient(ENetPacket *packet);
    int ReadCommand(ENetPacket *packet);
    int ReadPlayerId(ENetPacket *packet);
    int ReadWhoSendPacket(ENetPacket *packet);
    void ZeroRemoteClient(int num);
};