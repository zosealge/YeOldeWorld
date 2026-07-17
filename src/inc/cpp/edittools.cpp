#include"../edittools.hpp"

EditTools::EditTools()
{
    for(int i=0;i<(pngarray*pngarray);i++)
    {
        tiles[i].x=0;
        tiles[i].y=0;
        tiles[i].width=tilesize;
        tiles[i].height=tilesize;
    }

    int tilenum=0;
    for(int y=0;y<pngarray;y++)
    {
        for(int x=0;x<pngarray;x++)
        {
            tiles[tilenum].x=x*tilesize;
            tiles[tilenum].y=y*tilesize;
            tilenum++;
        }
    }
    is_spawn_set=false;
    which_spawn_set=-1;
    current_tile=0;
}

void EditTools::MovePointer(Vector2 &pos)
{
    pointer_pos=pos;

    cur_x=(int)(pos.x/tilesize);
    cur_y=(int)(pos.y/tilesize);
}

void EditTools::DrawPointer(Game_Assets &OldeAssets,float &d_time,bool debug_mode)
{
    if(d_time>0.0 && d_time<0.5) current_draw_offset=0;
    if(d_time>0.5 && d_time<1.0) current_draw_offset=1;
    if(d_time>1.0 && d_time<1.5) current_draw_offset=2;
    if(d_time>1.5 && d_time<2.0) current_draw_offset=3;
    if(d_time>2.0 && d_time<2.5) current_draw_offset=4;
    if(d_time>2.5 && d_time<3.0) current_draw_offset=5;
    // if(d_time>3.0 && d_time<3.5) current_draw_offset=6;
    // if(d_time>3.5 && d_time<4.0) current_draw_offset=7;
    DrawTextureRec(OldeAssets.EtcSprite,tiles[pointer_tile+current_draw_offset],(Vector2){(float)cur_x*tilesize,(float)cur_y*tilesize},WHITE);
    if(debug_mode) DrawDebugPointer();
    //DrawText(TextFormat("I'm on x%d y%d !",cur_x,cur_y),(pointer_pos.x)-16,(pointer_pos.y)-16,8,WHITE);
    if(is_spawn_set)
    {
        if(d_time>0.0 && d_time<0.5) DrawText(TextFormat("Spawn point %d set!",which_spawn_set),(pointer_pos.x)-16,(pointer_pos.y)+8,8,WHITE);
        // if(d_time>0.5 && d_time<1.0) current_draw_offset=1;
        if(d_time>1.0 && d_time<1.5) DrawText(TextFormat("Spawn point %d set!",which_spawn_set),(pointer_pos.x)-16,(pointer_pos.y)+8,8,GREEN);
        // if(d_time>1.5 && d_time<2.0) current_draw_offset=3;
        if(d_time>2.0 && d_time<2.5) DrawText(TextFormat("Spawn point %d set!",which_spawn_set),(pointer_pos.x)-16,(pointer_pos.y)+8,8,RED);
        // if(d_time>2.5 && d_time<3.0) current_draw_offset=5;
        if(d_time>2.5) is_spawn_set=false;
    }
}

void EditTools::DrawDebugPointer()
{
    DrawRectangle(pointer_pos.x,pointer_pos.y,tilesize/2,tilesize/2,GREEN);
    DrawText(TextFormat("%d",cur_x),pointer_pos.x,pointer_pos.y,8,RED);
    DrawText(TextFormat("%d",cur_y),pointer_pos.x,pointer_pos.y+8,8,RED);
}