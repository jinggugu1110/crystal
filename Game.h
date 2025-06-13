#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "Player.h"
#include "EnergyManager.h"
#include "EnemyManager.h"
#include <memory> 

enum class GameState
{
    TITLE,
    STAGE,
    GAMEOVER,
    CLEAR
};
enum UIState {
    UI_NONE,
    UI_GAME_OVER,
    UI_STAGE_CLEAR
};

struct ParticleG {
    Vector2 pos;
    Vector2 vel;
    float life;
    Color color;
};



class Game
{
private:
    int windowWidth;
    int windowHeight;
    GameState gameState;
    int resultType;
    std::vector<ParticleG> particleGs;

	Music bgm;

    // **背景 & UI 资源**
    Texture2D titleScreen;
    Texture2D background, midground, foreground;
    Texture2D score;
    Texture2D heartIcon;

    Texture2D gameOver;
    Texture2D stageClear;
    Texture2D pressRTex;

    AnimData scoreAnim;
    Sound damageSound;
	Sound enemyHitSound;
    // **游戏对象**
    //Player* player;
    std::unique_ptr<Player> player;
    EnergyManager energyManager;
    EnemyManager enemyManager;
    //shake
    float shakeDuration = 0.0f;     // 剩余震动时间
    float shakeMagnitude = 5.0f;    // 震动强度
    //Camera
    Camera2D camera;

    Vector2 energyUIpos = { 100,100 };
    Vector2 pointUIpos = { 200, 100 };
    Vector2 ultpos = { 300, 100 };
    float resultTimer=0.0f;

    UIState uiState;
    float stageClearTimer;
    bool showReplayButton;
    void InitCamera();

    std::vector<Particle> enemyParticles;
public:
    Game(int width, int height);
    ~Game();

    void Init();    // **资源初始化**
    void Update();  // **统一更新对象**
    void HandleCollisions(float deltaTime);
    void SpawnParticle(Vector2 position, Color color, int count);
    void Draw();   // **统一绘制对象**
    void DrawGameOverUI();
    void DrawStageClearUI();
    void Uninit();  // **统一释放资源**
    void ResetGame();
    void TriggerCameraShake(float duration, float magnitude);
};

#endif
