#include "Game.h"
#include "Player.h" 
#include <iostream>
#include <string>
#include <memory>


Game::Game(int width, int height)
    : windowWidth(width), windowHeight(height),
    gameState(GameState::TITLE), uiState(UI_NONE),                    
    stageClearTimer(1.0f),
    player(nullptr)
    
{
}

void Game::Init()
{
    std::cout << "working directory: " << GetWorkingDirectory() << std::endl;
    std::cout << "init started" << std::endl;
    InitWindow(windowWidth, windowHeight, "DASH");
    SetTargetFPS(60);

    // **加载 UI & 背景**
    titleScreen = LoadTexture("textures/title.png");
    gameOver = LoadTexture("textures/gameover.png");
    background = LoadTexture("textures/long far.png");
    midground = LoadTexture("textures/long mid.png");
    foreground = LoadTexture("textures/long close.png");
    score = LoadTexture("textures/score.png");
    heartIcon = LoadTexture("textures/heart.png");
	// **加载音效**
    InitAudioDevice(); // 初始化音频系统
    damageSound = LoadSound("sound/damage.mp3");
    enemyHitSound = LoadSound("sound/enemy_hit.mp3");
    if (enemyHitSound.frameCount == 0) {
        std::cerr << "energy.mp3 failed to load\n";
        return;
    }

    scoreAnim = { { 0.0f, 0.0f, static_cast<float>(score.width / 24), static_cast<float>(score.height) },
                   0, 0.1f, 0.0f };
    // **初始化 Player**
    player = std::make_unique<Player>();
    player->Init({ 200, 400 });

    energyManager.Init(); // **初始化能量**
    enemyManager.Init();
    resultTimer = 5.0f;
    
    InitCamera();
  
}

void Game::InitCamera() {
    camera.target = player->GetPosition();
    camera.offset = { (float)windowWidth / 2 - 400, (float)windowHeight / 2 + 200 }; // 用同一套值
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;
}

void Game::Update()
{
    float deltaTime = GetFrameTime();  // **Game 统一计算 `deltaTime`**
    if (uiState == UI_GAME_OVER)
    {
        // 玩家倒地后已完成 FALL，等待 R 键
        if (IsKeyPressed(KEY_R)) ResetGame();
        return;
    }

    if (uiState == UI_STAGE_CLEAR)
    {
        // 玩家处于 ULT 动画中，等待 R 重置
        player->Update(deltaTime);  // 继续播放 ULT 动画
        if (IsKeyPressed(KEY_R)) ResetGame();
        return;
    }

    if (gameState == GameState::STAGE)
    {
        player->Update(deltaTime);
        energyManager.Update(deltaTime, player.get());
        enemyManager.Update(deltaTime);
        HandleCollisions(deltaTime);
        // **相机跟随 Player 横向移动**
        Vector2 playerPos = player->GetPosition();
        camera.target.x = playerPos.x;
        player->GetHP();
     
        if (player->GetIsDown()) {
            uiState = UI_GAME_OVER;
            return;
        }

        if (energyManager.Getscore() >= 10) {
            stageClearTimer -= deltaTime;   
            player->SetState(ULT);
            
            if (stageClearTimer <= 0.0f) {
                uiState = UI_STAGE_CLEAR;
            }
        }
        for (auto it = particleGs.begin(); it != particleGs.end(); ) {
            it->pos.x += it->vel.x * deltaTime;
            it->pos.y += it->vel.y * deltaTime;
            it->life -= deltaTime;
            if (it->life <= 0) {
                it = particleGs.erase(it);
            }
            else {
                ++it;
            }
        }
        // 相机跟随放在最后
        camera.target.x = player->GetPosition().x;
        // 添加屏幕震动
        if (shakeDuration > 0.0f) {
            float shakeX = GetRandomValue(-100, 100) / 100.0f * shakeMagnitude;
            float shakeY = GetRandomValue(-100, 100) / 100.0f * shakeMagnitude;
            camera.offset.x += shakeX;
            camera.offset.y += shakeY;
            shakeDuration -= deltaTime;
        }
    } //
   
}

void Game::HandleCollisions(float deltaTime)
{
    Rectangle attackRect = player->GetAttackRect(); // A
    Rectangle playerRect = player->GetCollisionRect();   // B
    bool isAttacking = (player->GetState() == ATK);

    for (auto& enemy : enemyManager.GetEnemies())
    {
        if (!enemy.IsAlive()) continue;

        Rectangle enemyRect = enemy.GetCollisionRect();  // C

        // A ∩ C
        if (isAttacking && CheckCollisionRecs(attackRect, enemyRect))
        {
            PlaySound(enemyHitSound);
            Vector2 enemyCenter = { enemyRect.x + enemyRect.width / 2, enemyRect.y + enemyRect.height / 2 };
            SpawnParticle(enemyCenter, RED, 15);
            enemy.Kill();
            continue;
        }

        // 受伤：B ∩ C 且非无敌
        if (!player->isInvincible && CheckCollisionRecs(playerRect, enemyRect))
        {
            player->Damage();  // 扣血 + 播放受伤动画
            TriggerCameraShake(0.2f, 5.0f);  // 震动 0.2 秒，强度 5 像素
            PlaySound(damageSound);
            break;  // 只受一次伤
        }
        
        }
    }
void Game::SpawnParticle(Vector2 position, Color color, int count = 10)
{
    for (int i = 0; i < count; ++i) {
        float angle = GetRandomValue(-30, 90) * DEG2RAD;
        float speed = GetRandomValue(150, 200);
        ParticleG p;
        p.pos = position;
        p.vel = { cosf(angle) * speed, sinf(angle) * speed };
        p.life = GetRandomValue(5, 15) / 10.0f;
        p.color = color;
        particleGs.push_back(p);
    }
}

void Game::Draw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode2D(camera);

    // **背景滚动**
    float camX = camera.target.x;
    float bgOffsetX = -camX * 0.1f;
    float mgOffsetX = -camX * 0.4f;
    float fgOffsetX = -camX * 0.6f;

    DrawTexture(background, (int)bgOffsetX, 0, WHITE);
    DrawTexture(midground, (int)mgOffsetX, 0, WHITE);
    DrawTexture(foreground, (int)fgOffsetX, 0, WHITE);

    //test collision
    DrawRectangleLinesEx(player->GetAttackRect(), 1, RED);
    DrawRectangleLinesEx(player->GetCollisionRect(), 1, GREEN);
    
    switch (gameState)
    {
    case GameState::TITLE:
        EndMode2D();
        //camera.zoom = 2.0;
        DrawTexture(titleScreen, 0, 0, WHITE);
        DrawText("DASH", 700, 200, 50, BLACK);
        DrawText("Press ENTER to Start!", 500, 400, 40, WHITE);
        if (IsKeyPressed(KEY_ENTER)) gameState = GameState::STAGE;
        break;

    case GameState::STAGE:

        DrawTexture(score, camera.target.x - 80, camera.target.y - 305.0f, WHITE);
        DrawText((std::to_string(energyManager.Getscore()) + "/10").c_str(),
            camera.target.x - 50.0f, camera.target.y - 298.0f, 20, WHITE);
        DrawText("* <A>: BACK  * <D>: FORWARD   * <W>: JUMP  * <SPACE>: DASH  * <ATTACK>: MOUSE LEFT CLICK",
            camera.target.x + 10.0f, camera.target.y - 250.0f, 10, WHITE);
        energyManager.Draw();
        enemyManager.Draw();
        player->Draw();
        //HP
        for (int i = 0; i < player->GetHP(); i++)
        {
            DrawTexture(heartIcon, camera.target.x - 180 + i * (heartIcon.width + 3), camera.target.y - 305.0f, WHITE);
        }
        for (const auto& p : particleGs) {
            DrawCircleV(p.pos, 2.0f, p.color); // 固定半径为 3
        }
        if (player->GetState() == ULT) {
            float t = GetTime();
            if (fmod(t, 0.2f) < 0.1f) {
                DrawRectangle(camera.target.x, camera.target.y, windowWidth, windowHeight, Fade(PURPLE, 0.1f));
            }
        }
        break;
    }
    
    if (uiState == UI_GAME_OVER) DrawGameOverUI();
    else if (uiState == UI_STAGE_CLEAR) DrawStageClearUI();


    EndDrawing();
}

void Game::DrawGameOverUI() {
    
    //Vector2 gameOverPos = {
    //    camera.target.x - (windowWidth / 2.0f - 400) / camera.zoom,
    //camera.target.y - (windowHeight / 2.0f + 200) / camera.zoom
    //};
    Vector2 centerScreen = { windowWidth / 2.0f, windowHeight / 2.0f };
    //DrawTexture(gameOver,gameOverPos.x,gameOverPos.y, WHITE);
    DrawTexturePro(gameOver,
        { 0, 0, (float)gameOver.width, (float)gameOver.height }, // 源
        { centerScreen.x, centerScreen.y, (float)gameOver.width, (float)gameOver.height }, // 目标
        { gameOver.width / 2.0f, gameOver.height / 2.0f }, // origin 居中
        0.0f, WHITE);
    // 文字中心对齐（可选）
    const char* text1 = "STAGE CLEAR!";
    const char* text2 = "Press R to Replay";

    int text1Width = MeasureText(text1, 50);
    int text2Width = MeasureText(text2, 30);

    DrawText(text1, centerScreen.x - text1Width / 2, centerScreen.y + 150, 50, GREEN);
    DrawText(text2, centerScreen.x - text2Width / 2, centerScreen.y + 210, 30, WHITE);
}

void Game::DrawStageClearUI() {
    Vector2 gameOverPos = {
       camera.target.x - (windowWidth / 2.0f - 400) / camera.zoom,
   camera.target.y - (windowHeight / 2.0f + 200) / camera.zoom
    };
    DrawText("STAGE CLEAR!", gameOverPos.x, gameOverPos.y, 50, GREEN);
    DrawText("Press R to Replay", gameOverPos.x, gameOverPos.y, 30, WHITE);
}

void Game::Uninit()
{
    player.reset();
    energyManager.Unload();
    enemyManager.Unload();

    UnloadTexture(titleScreen);
    UnloadTexture(gameOver);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    UnloadTexture(score);
    UnloadTexture(heartIcon);

    UnloadSound(damageSound);
    UnloadSound(enemyHitSound);
}

void Game::ResetGame() {
    // 重建 Player 状态
    player->Init({ 200, 400 });
    //player->SetState(IDLE);  // 回到默认状态

    // 重置能量与敌人
    energyManager.Reset();
    enemyManager.Reset();

    // UI 状态和计时器
    uiState = UI_NONE;
    stageClearTimer = 1.0f;

    // 游戏状态
    gameState = GameState::STAGE;

    // 相机重新跟随
    InitCamera();
}

void Game::TriggerCameraShake(float duration, float magnitude)
{
    shakeDuration = duration;
    shakeMagnitude = magnitude;
}

Game::~Game()
{
    Uninit();
    CloseWindow();
}
