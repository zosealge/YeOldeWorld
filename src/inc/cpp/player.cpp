#include"../player.hpp"

Game_Player::Game_Player()
{
    local_use=false;
    local_player_pos.x=0;
    local_player_pos.y=0;
    current_tile_left=0;
    current_tile_right=current_tile_left+50;
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


    if(d_time>0.0 && d_time<0.5) current_draw_offset=1;
    if(d_time>0.5 && d_time<1.0) current_draw_offset=2;
    if(d_time>1.0 && d_time<1.5) current_draw_offset=1;
    if(d_time>1.5 && d_time<2.0) current_draw_offset=2;
    if(d_time>2.0 && d_time<2.5) current_draw_offset=1;
    if(d_time>2.5 && d_time<3.0) current_draw_offset=2;

    if(pl_dir[player_num]=='w' || pl_dir[player_num]=='q' || pl_dir[player_num]=='e')
    {
        if(pl_act[player_num]=='w')
        {
            DrawTextureRec(OldeAssets.AvatarsSprite,tiles[0+current_draw_offset],local_player_pos,WHITE);
        }
        else DrawTextureRec(OldeAssets.AvatarsSprite,tiles[0],local_player_pos,WHITE);
    }

    if(pl_dir[player_num]=='x' || pl_dir[player_num]=='z' || pl_dir[player_num]=='c')
    {
        if(pl_act[player_num]=='w')
        {
            DrawTextureRec(OldeAssets.AvatarsSprite,tiles[10+current_draw_offset],local_player_pos,WHITE);
        }
        else DrawTextureRec(OldeAssets.AvatarsSprite,tiles[10],local_player_pos,WHITE);
    }

    if(pl_dir[player_num]=='a')
    {
        if(pl_act[player_num]=='w')
        {
            DrawTextureRec(OldeAssets.AvatarsSprite,tiles[20+current_draw_offset],local_player_pos,WHITE);
        }
        else DrawTextureRec(OldeAssets.AvatarsSprite,tiles[20],local_player_pos,WHITE);
    }

    if(pl_dir[player_num]=='d')
    {
        if(pl_act[player_num]=='w')
        {
            DrawTextureRec(OldeAssets.AvatarsSprite,tiles[30+current_draw_offset],local_player_pos,WHITE);
        }
        else DrawTextureRec(OldeAssets.AvatarsSprite,tiles[30],local_player_pos,WHITE);
    }

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

void Game_Player::DrawNetworkPlayer(Game_Assets &OldeAssets,float &d_time)
{
    if(d_time>0.0 && d_time<0.5) current_draw_offset=1;
    if(d_time>0.5 && d_time<1.0) current_draw_offset=2;
    if(d_time>1.0 && d_time<1.5) current_draw_offset=1;
    if(d_time>1.5 && d_time<2.0) current_draw_offset=2;
    if(d_time>2.0 && d_time<2.5) current_draw_offset=1;
    if(d_time>2.5 && d_time<3.0) current_draw_offset=2;

    for(int i=0;i<max_clients;i++)
    {
        if(pl_active[i]==true)
        {
            if(pl_dir[i]=='w' || pl_dir[i]=='q' || pl_dir[i]=='e')
            {
                if(pl_act[i]=='w')
                {
                    DrawTextureRec(OldeAssets.AvatarsSprite,tiles[0+current_draw_offset],pl_pointer_pos[i],WHITE);
                }
                else DrawTextureRec(OldeAssets.AvatarsSprite,tiles[0],pl_pointer_pos[i],WHITE);
            }
            if(pl_dir[i]=='x' || pl_dir[i]=='z' || pl_dir[i]=='c')
            {
                if(pl_act[i]=='w')
                {
                    DrawTextureRec(OldeAssets.AvatarsSprite,tiles[10+current_draw_offset],pl_pointer_pos[i],WHITE);
                }
                else DrawTextureRec(OldeAssets.AvatarsSprite,tiles[10],pl_pointer_pos[i],WHITE);
            }
            if(pl_dir[i]=='w' || pl_dir[i]=='q' || pl_dir[i]=='e')
            {
                if(pl_act[i]=='w')
                {
                    DrawTextureRec(OldeAssets.AvatarsSprite,tiles[0+current_draw_offset],pl_pointer_pos[i],WHITE);
                }
                else DrawTextureRec(OldeAssets.AvatarsSprite,tiles[0],pl_pointer_pos[i],WHITE);
            }

            if(pl_dir[i]=='x' || pl_dir[i]=='z' || pl_dir[i]=='c')
            {
                if(pl_act[i]=='w')
                {
                    DrawTextureRec(OldeAssets.AvatarsSprite,tiles[10+current_draw_offset],pl_pointer_pos[i],WHITE);
                }
                else DrawTextureRec(OldeAssets.AvatarsSprite,tiles[10],pl_pointer_pos[i],WHITE);
            }

            if(pl_dir[i]=='a')
            {
                if(pl_act[i]=='w')
                {
                    DrawTextureRec(OldeAssets.AvatarsSprite,tiles[20+current_draw_offset],pl_pointer_pos[i],WHITE);
                }
                else DrawTextureRec(OldeAssets.AvatarsSprite,tiles[20],pl_pointer_pos[i],WHITE);
            }

            if(pl_dir[i]=='d')
            {
                if(pl_act[i]=='w')
                {
                    DrawTextureRec(OldeAssets.AvatarsSprite,tiles[30+current_draw_offset],pl_pointer_pos[i],WHITE);
                }
                else DrawTextureRec(OldeAssets.AvatarsSprite,tiles[30],pl_pointer_pos[i],WHITE);
            }
        }


    }
}

void Game_Player::SetPlayerNumber(int num)
{
    player_num=num;
    pl_dir[player_num]='a';
    pl_act[player_num]='i';
}