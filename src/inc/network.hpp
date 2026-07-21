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
const int map_window_buffer=12;
const size_t map_t=sizeof(uint16_t);
const size_t map_offset_start_t=6;
// MUST BE A VALUE THAT CAN BE DIVIDED BY 2
// it means it is a x=12 and y=12 tiles of map window sent to player in packet
// map is now configured as uint16_t and its a 2 byte integer
// so 12 x 2 = 24 bytes per every x
// multipy by 12 as every y 
// packet for event.peer need to be 288 - constructed as uint8_t game_buffer[288] - maybe plus some header info
const enet_uint32 network_host_timeout=5;


struct ServerInfo
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
    uint8_t avatar;
    float x;                    // 6
    float y;                    // 10
    uint16_t posx;
    uint16_t posy;
    uint16_t act_posx;
    uint16_t act_posy;
    float dx;
    float dy;
};

struct ClientInfo
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
    uint8_t avatar;
    uint16_t posx;
    uint16_t posy;
    uint16_t act_posx;
    uint16_t act_posy;

};

struct MapFrame
{
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t z;
};

enum NetworkCommands:uint8_t
{
    NullCommand=0,
    AssignClientNumber=1,
    NewPlayer=2,
    RemovePlayer=3,
    MapForPlayer=4
};
