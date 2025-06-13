#include "EnemyManager.h"
#include <raylib.h>
#include <iostream>

void EnemyManager::Init()
{
    enemyTexture = LoadTexture("textures/slime.png");
    if (enemyTexture.id == 0) {
        std::cerr << "[EnemyManager]Failed to load slime.png\n";
        return;  
    }

	
    enemies.clear();
    enemies.push_back(Enemy({ 600, 400 }, &enemyTexture));
    enemies.push_back(Enemy({ 1000, 360 }, &enemyTexture));
    enemies.push_back(Enemy({ 1400, 390 }, &enemyTexture));
    enemies.push_back(Enemy({ 600, 420 }, &enemyTexture));
    enemies.push_back(Enemy({ 600, 380 }, &enemyTexture));
    enemies.push_back(Enemy({ 600, 360 }, &enemyTexture));
    enemies.push_back(Enemy({ 800, 460 }, &enemyTexture));
    enemies.push_back(Enemy({ 820, 460 }, &enemyTexture));
    enemies.push_back(Enemy({ 840, 460 }, &enemyTexture));
    enemies.push_back(Enemy({ 860, 460 }, &enemyTexture));
    enemies.push_back(Enemy({ 880, 460 }, &enemyTexture));
    enemies.push_back(Enemy({ 900, 460 }, &enemyTexture));
    enemies.push_back(Enemy({ 1000, 360 }, &enemyTexture));
    enemies.push_back(Enemy({ 1100, 340 }, &enemyTexture));
    enemies.push_back(Enemy({ 1400, 390 }, &enemyTexture));
}

void EnemyManager::Update(float deltaTime)
{
    for (auto& enemy : enemies)
    {
        if (!enemy.IsAlive()) continue;

        // ?人自己的行???，例如巡?、移?、?画更新
        enemy.Update(deltaTime);
    }
}

std::vector<Enemy>& EnemyManager::GetEnemies()
{

    return enemies;

}

void EnemyManager::Draw()
{
    for (auto& enemy : enemies)
    {
        enemy.Draw();
    }
}

void EnemyManager::Reset()
{
    if (enemyTexture.id == 0) return;
    enemies.clear();
    enemies.push_back(Enemy({ 600, 400 }, &enemyTexture));
    enemies.push_back(Enemy({ 1000, 360 }, &enemyTexture));
    enemies.push_back(Enemy({ 1400, 390 }, &enemyTexture));
    enemies.push_back(Enemy({ 600, 420 }, &enemyTexture));
    enemies.push_back(Enemy({ 600, 380 }, &enemyTexture));
    enemies.push_back(Enemy({ 600, 360 }, &enemyTexture));
    enemies.push_back(Enemy({ 800, 460 }, &enemyTexture));
    enemies.push_back(Enemy({ 820, 460 }, &enemyTexture));
    enemies.push_back(Enemy({ 840, 460 }, &enemyTexture));
    enemies.push_back(Enemy({ 860, 460 }, &enemyTexture));
    enemies.push_back(Enemy({ 880, 460 }, &enemyTexture));
    enemies.push_back(Enemy({ 900, 460 }, &enemyTexture));
    enemies.push_back(Enemy({ 1000, 360 }, &enemyTexture));
    enemies.push_back(Enemy({ 1100, 340 }, &enemyTexture));
    enemies.push_back(Enemy({ 1400, 390 }, &enemyTexture));
 
}

void EnemyManager::Unload()
{
    UnloadTexture(enemyTexture);
	
}