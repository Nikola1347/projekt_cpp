#include "timesystem.hpp"

void TimeSystem::update(float dt) {

    timeAccumulator += dt;

    // 1 s = 1 m w grze
    while (timeAccumulator >= 1.f) {
        timeAccumulator -= 1.f;

        minute++;

        if (minute >= 60) {
            minute = 0;
            hour++;
        }

        if (hour >= 24) {
            hour = 0;
            day++;
        }
    }
}