#include"../gui.hpp"

PlayerGui::PlayerGui()
{
    zoom=4;
    header.x=0;
    header.y=0;
    header.width=GetScreenWidth();
    header.height=(tilesize)*4;

    inventory.x=(GetScreenWidth()/4)*3;
    inventory.y=(tilesize)*4;
    inventory.width=(GetScreenWidth()/4);
    inventory.height=GetScreenHeight();
}

void PlayerGui::DrawGui()
{
    // 1280 width
    // 720 height
    DrawRectangleRec(header,DARKBROWN);
    DrawRectangleRec(inventory,DARKBROWN);
}