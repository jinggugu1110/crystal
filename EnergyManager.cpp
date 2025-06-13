#include "EnergyManager.h"
#include <iostream>


void EnergyManager::Init()
{
    sharedEnergyTexture = LoadTexture("textures/crystal24.png");
    if (sharedEnergyTexture.id == 0) {
        std::cerr << "crystal24.png failed to load\n";
        return;
    }

    energyCollectSound = LoadSound("sound/energy.mp3");
    if (energyCollectSound.frameCount == 0) {
        std::cerr << "energy.mp3 failed to load\n";
        return;
    }

    energies.clear();
    const std::vector<Vector2> positions = {
        {300,400}, {500,300}, {850,350}, {1000,350}, {1200,400}, {1250,300},
        {1400,320}, {1480,395}, {1520,380}, {1600,305}, {1670,388},
        {1700,360}, {1700,400}, {1740,420}, {1805,380}
    };
   

    for (const auto& pos : positions)
    {
        energies.emplace_back(pos, &sharedEnergyTexture);
    }
}

void EnergyManager::Update(float deltaTime, Player* player)
{
    for (auto& energy : energies)
    {
        energy.Update(deltaTime);
       
        if (energy.isActive && CheckCollisionRecs(
            { player->GetPosition().x, player->GetPosition().y, 64, 64 },
            { energy.position.x+5, energy.position.y, energy.energyAnim.rec.width*0.6f, energy.energyAnim.rec.height}))
        {
            
            PlaySound(energyCollectSound);
            //particles
            for (int i = 0; i < 20; i++) {
                Particle p;
                p.pos.x = energy.position.x;
                p.pos.y = energy.position.y+10;
                float angle = GetRandomValue(0, 360) * DEG2RAD;
                float speed = GetRandomValue(50, 150);
                p.vel = { cosf(angle) * speed, sinf(angle) * speed };
                p.life = 0.5f + GetRandomValue(0, 20) / 100.0f;
                p.color = PURPLE;
                particles.push_back(p);
            }
            energy.Deactivate();
          
            energyCount++;

            if (energyCount >= 10)
            {
                player->SetState(ULT);
            }
        }
    }
    for (auto it = particles.begin(); it != particles.end(); )
    {
        it->pos.x += it->vel.x * deltaTime;
        it->pos.y += it->vel.y * deltaTime;
        it->life -= deltaTime;

        if (it->life <= 0)
            it = particles.erase(it);
        else
            ++it;
    }
}

int EnergyManager::Getscore() const
{
    return energyCount;
}

void EnergyManager::Draw()
{
    for (auto& energy : energies)
    {
        energy.Draw();
    }
    for (const auto& p : particles) {
        DrawCircleV(p.pos, 1, Fade(p.color, p.life * 3.0f));  // life 越小越透明
    }
   
    
}

void EnergyManager::Reset()
{
    energyCount = 0;
    Init();
    std::cout << "[EnergyManager] Reset complete.\n";
}

void EnergyManager::Unload()
{
    UnloadTexture(sharedEnergyTexture);
	UnloadSound(energyCollectSound);
}
