#include "ant.h"
#include "actions.h"
#include <array>
#include <iostream>
#include <math.h>

Ant doAction(int action, Ant &ant) {
    Vector2f pointingVector = angleToVector(ant.pointingDirection);
    switch(action) {
        case State::moveForward:
            ant.moveFw();
            return ant;
        case State::sleep:
            ant.sleep();
            return ant;
        case State::turnRightAndMove:
            ant.turnRight();
            return ant;
        case State::turnLeftAndMove:
            ant.turnLeft();
            return ant;
        default:
            return ant;
    }
}

void dropPheromone(Ant &ant, PheromoneBlob &blob) {
    blob.position = ant.position;
    blob.size = 3;
    blob.shape.setRadius(blob.size);
    if(ant.currentTarget == Target::home) {
        blob.type = Target::food;
        blob.shape.setFillColor(Color::Green);
    } else {
        blob.type = Target::home;
        blob.shape.setFillColor(Color::Blue);
    }
    blob.shape.setPosition(blob.position);
}