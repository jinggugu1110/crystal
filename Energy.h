#pragma once

#include "raylib.h"
#include "Anim.h"

//struct AnimData {
//    Rectangle rec;
//    int frame;
//    float updateTime;
//    float runningTime;
//};

class Energy
{
public:
    Vector2 position;
    Texture2D* energyTexture;  // **共享的能量贴图**
    AnimData energyAnim;       // **能量动画**
    bool isActive;

    Energy(Vector2 pos, Texture2D* sharedTexture);  // **传入共享的 `Texture2D`**

    void Update(float deltaTime);
    void Draw();
    void Deactivate();
};


