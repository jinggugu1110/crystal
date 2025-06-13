#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "Anim.h"

enum PlayerState
{
    IDLE,
    WALK,
    JUMP,
    DASH,
    ATK,
    ULT,
    HURT,
    FALL,
};

class Player
{
private:
    Texture2D idleTexture;
    Texture2D walkTexture;
    Texture2D jumpTexture;
    Texture2D dashTexture;
    Texture2D atkTexture;
    Texture2D ultTexture;
    Texture2D hurtTexture;
    Texture2D fallTexture;
    
    AnimData idleAnim;
    AnimData walkAnim;
    AnimData jumpAnim;
    AnimData dashAnim;
    AnimData atkAnim;
    AnimData ultAnim;
    AnimData hurtAnim;
    AnimData fallAnim;

    PlayerState currentState;
    PlayerState nextState;
   
    Vector2 position;
    float velocityY;
    bool isJumping;
    bool isDashing;
    bool isUlt;
    bool isDown=false;

    int hp = 3;
    const float gravity = 600.0f;
    const float jumpForce = -300.0f;
    const float groundLevel = 400.0f;
    const float dashSpeed = 400.0f;
    const float dashTime = 0.3f;
    float dashTimer;
    float ultTimer;
    float atkTimer;
    float hurtTimer;
	float fallTimer=0.5f;
    // **音频资源**
    Sound walkSound;
    Sound jumpSound;
    Sound attackSound;
    Sound hurtSound;
    Sound getEnergySound;
   
public:
    Player();
    ~Player();
    bool isInvincible = false;
    float invincibilityTimer = 0.3f;
    float knockbackVelocityX = -200.0f; // 向左推动，可根据敌人方向动态设置
    void updateAnimData(AnimData& data, float deltaTime, int maxFrame);
    void Init(Vector2 startPos);
    void HandleIdle(float deltaTime);
    void HandleInput();
    void HandleWalk(float deltaTime);
    void HandleJump(float deltaTime);
    void HandleDash(float deltaTime);
    void HandleAttack(float deltaTime);
    void HandleUlt(float deltaTime);
    void HandleHurt(float deltaTime);
    void HandleFall(float deltaTime);
    void Update(float deltaTime);
    void Draw();
    void SetState(PlayerState newState);
    Vector2 GetPosition() const;
    PlayerState GetState() const;
	bool GetIsDown() const;
   
    //HP 
    void Damage();
    //void InitHp(Vector2 pos);
    int GetHP() const;
    
    Rectangle GetCollisionRect() const;
    Rectangle GetAttackRect() const;
   
    //bool IsDead() const;
};

#endif
