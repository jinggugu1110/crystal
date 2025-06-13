#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

void resetGame(AnimData &scarfyData, AnimData nebulae[], int sizeOfNebulae, int windowWidth, int windowHeight, Texture2D nebula)
{
    scarfyData.pos.x = windowWidth / 2 - scarfyData.rec.width / 2;
    scarfyData.pos.y = windowHeight - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.runningTime = 0.0;

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].pos.x = windowWidth + i * 300;
        nebulae[i].pos.y = windowHeight - nebula.height / 1;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
    }
}

int main()
{
    int windowDimensions[2] = {800, 500};
    InitWindow(windowDimensions[0], windowDimensions[1], "JUMP!");

    const int gravity{1'000};
    Texture2D nebula = LoadTexture("textures/slime_run left.png");
    const int sizeOfNebulae{10};
    AnimData nebulae[sizeOfNebulae]{};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        float randomSpacing = static_cast<float>(GetRandomValue(200, 500)); // Random spacing between 200 and 500
        nebulae[i].rec = { 0.0, 0.0, static_cast<float>(nebula.width / 6), static_cast<float>(nebula.height) };
        nebulae[i].pos = {
            static_cast<float>(windowDimensions[0] + i * randomSpacing),
            static_cast<float>(windowDimensions[1] - nebula.height) };
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 0.0;
    }
    float finishLine{ nebulae[sizeOfNebulae - 1].pos.x };
    int nebVel{-200};

    Texture2D scarfy = LoadTexture("textures/Walking_KG_1.png");
    AnimData scarfyData = {
        {0.0, 0.0, static_cast<float>(scarfy.width / 7), static_cast<float>(scarfy.height)},
        {static_cast<float>(windowDimensions[0] / 2 - scarfy.width / 14), static_cast<float>(windowDimensions[1] - scarfy.height)},
        0,
        1.0f / 12.0f,
        0.0f
    };

    bool isInAir{};
    const int jumpVel{-400};
    int velocity{0};

    Texture2D background = LoadTexture("textures/far.png");
    float bgX{};
    Texture2D midground = LoadTexture("textures/mid.png");
    float mgX{};
    Texture2D foreground = LoadTexture("textures/close.png");
    float fgX{};

    bool collision{};
    bool gameOver{};
    bool win{};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        const float dT{GetFrameTime()};
        
        if (!gameOver && !win)
        {
            
            if (isOnGround(scarfyData, windowDimensions[1]))
            {
                velocity = 0;
                isInAir = false;
            }
            else
            {
                velocity += gravity * dT;
                isInAir = true;
            }

            if (IsKeyPressed(KEY_SPACE) && !isInAir)
            {
                velocity += jumpVel;
            }

            for (int i = 0; i < sizeOfNebulae; i++)
            {
                nebulae[i].pos.x += nebVel * dT;
            }

            finishLine += nebVel * dT;
            scarfyData.pos.y += velocity * dT;

            if (!isInAir)
            {
                scarfyData = updateAnimData(scarfyData, dT, 5);
            }

            for (int i = 0; i < sizeOfNebulae; i++)
            {
                nebulae[i] = updateAnimData(nebulae[i], dT, 7);
            }

            for (AnimData nebula : nebulae)
            {
                Rectangle nebRec{nebula.pos.x+15, nebula.pos.y+15, nebula.rec.width-15, nebula.rec.height-15};
                Rectangle scarfyRec{scarfyData.pos.x, scarfyData.pos.y, scarfyData.rec.width, scarfyData.rec.height};
                if (CheckCollisionRecs(nebRec, scarfyRec))
                {
                    collision = true;
                    gameOver = true;
                }
            }

            if (scarfyData.pos.x >= finishLine)
            {
                win = true;
            }
        }
        
        BeginDrawing();
        ClearBackground(WHITE);

        bgX -= 20 * dT;
        if (bgX <= -background.width * 2) bgX = 0.0;
        mgX -= 40 * dT;
        if (mgX <= -midground.width * 2) mgX = 0.0;
        fgX -= 80 * dT;
        if (fgX <= -foreground.width * 2) fgX = 0.0;

        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        Vector2 bg2Pos{bgX + background.width * 2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        Vector2 mg2Pos{mgX + midground.width * 2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        Vector2 fg2Pos{fgX + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);
        DrawText("Press SPACE to jump!", windowDimensions[0] / 4, 20, 20, WHITE);
        if (gameOver)
        {
            DrawText("Game Over!", windowDimensions[0] / 4, windowDimensions[1] / 2, 40, RED);
            DrawText("Replay? Y/N", windowDimensions[0] / 4, windowDimensions[1] / 4 * 3, 40, WHITE);
            if (IsKeyPressed(KEY_Y))
            {
                resetGame(scarfyData, nebulae, sizeOfNebulae, windowDimensions[0], windowDimensions[1], nebula);
                gameOver = false;
                collision = false;
            }
        }
        else if (win)
        {
            DrawText("You Win!", windowDimensions[0] / 4, windowDimensions[1] / 2, 40, RED);
            DrawText("Replay? Y/N", windowDimensions[0] / 4, windowDimensions[1] / 4 * 3, 40, WHITE);
            if (IsKeyPressed(KEY_Y))
            {
                resetGame(scarfyData, nebulae, sizeOfNebulae, windowDimensions[0], windowDimensions[1], nebula);
                gameOver = false;
                collision = false;
                win = false;
            }
        }
        else
        {
            for (int i = 0; i < sizeOfNebulae; i++)
            {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }

        EndDrawing();
    }

    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}