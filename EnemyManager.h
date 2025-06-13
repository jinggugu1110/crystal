#pragma once

#include "Enemy.h"
#include "Player.h"
#include <vector>


class EnemyManager {
private:
    std::vector<Enemy> enemies;

	Texture2D enemyTexture;
	

public:
    void Init();
    void Update(float deltaTime);
    std::vector<Enemy>& GetEnemies();
    void Draw();
    void Reset();
    void Unload();
};