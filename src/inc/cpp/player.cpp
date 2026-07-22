#include"../player.hpp"

Game_Player::Game_Player()
{
    local_use=false;
    local_player_pos.x=0;
    local_player_pos.y=0;
    current_draw_offset=0;
    local_collision.x=0;
    local_collision.y=0;
    local_collision.width=12;
    local_collision.height=12;
    for(int i=0;i<(pngarraywidth*pngarrayheight);i++)
    {
        tiles[i].x=0;
        tiles[i].y=0;
        tiles[i].width=tilesize;
        tiles[i].height=tilesize;
    }

    int tilenum=0;
    for(int y=0;y<pngarrayheight;y++)
    {
        for(int x=0;x<pngarraywidth;x++)
        {
            tiles[tilenum].x=x*tilesize;
            tiles[tilenum].y=y*tilesize;
            tilenum++;
        }
    }
}

void Game_Player::MoveLocalPlayer(Vector2 &pos)
{
    local_player_pos=pos;
    local_collision.x=pos.x+2;
    local_collision.y=pos.y+2;
    cur_x=(int)(pos.x/tilesize);
    cur_y=(int)(pos.y/tilesize);
}

void Game_Player::DrawLocalPlayer(Game_Assets &OldeAssets,float &d_time,bool debug_mode)
{
    if(debug_mode)
    {
        DrawRectanglePro(local_collision,(Vector2){0,0},0.0f,RED);
    }

    if(d_time>0.0 && d_time<0.5) current_draw_offset=0;
    if(d_time>0.5 && d_time<1.0) current_draw_offset=1;
    if(d_time>1.0 && d_time<1.5) current_draw_offset=0;
    if(d_time>1.5 && d_time<2.0) current_draw_offset=1;
    if(d_time>2.0 && d_time<2.5) current_draw_offset=0;
    if(d_time>2.5 && d_time<3.0) current_draw_offset=1;

    if(pl_dir[player_num]=='q' || pl_dir[player_num]=='a' || pl_dir[player_num]=='z')
    {
        pl_side[player_num]=0;
    }
    if(pl_dir[player_num]=='e' || pl_dir[player_num]=='d' || pl_dir[player_num]=='c')
    {
        pl_side[player_num]=4;
    }
    int avatar_offset=0;
    if(pl_avatar[player_num]==0) avatar_offset=0;
    else if(pl_avatar[player_num]==1) avatar_offset=40;
    else if(pl_avatar[player_num]==2) avatar_offset=80;
    else if(pl_avatar[player_num]==3) avatar_offset=120;
    else avatar_offset=0;

    int offset=0;

    if(pl_act[player_num]=='w')
    {
        offset=avatar_offset+pl_side[player_num]+10;
        DrawTextureRec(OldeAssets.AvatarsSprite,tiles[offset+current_draw_offset],local_player_pos,WHITE);
    }
    else
    {
        offset=avatar_offset+pl_side[player_num];
        DrawTextureRec(OldeAssets.AvatarsSprite,tiles[offset],local_player_pos,WHITE);
    }

    // avatars only can look left or right
    // so keys "w" and "s" should not modify direction of rendering

    // int offset=0;

    // if(pl_act[player_num]=='w')
    // {
    //     offset=pl_avatar[player_num]+pl_side[player_num]+1;
    //     DrawTextureRec(OldeAssets.AvatarsSprite,tiles[offset+current_draw_offset],local_player_pos,WHITE);
    // }
    // else
    // {
    //     offset=pl_avatar[player_num]+pl_side[player_num];
    //     DrawTextureRec(OldeAssets.AvatarsSprite,tiles[offset],local_player_pos,WHITE);
    // }

    // might move this animation to debug menu - for now lets keep it on top to check collisions and interactions
    if(debug_mode)
    {
        if(pl_dir[player_num]=='w') DrawTextureRec(OldeAssets.AvatarsSprite,tiles[8],local_player_pos,WHITE);
        if(pl_dir[player_num]=='x') DrawTextureRec(OldeAssets.AvatarsSprite,tiles[9],local_player_pos,WHITE);
        if(pl_dir[player_num]=='a') DrawTextureRec(OldeAssets.AvatarsSprite,tiles[18],local_player_pos,WHITE);
        if(pl_dir[player_num]=='d') DrawTextureRec(OldeAssets.AvatarsSprite,tiles[19],local_player_pos,WHITE);

        if(pl_dir[player_num]=='q') DrawTextureRec(OldeAssets.AvatarsSprite,tiles[28],local_player_pos,WHITE);
        if(pl_dir[player_num]=='e') DrawTextureRec(OldeAssets.AvatarsSprite,tiles[29],local_player_pos,WHITE);
        if(pl_dir[player_num]=='z') DrawTextureRec(OldeAssets.AvatarsSprite,tiles[38],local_player_pos,WHITE);
        if(pl_dir[player_num]=='c') DrawTextureRec(OldeAssets.AvatarsSprite,tiles[39],local_player_pos,WHITE);
    }
}

void Game_Player::DrawPlayers(Game_Assets &OldeAssets,float &d_time)
// that means local player too
{
    if(d_time>0.0 && d_time<0.5) current_draw_offset=0;
    if(d_time>0.5 && d_time<1.0) current_draw_offset=1;
    if(d_time>1.0 && d_time<1.5) current_draw_offset=0;
    if(d_time>1.5 && d_time<2.0) current_draw_offset=1;
    if(d_time>2.0 && d_time<2.5) current_draw_offset=0;
    if(d_time>2.5 && d_time<3.0) current_draw_offset=1;

    for(int i=0;i<max_clients;i++)
    {
        if(pl_active[i]==true)
        {
            if(pl_dir[i]=='q' || pl_dir[i]=='a' || pl_dir[i]=='z')
            {
                pl_side[i]=0;
            }
            if(pl_dir[i]=='e' || pl_dir[i]=='d' || pl_dir[i]=='c')
            {
                pl_side[i]=4;
            }
            int avatar_offset=0;
            if(pl_avatar[i]==0) avatar_offset=0;
            else if(pl_avatar[i]==1) avatar_offset=40;
            else if(pl_avatar[i]==2) avatar_offset=80;
            else if(pl_avatar[i]==3) avatar_offset=120;
            else avatar_offset=0;

            int offset=0;

            if(pl_act[i]=='w')
            {
                if(pl_act[i]==player_num)
                {
                    offset=avatar_offset+pl_side[i]+10;
                    DrawTextureRec(OldeAssets.AvatarsSprite,tiles[offset+current_draw_offset],local_player_pos,WHITE);
                }
                else
                {
                    offset=avatar_offset+pl_side[i]+10;
                    DrawTextureRec(OldeAssets.AvatarsSprite,tiles[offset+current_draw_offset],pl_pointer_pos[i],WHITE);
                }
            }
            else
            {
                if(pl_act[i]==player_num)
                {
                    offset=avatar_offset+pl_side[i];
                    DrawTextureRec(OldeAssets.AvatarsSprite,tiles[offset],local_player_pos,WHITE);
                }
                else
                {
                    offset=avatar_offset+pl_side[i];
                    DrawTextureRec(OldeAssets.AvatarsSprite,tiles[offset],pl_pointer_pos[i],WHITE);
                }
            }
        }
    }
    // for(int i=0;i<max_clients;i++)
    // {
    //     if(pl_active[i]==true) // && pl_active[i]!=player_num)
    //     {
    //         if(pl_dir[i]=='w' || pl_dir[i]=='q' || pl_dir[i]=='e')
    //         {
    //             if(pl_act[i]=='w')
    //             {
    //                 DrawTextureRec(OldeAssets.AvatarsSprite,tiles[0+current_draw_offset],pl_pointer_pos[i],WHITE);
    //             }
    //             else DrawTextureRec(OldeAssets.AvatarsSprite,tiles[0],pl_pointer_pos[i],WHITE);
    //         }
    //         if(pl_dir[i]=='x' || pl_dir[i]=='z' || pl_dir[i]=='c')
    //         {
    //             if(pl_act[i]=='w')
    //             {
    //                 DrawTextureRec(OldeAssets.AvatarsSprite,tiles[10+current_draw_offset],pl_pointer_pos[i],WHITE);
    //             }
    //             else DrawTextureRec(OldeAssets.AvatarsSprite,tiles[10],pl_pointer_pos[i],WHITE);
    //         }
    //         if(pl_dir[i]=='w' || pl_dir[i]=='q' || pl_dir[i]=='e')
    //         {
    //             if(pl_act[i]=='w')
    //             {
    //                 DrawTextureRec(OldeAssets.AvatarsSprite,tiles[0+current_draw_offset],pl_pointer_pos[i],WHITE);
    //             }
    //             else DrawTextureRec(OldeAssets.AvatarsSprite,tiles[0],pl_pointer_pos[i],WHITE);
    //         }

    //         if(pl_dir[i]=='x' || pl_dir[i]=='z' || pl_dir[i]=='c')
    //         {
    //             if(pl_act[i]=='w')
    //             {
    //                 DrawTextureRec(OldeAssets.AvatarsSprite,tiles[10+current_draw_offset],pl_pointer_pos[i],WHITE);
    //             }
    //             else DrawTextureRec(OldeAssets.AvatarsSprite,tiles[10],pl_pointer_pos[i],WHITE);
    //         }

    //         if(pl_dir[i]=='a')
    //         {
    //             if(pl_act[i]=='w')
    //             {
    //                 DrawTextureRec(OldeAssets.AvatarsSprite,tiles[20+current_draw_offset],pl_pointer_pos[i],WHITE);
    //             }
    //             else DrawTextureRec(OldeAssets.AvatarsSprite,tiles[20],pl_pointer_pos[i],WHITE);
    //         }

    //         if(pl_dir[i]=='d')
    //         {
    //             if(pl_act[i]=='w')
    //             {
    //                 DrawTextureRec(OldeAssets.AvatarsSprite,tiles[30+current_draw_offset],pl_pointer_pos[i],WHITE);
    //             }
    //             else DrawTextureRec(OldeAssets.AvatarsSprite,tiles[30],pl_pointer_pos[i],WHITE);
    //         }
    // }


    // }
}

void Game_Player::SetLocalPlayerNumber(int num,uint8_t av_num)
{
    pl_dir[num]='a';
    pl_act[num]='i';
    pl_avatar[num]=av_num;
}