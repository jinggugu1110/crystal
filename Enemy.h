#pragma once
#include "raylib.h"
#include "Anim.h"

class Enemy {
private:
    Vector2 position;
    Texture2D* texture;
    AnimData anim;
    bool isAlive = true;
    float width;
    float height;

public:
    Enemy(Vector2 pos, Texture2D* tex);

    void Update(float deltaTime);
    void Draw();
    void Kill();

    Rectangle GetCollisionRect() const;
    bool IsAlive() const;
};
