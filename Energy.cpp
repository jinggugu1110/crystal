#include "Energy.h"

// **构造函数**
Energy::Energy(Vector2 pos, Texture2D* sharedTexture) : position(pos), energyTexture(sharedTexture), isActive(true)
{
    energyAnim = { { 0.0f, 0.0f, static_cast<float>(energyTexture->width/24), static_cast<float>(energyTexture->height) },
                   0, 0.1f, 0.0f };
}

// **更新动画**
void Energy::Update(float deltaTime)
{
    if (isActive)
    {
        energyAnim.runningTime += deltaTime;
        if (energyAnim.runningTime >= energyAnim.updateTime)
        {
            energyAnim.runningTime = 0.0f;
            energyAnim.frame = (energyAnim.frame + 1) % 24;
            energyAnim.rec.x = energyAnim.frame * energyAnim.rec.width;
        }
    }
   
}

// **绘制能量**
void Energy::Draw()
{
    if (isActive && energyTexture)
    {
        DrawTexturePro(*energyTexture, energyAnim.rec, { position.x, position.y, energyAnim.rec.width, energyAnim.rec.height},
            { 0, 0 }, 0, WHITE);
        // [调试用] 绘制碰撞矩形边框（绿色）
        //DrawRectangleLinesEx(
        //    { position.x+5, position.y, energyAnim.rec.width*0.6f, energyAnim.rec.height },
        //    1, GREEN
        //);
    }

}

// **收集后消失**
void Energy::Deactivate()
{
    isActive = false;
}
