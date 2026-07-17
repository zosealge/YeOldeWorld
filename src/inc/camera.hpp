#pragma once
#include"raylib.h"

class PlayerCamera
{
    public:
    const int tilesize=16;
    Camera2D cam;

    PlayerCamera();
    void Move(Vector2 &player_pos);

    void ChangeSpeed(float new_speed);
    private:
    float scroll_speed;
};