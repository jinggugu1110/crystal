#include "raylib.h"
#include "Background.h"

Background::Background()
{
    background = LoadTexture("textures/far.png");
    if (background.id == 0)
    {
        TraceLog(LOG_ERROR, "Failed to load background!");
    }
}
