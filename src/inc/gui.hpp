#pragma once
#include"raylib.h"

class PlayerGui
{
    public:
    const int tilesize=16;
    PlayerGui();

    void DrawGui();

    private:
    Rectangle header;
    Rectangle inventory;
    int zoom;
    //Rectangle footer;
};