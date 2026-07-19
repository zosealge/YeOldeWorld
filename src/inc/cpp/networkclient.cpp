#include"../networkclient.hpp"

bool NetworkClient::NetworkInitialize()
{
    local_spawn_point_x=0;
    local_spawn_point_y=0;
    server_disconnect=false;
    if(enet_initialize()!=0)
    {
        printf("ENET: enet_initialize() failed!\n");
        return false;
    }
    client=enet_host_create(
        nullptr, // this should be nullptr for client
        max_clients,
        max_channels,
        max_in_bandwith,
        max_out_bandwith);
    
    if(client==nullptr)
    {
        printf("ENET: enet_host_create() failed!");
        return false;
    }

    address.port=network_port;
    // if everything initialize fine then
    return true;
}

void NetworkClient::NetworkDeinitialize()
{
    enet_deinitialize();
}


// 
// 
// 
// 
// 
// 
// 

bool NetworkClient::NetworkConnectToHost(std::string what_address)
{
    LastNow=0;
    LastInputSend=-100;
    InputUpdateInterval=1.0f/20.0f;
    enet_address_set_host(&address,what_address.c_str());
    server=enet_host_connect(client,&address,
        max_channels, // channelCount
        0); // optional data to send when connecting - uses enet_uint32
    if(server==nullptr)
    {
        printf("enet_host_connect() failed!\n");
        return false;
    }
    int ms_timeout=5000;
    ENetEvent event{};
    if(enet_host_service(client,&event,ms_timeout)>0 && event.type==ENET_EVENT_TYPE_CONNECT)
    {
        server_disconnect=false;
        return true;
        // connection succeeded!!!
        // after returning from that function make sure that
        // enet_host_service will be called frequently to not disconnect
        // client from host 
        // in this example enet_host_service() is called every frame start via NetworkHostService()
    }
    else
    {
        enet_peer_reset(server);
        return false;
    }
}

void NetworkClient::Update(double now,float deltaT,MapTools &OldeMap)
{
    LastNow=now;

    if(server==nullptr) return;

    if(LocalPlayerId>=0 && now-LastInputSend>InputUpdateInterval)
    {
        // PACKET CONSTRUCTION FROM CLIENT TO SERVER
        //
        // ###### id/dir/act/hand/bps0/bps1/bps2/posx - float/posy - float/posx - uint16_t/posy - uint16_t
        //       ####
        //           ####
        //               ##
        //                 ##
        uint8_t game_buffer[20]{};

        game_buffer[0]=(uint8_t)LocalPlayerId;
        game_buffer[1]=Clients[LocalPlayerId].direction;
        game_buffer[2]=Clients[LocalPlayerId].action;
        game_buffer[3]=Clients[LocalPlayerId].hands;
        game_buffer[4]=Clients[LocalPlayerId].backpack_slot0;
        game_buffer[5]=Clients[LocalPlayerId].backpack_slot1;
        game_buffer[6]=Clients[LocalPlayerId].backpack_slot2;
        memcpy(&game_buffer[7],&Clients[LocalPlayerId].Position.x,sizeof(float));
        memcpy(&game_buffer[11],&Clients[LocalPlayerId].Position.y,sizeof(float));
        memcpy(&game_buffer[15],&Clients[LocalPlayerId].posx,sizeof(uint16_t));
        memcpy(&game_buffer[17],&Clients[LocalPlayerId].posy,sizeof(uint16_t));

        ENetPacket *packet=enet_packet_create(game_buffer,sizeof(game_buffer),ENET_PACKET_FLAG_RELIABLE);
        
        enet_peer_send(server,channel_game,packet);

        LastInputSend=now;
    }

    ENetEvent event{};
    while(enet_host_service(client,&event,0)>0)
    {
        switch(event.type)
        {
            case ENET_EVENT_TYPE_RECEIVE:
            {
                if(event.packet->dataLength<1) break;

                        if(event.channelID==channel_game)
                {
                    // //event.packet->data[0] - is the sender number
                    // Clients[playerId].direction=event.packet->data[1];
                    // Clients[playerId].action=event.packet->data[2];
                    // Clients[playerId].hands=event.packet->data[3];
                    // Clients[playerId].backpack_slot0=event.packet->data[4];
                    // Clients[playerId].backpack_slot1=event.packet->data[5];
                    // Clients[playerId].backpack_slot2=event.packet->data[6];
                    // memcpy(&Clients[playerId].x,&event.packet->data[7],sizeof(float));
                    // memcpy(&Clients[playerId].y,&event.packet->data[11],sizeof(float));

                    int RemotePlayer=ReadWhoSendPacket(event.packet);
                    if(RemotePlayer!=LocalPlayerId)
                    {
                        Clients[RemotePlayer].Active=true;
                        Clients[RemotePlayer].direction=event.packet->data[1];
                        Clients[RemotePlayer].action=event.packet->data[2];
                        Clients[RemotePlayer].hands=event.packet->data[3];
                        Clients[RemotePlayer].backpack_slot0=event.packet->data[4];
                        Clients[RemotePlayer].backpack_slot1=event.packet->data[5];
                        Clients[RemotePlayer].backpack_slot2=event.packet->data[6];
                        memcpy(&Clients[RemotePlayer].Position.x,&event.packet->data[7],sizeof(float));
                        memcpy(&Clients[RemotePlayer].Position.y,&event.packet->data[11],sizeof(float));
                    }

                    if(RemotePlayer==LocalPlayerId)
                    {
                        if(event.packet->data[1]!=MapForPlayer) break;
                        else
                        {
                            // uint8_t fake_buffer[300]{};
                            // uint16_t fake_tile=20;
                            // size_t fake_t=sizeof(uint16_t);
                            // size_t offset_t=0;
                            // for(int x=0;x<140;x++)
                            // {
                            //     memcpy(&fake_buffer[offset_t],&fake_tile,fake_t);
                            //     offset_t+=fake_t;
                            // }
                            size_t sizetile=sizeof(uint16_t);
                            size_t packetsize=event.packet->dataLength;
                            int16_t start_x=Clients[LocalPlayerId].posx-(map_window_buffer/2);
                            int16_t start_y=Clients[LocalPlayerId].posy-(map_window_buffer/2);
                            int16_t end_x=(start_x)+map_window_buffer;
                            int16_t end_y=(start_y)+map_window_buffer;
                            uint16_t end_map_x=OldeMap.EditorGetMaxX();
                            uint16_t end_map_y=OldeMap.EditorGetMaxY();
                            size_t offset=sizeof(uint16_t); // will set on 2
                                                            // 0 - is sender ID
                                                            // 1 - is packet command
                            uint16_t tile_mem=0;
                            uint16_t tile_set=0;

                            int counter=0;

                            for(int16_t y=start_y;y<end_y;y++)
                            {
                                for(int16_t x=start_x;x<end_x;x++)
                                {
                                    if(x<0 || y<0 || x>end_map_x-1 || y>end_map_y-1)
                                    {
                                        // printf("opsie\n");
                                        // if(counter%12==0) printf("\n");
                                    }
                                    else
                                    {
                                        memcpy(&tile_set,&event.packet->data[offset],sizetile);
                                        // memcpy(&tile_set,&fake_buffer[offset],sizetile);
                                        // tile_set=10; for debug
                                        OldeMap.PlaceTileInfoAt(tile_set,x,y);
                                        // OldeMap.PlaceTileByMemPos(tile_set,tile_mem);
                                        // printf("%x ",tile_set);
                                        // if(counter%12==0) printf("\n");
                                    }
                                    offset+=sizetile;
                                    counter++;
                                    
                                }
                            }

                            //printf("packet from host is %zu\n",packetsize);
                            // printf("packet is %zu bytes\n",offset);
                        }
                        //plaace tile where player stands
                    }

                }
                else if(event.channelID==channel_comm)
                {
                    // NullCommand=0,
                    // AssignClientNumber=1;
                    // NewPlayer=2,
                    // RemovePlayer=3


                    int command=ReadCommand(event.packet);
                    if(LocalPlayerId==-1)
                    {
                        if(command==AssignClientNumber)
                        {
                            LocalPlayerId=ReadPlayerId(event.packet);
                            printf("You got number %d\n",LocalPlayerId);
                            Clients[LocalPlayerId].Active=true;
                            memcpy(&local_spawn_point_x,&event.packet->data[2],sizeof(uint16_t));
                            memcpy(&local_spawn_point_y,&event.packet->data[4],sizeof(uint16_t));
                            uint16_t mapx=0;
                            uint16_t mapy=0;
                            memcpy(&mapx,&event.packet->data[6],sizeof(uint16_t));
                            memcpy(&mapy,&event.packet->data[8],sizeof(uint16_t));

                            OldeMap.GameSetMapSize(mapx,mapy);

                            // this will be automated via buffer functions
                            // counting packets instead handcoding pointer positions
                        }
                    }
                    if(command==NewPlayer)
                    {
                        int ConnectingPlayer=ReadPlayerId(event.packet);
                        printf("there is a new kid: %d\n",ConnectingPlayer);
                        Clients[ConnectingPlayer].Active=true;
                    }
                    if(command==RemovePlayer)
                    {
                        int DisconnectingPlayer=ReadPlayerId(event.packet);
                        printf("this says bye bye: %d\n",DisconnectingPlayer);
                        Clients[DisconnectingPlayer].Active=false;
                        ZeroRemoteClient(DisconnectingPlayer);
                    }
                }
                else if(event.channelID==channel_msgs)
                {
                    // no messages yet implemented!
                }

                enet_packet_destroy(event.packet);
                break;
            }

            case ENET_EVENT_TYPE_DISCONNECT:
                server=nullptr;
                server_disconnect=true;
                LocalPlayerId=-1;
            break;
        }
    }
}

void NetworkClient::Disconnect()
{
    bool disconnected=false;
    ENetEvent event{};
    enet_peer_disconnect(server,0);
    while(enet_host_service(client,&event,1000)>0)
    {
        switch(event.type)
        {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy(event.packet);
            break;

            case ENET_EVENT_TYPE_DISCONNECT:
                disconnected=true;
            break;
        }
    }
    if(!disconnected)
    {
        enet_peer_reset(server);
    }
}

int NetworkClient::GetLocalPlayerId(){return LocalPlayerId;}
bool NetworkClient::Connected(){return server!=nullptr && LocalPlayerId>=0;}

void NetworkClient::PrepareLocalClient(const Vector2 &pos,const uint8_t dir,const uint8_t act,uint16_t posx,uint16_t posy)
{
    Clients[LocalPlayerId].Position.x=pos.x;
    Clients[LocalPlayerId].Position.y=pos.y;
    Clients[LocalPlayerId].direction=dir;
    Clients[LocalPlayerId].action=act;
    Clients[LocalPlayerId].hands=0;
    Clients[LocalPlayerId].backpack_slot0=0;
    Clients[LocalPlayerId].backpack_slot1=0;
    Clients[LocalPlayerId].backpack_slot2=0;
    Clients[LocalPlayerId].posx=posx;
    Clients[LocalPlayerId].posy=posy;
}
    
    
    
    
void NetworkClient::UpdateRemoteClient(ENetPacket *packet)
    {
        int remotePlayer=ReadPlayerId(packet);
        if(remotePlayer >= max_clients || remotePlayer==LocalPlayerId || !Clients[remotePlayer].Active)
            return;

        // size_t buffer_size=sizeof(packet->dataLength);
        // if(buffer_size<1) return;

        // uint8_t buffer[buffer_size]{};
        // buffer=packet->data;

        Clients[remotePlayer].direction=packet->data[1];
        Clients[remotePlayer].action   =packet->data[2];
        Clients[remotePlayer].hands    =packet->data[3];
        memcpy(&Clients[remotePlayer].Position.x,&packet->data[7],sizeof(float));
        memcpy(&Clients[remotePlayer].Position.y,&packet->data[11],sizeof(float));
    }

int NetworkClient::ReadCommand(ENetPacket *packet)
{
    uint8_t data=packet->data[0];
    return static_cast<int>(data);
}

int NetworkClient::ReadPlayerId(ENetPacket *packet)
{
    uint8_t data=packet->data[1];
    return (int)data; // what cast is better?
}

int NetworkClient::ReadWhoSendPacket(ENetPacket *packet)
{
    uint8_t data=packet->data[0];
    return (int)data;
}

void NetworkClient::ZeroRemoteClient(int num)
{
    Clients[num].Position.x=0;
    Clients[num].Position.y=0;
    Clients[num].ExtrapolatedPosition.x=0;
    Clients[num].ExtrapolatedPosition.y=0;
    Clients[num].direction=0;
    Clients[num].action=0;
    Clients[num].hands=0;
    Clients[num].backpack_slot0=0;
    Clients[num].backpack_slot1=0;
    Clients[num].backpack_slot2=0;
}