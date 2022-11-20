#pragma once

#include "common.h"

class Transform {
    public:
    sf::Vector3f position;   // x, y, z
    sf::Vector3f size;       // x, y, z multiplier

    Transform() {
        position = {0, 0, 0};
        size = {1, 1, 1};
    }
};