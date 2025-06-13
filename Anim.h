#pragma once
#ifndef ANIMDATA_H
#define ANIMDATA_H

#include "raylib.h"

struct AnimData {
    Rectangle rec;
    int frame;
    float updateTime;
    float runningTime;
};

#endif
