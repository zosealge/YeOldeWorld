#include"../camera.hpp"

PlayerCamera::PlayerCamera()
{
    cam.offset.x=((GetScreenWidth()/2)-tilesize);
    cam.offset.y=((GetScreenHeight()/2)-tilesize);
    // target will be set from map integers
    cam.rotation=0.0f;
    cam.zoom=4.0f;
    scroll_speed=0.25f;
}

void PlayerCamera::Move(Vector2 &player_pos)
{
    if(player_pos.y<cam.target.y) cam.target.y-=scroll_speed;
    if(player_pos.y>cam.target.y) cam.target.y+=scroll_speed;

    if(player_pos.x<cam.target.x) cam.target.x-=scroll_speed;
    if(player_pos.x>cam.target.x) cam.target.x+=scroll_speed;
}

void PlayerCamera::ChangeSpeed(float new_speed)
{
    scroll_speed=new_speed;
}