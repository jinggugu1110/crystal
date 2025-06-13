#include "Player.h"
#include <iostream>

Player::Player()
{
    idleTexture.id = 0;
    walkTexture.id = 0;
    jumpTexture.id = 0;
    dashTexture.id = 0;
    atkTexture.id = 0;
    ultTexture.id = 0;
    hurtTexture.id = 0;  
    fallTexture.id = 0;  

    currentState = IDLE;
    nextState = IDLE;
    velocityY = 0.0f;
    isJumping = false;
    isDashing = false;
    isUlt = false;
    dashTimer = 0.0f;
    atkTimer = 0.0f;
}

void Player::Init(Vector2 startPos)
{
    position = startPos;
    hp = 3;
    // === 重置所有状态 ===
    velocityY = 0.0f;
    isJumping = false;
    isDashing = false;
    isUlt = false;
    isDown = false;
    isInvincible = false;

    invincibilityTimer = 0.0f;
    hurtTimer = 0.0f;
    dashTimer = 0.0f;
    ultTimer = 0.0f;
    atkTimer = 0.0f;
    fallTimer = 0.0f;
    knockbackVelocityX = 0.0f;

    currentState = IDLE;
    nextState = IDLE;

    idleTexture = LoadTexture("textures/idle.png");
    walkTexture = LoadTexture("textures/walk.png");
    jumpTexture = LoadTexture("textures/jump.gif");
    dashTexture = LoadTexture("textures/dash1.png");
    atkTexture = LoadTexture("textures/atk.png");
    ultTexture = LoadTexture("textures/ult.png");
    hurtTexture = LoadTexture("textures/hurt.png");
    fallTexture = LoadTexture("textures/fall.png");

    //SE

    //walkSound = LoadSound("sound/walk.mp3");
    attackSound = LoadSound("sound/atk.mp3");

    idleAnim = { { 0.0f, 0.0f, static_cast<float>(idleTexture.width / 4), static_cast<float>(idleTexture.height) }, 0, 0.15f, 0.0f };
    walkAnim = { { 0.0f, 0.0f, static_cast<float>(walkTexture.width / 7), static_cast<float>(walkTexture.height) }, 0, 0.1f, 0.0f };
    jumpAnim = { { 0.0f, 0.0f, static_cast<float>(jumpTexture.width / 6), static_cast<float>(jumpTexture.height) }, 0, 0.12f, 0.0f };
    dashAnim = { { 0.0f, 0.0f, static_cast<float>(dashTexture.width / 4), static_cast<float>(dashTexture.height) }, 0, 0.1f, 0.0f };
    atkAnim = { { 0.0f, 0.0f, static_cast<float>(atkTexture.width / 6), static_cast<float>(atkTexture.height) }, 0, 0.1f, 0.0f };
    ultAnim = { { 0.0f, 0.0f, static_cast<float>(ultTexture.width / 10), static_cast<float>(ultTexture.height) }, 0, 0.1f, 0.0f };
    hurtAnim = { { 0.0f, 0.0f, static_cast<float>(hurtTexture.width / 4), static_cast<float>(hurtTexture.height) }, 0, 0.2f, 0.0f };
    fallAnim = { { 0.0f, 0.0f, static_cast<float>(fallTexture.width / 6), static_cast<float>(fallTexture.height) }, 0, 0.3f, 0.0f };

    SetState(IDLE);
}

void Player::HandleIdle(float deltaTime)
{
    if (position.y <= groundLevel) {
        
        velocityY += gravity * deltaTime;
        position.y += velocityY * deltaTime;
    }
    currentState = IDLE;
    isJumping = false;
}

// **处理输入**
void Player::HandleInput()
{
    nextState = currentState;

    if (IsKeyPressed(KEY_SPACE) && currentState != DASH)
    {
        nextState = DASH;
    }
    else if (IsKeyPressed(KEY_W) && currentState != JUMP && !isJumping)
    {
        nextState = JUMP;
    }
    else if ((IsKeyDown(KEY_D)|| IsKeyDown(KEY_A)) && currentState != JUMP && currentState != DASH)
    {
        nextState = WALK;
      
			
		
    }
    //else if (IsKeyPressed(KEY_X))
    //{
    //    nextState = ULT;
    //}
    else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)|| IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        nextState = ATK;
		PlaySound(attackSound);  // 播放攻击音效
    }
 
    else if (!IsKeyDown(KEY_D) && currentState == WALK)
    {
        nextState = IDLE;
    }
    
}

// **处理 WALK 逻辑**
void Player::HandleWalk(float deltaTime)
{
    if (IsKeyDown(KEY_D)) { position.x += 100 * deltaTime; }
    else if (IsKeyDown(KEY_A))
    {
        position.x -= 100 * deltaTime;
        if (position.x <= 200) {
            position.x = 200;
        }
    }
   if (position.y <= groundLevel) {

        velocityY += gravity * deltaTime;
        position.y += velocityY * deltaTime;
    }
   
}

void Player::HandleJump(float deltaTime)
{
    isJumping = true;
    if (IsKeyDown(KEY_D))
    {
        position.x += 100 * deltaTime;  // 允许在空中前进
        
    }

    velocityY += gravity * deltaTime;
    position.y += velocityY * deltaTime;

    if (position.y >= groundLevel)
    {
        position.y = groundLevel;
        velocityY = 0.0f;
        isJumping = false;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

            SetState(ATK);
            
        }
        if (IsKeyDown(KEY_SPACE))  // **防止落地时立即 DASH**
        {
            SetState(DASH);
        }
            SetState(IDLE);
    }
}


void Player::HandleDash(float deltaTime)
{
    position.x += dashSpeed * deltaTime;  // **DASH 持续冲刺**
 
    dashTimer -= deltaTime;
    if (dashTimer <= 0)
    {
        isDashing = false;

        if (position.y < groundLevel)  // **角色仍在空中**
        {
            currentState = IDLE;  // **恢复 JUMP 状态但不触发跳跃**
            isJumping = true;  // **确保 JUMP 继续执行**
        }
        else
        {
            SetState(IDLE);
        }
    }
}

void Player::HandleAttack(float deltaTime)
{
    // 播放攻击动画
    //updateAnimData(atkAnim, deltaTime, 4); // 假设攻击动画为4帧

    // 攻击计时
    atkTimer -= deltaTime;

    if (atkTimer <= 0.0f)
    {
       

        // 如果在空中，恢复到 JUMP；否则回到 IDLE
        if (position.y < groundLevel - 5.0f)
        {
            currentState = JUMP;
            isJumping = true;
        }
        else
        {
            currentState = IDLE;
        }
    }
    
}

void Player::HandleUlt(float deltaTime)
{
    
    ultTimer -= deltaTime;
    if (ultTimer <= 0)
    {
        isUlt = false;
       
        SetState(IDLE);  // **ULT 结束后回到 IDLE**
    }

   
}
void Player::HandleHurt(float deltaTime)
{
    std::cout << "currentState = " << currentState << ", isInvincible = " << isInvincible << ", hurtTimer = " << hurtTimer << "\n";

    position.x += knockbackVelocityX * deltaTime;
    hurtTimer -= deltaTime;
    invincibilityTimer -= deltaTime;

    if (hurtTimer <= 0.0f)
    {
        knockbackVelocityX = 0.0f;
    }

    if (invincibilityTimer <= 0.0f)
    {
        isInvincible = false;

        if (hp <= 0)
        {
            SetState(FALL);
           
        }
        else if (position.y < groundLevel - 5.0f)
            SetState(JUMP);
        else
            SetState(IDLE);
    }
}

void Player::HandleFall(float deltaTime)
{
    
    
    if (position.y < groundLevel)
    {
        velocityY += gravity * deltaTime;
        position.y += velocityY * deltaTime;

        // 防止穿透地面
        if (position.y >= groundLevel)
        {
            position.y = groundLevel;
            velocityY = 0.0f;
        }
    }
    else
    {
        position.y = groundLevel;
        velocityY = 0.0f;
    }
    if (!isDown && fallAnim.frame >= 5)  // 0 ~ 5 共6帧
    {
        isDown = true;
    }
}
void Player::Damage()
{
    if (isInvincible || hp <= 0) return;

    hp--;
    isInvincible = true;
    invincibilityTimer = 0.6f;
    SetState(HURT);
}
	

int Player::GetHP() const
{
    return hp;
}



// **更新游戏逻辑**
void Player::Update(float deltaTime)
{

    if (currentState == FALL)
    {
        if (!isDown)
        {
            fallTimer -= deltaTime;
            HandleFall(deltaTime);
            updateAnimData(fallAnim, deltaTime, 6);
          
		}
        return; // 💡 动画播完前不能执行任何其他逻辑
    }
    if (currentState != HURT && currentState != FALL) {
        HandleInput(); // 先检测输入，获取 `nextState`

    if (currentState != nextState)
    
        SetState(nextState);  // **切换状态**
    }

    switch (currentState)
    {
    case IDLE:
        HandleIdle(deltaTime);
        break;
    case WALK:
        HandleWalk(deltaTime);
        break;
    case JUMP:
        HandleJump(deltaTime);
        break;
    case DASH:
        HandleDash(deltaTime);
        break;
    case ATK:
        HandleAttack(deltaTime);
        break;
    case ULT:
        HandleUlt(deltaTime);
        break;
    case HURT:
        HandleHurt(deltaTime);
        break;
	case FALL:
        HandleFall(deltaTime);
        break;
    }

    // **更新动画**
    switch (currentState)
    {
    case IDLE:
        updateAnimData(idleAnim, deltaTime, 4);
        break;
    case WALK:
        updateAnimData(walkAnim, deltaTime, 7);
        break;
    case JUMP:
        updateAnimData(jumpAnim, deltaTime, 6);
        break;
    case DASH:
        updateAnimData(dashAnim, deltaTime, 4);
        break;
    case ATK:
        updateAnimData(atkAnim, deltaTime, 6);
        break;
    case ULT:
        updateAnimData(ultAnim, deltaTime, 10);
        break;
    case HURT:
        updateAnimData(hurtAnim, deltaTime, 4);  
        break;
    case FALL: 
        updateAnimData(fallAnim, deltaTime, 6);
        break;
    }
    
    
}

// **修正 SetState 逻辑**
void Player::SetState(PlayerState newState)
{
    if (currentState == FALL && isDown)
        return;
    if (currentState != newState)
    {
        currentState = newState;

        switch (newState)
        {
        case IDLE:
            idleAnim.frame = 0;
            idleAnim.runningTime = 0.0f;
            isDashing = false;  // **确保 DASH 停止**
            break;
        case WALK:
            walkAnim.frame = 0;
            walkAnim.runningTime = 0.0f;
            break;
        case JUMP:
            jumpAnim.frame = 0;
            jumpAnim.runningTime = 0.0f;
            isJumping = true;
            velocityY = jumpForce;  // **确保 JUMP 触发跳跃**
            break;
        case DASH:
            dashAnim.frame = 0;
            dashAnim.runningTime = 0.0f;
            isDashing = true;
            dashTimer = 0.2f;  // **确保 DASH 维持 `dashTime`**
            break;
        case ATK:
            atkAnim.frame = 0;
            atkAnim.runningTime = 0.0f;
            atkTimer = 0.4f;  // **攻击持续 0.5 秒**
            break;
        case ULT:
            ultAnim.frame = 0;
            ultAnim.runningTime = 0.0f;
            isUlt = true;
            ultTimer = 2.0f;  // **ULT 持续 2 秒**
            break;
        case HURT:
            hurtAnim.frame = 0;
            hurtAnim.runningTime = 0.0f;
            hurtTimer = 0.3f;  // 播完完整动画（3~4帧）
            knockbackVelocityX = -120.0f; //  设置 knockback 速度
            break;

        case FALL:
            fallAnim.frame = 0;
            fallAnim.runningTime = 0.0f;
            fallTimer = 0.3f;
            nextState = FALL;         // 锁住状态
            break;


        }
    }
}

Vector2 Player::GetPosition() const
{
    return position;
}
PlayerState Player::GetState() const  {
    return currentState;
}

bool Player::GetIsDown() const
{
    return isDown;
}

// **绘制角色**
void Player::Draw()
{
    Texture2D currentTexture;
    AnimData currentAnim;

    switch (currentState)
    {
    case WALK:
        currentTexture = walkTexture;
        currentAnim = walkAnim;
        break;
    case JUMP:
        currentTexture = jumpTexture;
        currentAnim = jumpAnim;
        break;
    case DASH:
        currentTexture = dashTexture;
        currentAnim = dashAnim;
        break; 
    case ATK:
        currentTexture = atkTexture;
        currentAnim = atkAnim;
        break;
    case ULT:
        currentTexture = ultTexture;
        currentAnim = ultAnim;
        break;
    case HURT:
        currentTexture = hurtTexture;
        currentAnim = hurtAnim;
        break;

    case FALL:
        currentTexture = fallTexture;
        currentAnim = fallAnim;
        break;
    default:
        currentTexture = idleTexture;
        currentAnim = idleAnim;
        break;
    }

    DrawTexturePro(
        currentTexture,
        currentAnim.rec,
        { position.x, position.y, currentAnim.rec.width, currentAnim.rec.height },
        { 0, 0 }, 0, WHITE);
}

// **释放纹理**
Player::~Player()
{
    if (idleTexture.id != 0) UnloadTexture(idleTexture);
    if (walkTexture.id != 0) UnloadTexture(walkTexture);
    if (jumpTexture.id != 0) UnloadTexture(jumpTexture);
    if (dashTexture.id != 0) UnloadTexture(dashTexture);
    if (atkTexture.id != 0) UnloadTexture(atkTexture);
    if (ultTexture.id != 0) UnloadTexture(ultTexture);
    if (hurtTexture.id != 0) UnloadTexture(hurtTexture);
    if (fallTexture.id != 0) UnloadTexture(fallTexture);

    UnloadSound(attackSound);
    //UnloadSound(walkSound);
}

// **更新动画数据**
void Player::updateAnimData(AnimData& data, float deltaTime, int maxFrame)
{
    if (currentState == FALL)
    {
        if (data.frame < maxFrame - 1)
        {
            data.runningTime += deltaTime;
            if (data.runningTime >= data.updateTime)
            {
                data.runningTime = 0.0f;
                data.frame++;
                data.rec.x = data.frame * data.rec.width;
                
            }
        }
        // 最后一帧后就不再更新动画
        return;
    }

    // 普通状态循环播放
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0f;
        data.frame = (data.frame + 1) % maxFrame;
        data.rec.x = data.frame * data.rec.width;
    }
}

//collision
Rectangle Player::GetCollisionRect() const
{
    AnimData currentAnim;
    switch (currentState)
    {
    case WALK: currentAnim = walkAnim; break;
    case JUMP: currentAnim = jumpAnim; break;
    case DASH: currentAnim = dashAnim; break;
    case ATK: currentAnim = atkAnim; break;
    case ULT: currentAnim = ultAnim; break;
    default: currentAnim = idleAnim; break;
    }

    return {
        position.x+20,
        position.y,
        currentAnim.rec.width*0.4f,
        currentAnim.rec.height
    };
}

Rectangle Player::GetAttackRect() const
{
    if (currentState != ATK) return { 0, 0, 0, 0 };

    return {
        position.x + GetCollisionRect().width,  // 从右侧伸出
        position.y + 10,
        50, 40
    };
}


