#include "Enemy.h"

Enemy::Enemy(Vector2 pos, Texture2D* tex)
    : position(pos), texture(tex)
{
    width = (float)texture->width / 6;   // 6?¡üsprite sheet
    height = (float)texture->height;
    anim = { { 0.0f, 0.0f, width, height }, 0, 0.1f, 0.0f };  // n?æ?
    isAlive = true;
}

void Enemy::Update(float deltaTime)
{
    if (!isAlive) return;

    anim.runningTime += deltaTime;
    if (anim.runningTime >= anim.updateTime)
    {
        anim.runningTime = 0.0f;
        anim.frame = (anim.frame + 1) % 6;
        anim.rec.x = anim.frame * anim.rec.width;
    }
}

void Enemy::Draw()
{
    if (!isAlive) return;

    DrawTexturePro(*texture,
        anim.rec,
        { position.x, position.y, anim.rec.width, anim.rec.height },
        { 0, 0 }, 0.0f, WHITE);
	DrawRectangleLinesEx(GetCollisionRect(), 1, RED); // ??pC?¦?³y
}

void Enemy::Kill()
{
    isAlive = false;
	
}

bool Enemy::IsAlive() const
{
    return isAlive;
}

Rectangle Enemy::GetCollisionRect() const
{
    return {
        position.x+5.0f,
        position.y,
        anim.rec.width * 0.6f,   // ?³?xiÂ?®j
        anim.rec.height * 0.8f   // ?³xiÂ?®j
    };
}