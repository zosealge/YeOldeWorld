#pragma once
#include"raylib.h"

class Game_Assets
{
    public:
    const int menu_tilesize=64;

    Font GameFont;
    Texture2D GroundSprite;
    Texture2D EtcSprite;
    Texture2D AvatarsSprite;
    Texture2D ObjectsSprite;
    Texture2D RoofsSprite;
    Texture2D MenuSprite;
    Texture2D EditorHelpPanel;
    Rectangle MenuRectangle[101];

    Game_Assets();
    ~Game_Assets();

    void ReloadAssets();

    private:
    void LoadAssets();

    void UnloadAssets();
};