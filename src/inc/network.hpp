#pragma once
#include"enet.h"

const int max_clients=4;
const int max_channels=3;
// ENet channels
const enet_uint8 channel_game=0;
const enet_uint8 channel_comm=1;
const enet_uint8 channel_msgs=2;
const enet_uint8 channel_actn=3;
const int max_in_bandwith=0; // no limits
const int max_out_bandwith=0; // no limits
const int network_port=7777;
const int network_message_size=64;
const enet_uint32 network_host_timeout=5;


struct NetworkClientInfo
{
    bool Active; // is player slot active
    bool ValidPosition; 
    ENetPeer *Peer;
    uint8_t direction;          // 0
    uint8_t action;             // 1
    uint8_t hands;              // 2
    uint8_t backpack_slot0;     // 3
    uint8_t backpack_slot1;     // 4
    uint8_t backpack_slot2;     // 5
    float x;                    // 6
    float y;                    // 10
    float dx;
    float dy;
};

struct NetworkRemoteClientInfo
{
    bool Active;
    Vector2 Position;
    Vector2 ExtrapolatedPosition;
    double UpdateTime;
    uint8_t direction;
    uint8_t action;
    uint8_t hands;
    uint8_t backpack_slot0;
    uint8_t backpack_slot1;
    uint8_t backpack_slot2;
};

enum NetworkCommands:uint8_t
{
    NullCommand=0,
    AssignClientNumber=1,
    NewPlayer=2,
    RemovePlayer=3
};
