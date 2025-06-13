#pragma once

#include <vector>
#include "Energy.h"
#include "Player.h"
#include "raylib.h"

struct Particle {
	Vector2 pos;  // Position in pixels
	Vector2 vel;  // Velocity in pixels per second
	float life;   // Life in seconds
	Color color;  // Color of the particle
};

class EnergyManager
{
public:
    std::vector<Energy> energies;
    std::vector<Particle> particles;
    Texture2D sharedEnergyTexture;
    int energyCount = 0;
	Sound energyCollectSound;

    void Init();
    void Update(float deltaTime, Player* player);
    int Getscore() const;
    void Draw();
    void Reset();
    void Unload();
};
