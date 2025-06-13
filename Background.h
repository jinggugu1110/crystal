#pragma once
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "raylib.h"

class Background
{
private:
    Texture2D background, midground, foreground;
    float bgX, mgX, fgX;

public:
    Background();
    ~Background();
    void Draw(float deltaTime);
};

#endif
