#pragma once
#include <SFML/System.hpp>

struct TimeSystem {
    float timeAccumulator = 0.f;
    int minute = 0;
    int hour = 6;
    int day = 1;

    void update(float dt);
};