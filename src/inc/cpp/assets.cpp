#include"../assets.hpp"

Game_Assets::Game_Assets()
{
    int num=0;
    for (int y=0;y<9;y++)
    {
        for(int x=0;x<3;x++)
        {
            MenuRectangle[num].x=x*menu_tilesize;
            MenuRectangle[num].y=y*menu_tilesize;
            MenuRectangle[num].width=menu_tilesize;
            MenuRectangle[num].height=menu_tilesize;
            num++;
        }
    }
    LoadAssets();
}

Game_Assets::~Game_Assets()
{
    UnloadAssets();
    CloseWindow();
}

void Game_Assets::ReloadAssets()
{
    UnloadAssets();
    LoadAssets();
}

void Game_Assets::LoadAssets()
{
    GameFont=LoadFont("res/Nes.ttf");
    GroundSprite=LoadTexture("res/ground.png");
    EtcSprite=LoadTexture("res/etc.png");
    AvatarsSprite=LoadTexture("res/avatars.png");
    ObjectsSprite=LoadTexture("res/objects.png"); 
    MenuSprite=LoadTexture("res/menu.png");
    RoofsSprite=LoadTexture("res/roofs.png");
    EditorHelpPanel=LoadTexture("res/editor_help.png");
}

void Game_Assets::UnloadAssets()
{
    UnloadFont(GameFont);
    UnloadTexture(GroundSprite);
    UnloadTexture(EtcSprite);
    UnloadTexture(AvatarsSprite);
    UnloadTexture(ObjectsSprite);
    UnloadTexture(MenuSprite);
    UnloadTexture(RoofsSprite);
    UnloadTexture(EditorHelpPanel);
}