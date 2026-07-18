#include"../networkhost.hpp"

bool NetworkHost::NetworkInitialize()
{
    if(enet_initialize()!=0)
    {
        printf("enet_initialize() failed, exiting...\n");
        return false;
    }
    address.host=ENET_HOST_ANY;
    address.port=network_port;

    server=enet_host_create(&address,
                            max_clients,
                            max_channels,
                            max_in_bandwith,
                            max_out_bandwith);

    if(server==nullptr)
    {
        printf("enet_host_create() failed, exiting...\n");
        return false;
    }

    //if everyting init fine then
    printf("\nYeOldeWorlde server started\n");
    for(int i=0;i<max_clients;i++)
    {
        Clients[i].Peer=nullptr;
        printf("Client slot %d/%d is ready\n",i,(max_clients)-1);
    }
    printf("Listening on port: %d\n\n",network_port);
    return true;
};

void NetworkHost::NetworkDeinitialize()
{
    enet_host_destroy(server);
    enet_deinitialize();
    printf("YeOldeWorlde host closed properly!\n");
};

//
//
//
//
//
//
//

void NetworkHost::NetworkHostService(MapTools &OldeMap)
{
    ENetEvent event{};
    while(enet_host_service(server,&event,network_host_timeout)>0)
    {
        switch(event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
            {
                char addr_buff[64]{};
                enet_address_get_host(&event.peer->address,addr_buff,sizeof(addr_buff));
                printf("Incoming connection from %s\n",addr_buff);
                uint8_t playerId=0;
                for(;playerId<max_clients;playerId++)
                {
                    if(!Clients[playerId].Active) break;
                }

                if(playerId==max_clients)
                {
                    enet_peer_disconnect(event.peer,0);
                    break;
                }

                //else
                {
                    printf("New connection using slot: %d/%d\n",playerId,(max_clients)-1);
                    who_is_talking_now=playerId;
                    Clients[playerId].Active=true;
                    Clients[playerId].ValidPosition=false;
                    Clients[playerId].Peer=event.peer;

                    int spawn_point=GetRandomValue(0,3);
                    uint16_t givex=0;
                    uint16_t givey=0;
                    switch(spawn_point)
                    {
                        case 0:
                        givex=OldeMap.spawn_point0_x;
                        givey=OldeMap.spawn_point0_y;
                        break;

                        case 1:
                        givex=OldeMap.spawn_point1_x;
                        givey=OldeMap.spawn_point1_y;
                        break;

                        case 2:
                        givex=OldeMap.spawn_point2_x;
                        givey=OldeMap.spawn_point2_y;
                        break;

                        case 3:
                        givex=OldeMap.spawn_point3_x;
                        givey=OldeMap.spawn_point3_y;
                        break;
                    }

                    printf("Client %d spawned at: x%d y%d\n",playerId,givex,givey);

                    uint8_t buffer[6]{};
                    buffer[0]=AssignClientNumber; // send this info to connecting peer
                    buffer[1]=(uint8_t)playerId; // with number
                    memcpy(&buffer[2],&givex,sizeof(uint16_t));
                    memcpy(&buffer[4],&givey,sizeof(uint16_t));

                    uint8_t buffer_for_actives[2]{};
                    buffer_for_actives[0]=NewPlayer; //send this info to already connected clients
                    buffer_for_actives[1]=(uint8_t)playerId; // with number who is connecting

                    ENetPacket *packet=enet_packet_create(buffer,sizeof(buffer),ENET_PACKET_FLAG_RELIABLE);
                    enet_peer_send(event.peer,channel_comm,packet);

                    ENetPacket *packet_to_others=enet_packet_create(buffer_for_actives,sizeof(buffer_for_actives),ENET_PACKET_FLAG_RELIABLE);

                    for(int i=0;i<max_clients;i++)
                    {
                        if(i==playerId || Clients[i].Active==false) continue;
                        enet_peer_send(Clients[i].Peer,channel_comm,packet_to_others);
                    }
                    break;
                }
            }

            case ENET_EVENT_TYPE_RECEIVE:
            {
                uint8_t playerId=GetPlayerId(event.peer);
                if(playerId==-1)
                {
                    enet_peer_disconnect(event.peer,0);
                    break;
                }

                // channel_game=0;
                // channel_comm=1;
                // channel_msgs=2;
                // channel_actn=3;
                if(event.channelID==channel_game)
                {
                    
                    Clients[playerId].ValidPosition=true;
                    //event.packet->data[0] - is the sender number
                    Clients[playerId].direction=event.packet->data[1];
                    Clients[playerId].action=event.packet->data[2];
                    Clients[playerId].hands=event.packet->data[3];
                    Clients[playerId].backpack_slot0=event.packet->data[4];
                    Clients[playerId].backpack_slot1=event.packet->data[5];
                    Clients[playerId].backpack_slot2=event.packet->data[6];
                    memcpy(&Clients[playerId].x,&event.packet->data[7],sizeof(float));
                    memcpy(&Clients[playerId].y,&event.packet->data[11],sizeof(float));

                    ENetPacket *new_packet=enet_packet_create(event.packet->data,event.packet->dataLength,ENET_PACKET_FLAG_RELIABLE);
                    // and send this packet to everybody but not the playerId

                    for(int i=0;i<max_clients;i++)
                    {
                        if(i==playerId || Clients[i].Active==false) continue;
                        else
                        {
                            enet_peer_send(Clients[i].Peer,channel_game,new_packet);
                        }
                    }

                    // game_buffer[0]=Clients[LocalPlayerId].direction;
                    // game_buffer[1]=Clients[LocalPlayerId].action;
                    // game_buffer[2]=Clients[LocalPlayerId].hands;
                    // game_buffer[3]=Clients[LocalPlayerId].backpack_slot0;
                    // game_buffer[4]=Clients[LocalPlayerId].backpack_slot1;
                    // game_buffer[5]=Clients[LocalPlayerId].backpack_slot2;
                    // memcpy(&game_buffer[6],&Clients[LocalPlayerId].Position.x,sizeof(float));
                    // memcpy(&game_buffer[10],&Clients[LocalPlayerId].Position.y,sizeof(float));

                }
                if(event.channelID==channel_comm)
                {
                    printf("channel_comm in use!\n");
                    // here will be communication signals
                    // open door
                    // hitting another player with something
                    // changing something on map
                }
                if(event.channelID==channel_msgs)
                {
                    printf("channel_msgs in use!\n");
                    // here will be chat messages
                    // and commands
                }

                enet_packet_destroy(event.packet);
                break;
            }

            case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
            case ENET_EVENT_TYPE_DISCONNECT:
            {
                uint8_t playerId=GetPlayerId(event.peer);
                if(playerId==-1) break;

                printf("Client %d/%d disconnecting\n",playerId,(max_clients)-1);

                Clients[playerId].Active=false;
                Clients[playerId].Peer=nullptr;

                uint8_t buffer[2]{};
                buffer[0]=RemovePlayer;
                buffer[1]=playerId;

                ENetPacket *packet=enet_packet_create(buffer,sizeof(buffer),ENET_PACKET_FLAG_RELIABLE);
                // SendToOtherClients(packet,-1,channel_comm);
                enet_host_broadcast(server,channel_comm,packet);
                break;
            }

            case ENET_EVENT_TYPE_NONE:
            break;
        }
    }
};


void NetworkHost::NetworkDisconnectAllPeers()
{
    for(int i=0;i<max_clients;i++)
    {
        if(Clients[i].Peer==nullptr)
        {
            printf("Client slot %d/%d empty\n",i,(max_clients)-1);
        }
        else
        {
            enet_peer_disconnect(Clients[i].Peer,0);
            printf("Client slot %d/%d disconnecting...\n",i,(max_clients)-1);
        }
    }
    enet_host_flush(server);
};

uint8_t NetworkHost::GetPlayerId(ENetPeer *peer)
{
    for(int i=0;i<max_clients;i++)
    {
        if(Clients[i].Active && Clients[i].Peer==peer) return (uint8_t)i;
    }
    return -1;
};

void NetworkHost::SendToOtherClients(ENetPacket *packet,uint8_t exceptPlayerId,enet_uint8 what_channel)
{
    for(int i=0;i<max_clients;i++)
    {

        if(!Clients[i].Active || i==exceptPlayerId) continue;
        printf("sent to %d client\n",i);
        printf("who created this packet: %d\n",who_is_talking_now);
        size_t bufsiz=packet->dataLength;
        enet_peer_send(Clients[i].Peer,what_channel,packet);
    }
}