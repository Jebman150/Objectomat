#ifndef ANT_H
#define ANT_H

#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <array>

using namespace sf;

extern int frame;

float inRadians(float degrees);
float inDegrees(float radians);
float clamp(float max, float min, float val);
Vector2f angleToVector(float angle);
float vectorToAngle(sf::Vector2f vec);
float angleBetweenVectors(Vector2f a, Vector2f b);
float angleBetweenVectors_signed(Vector2f a, Vector2f b);
float dist(Vector2f a, Vector2f b);

enum State {
    moveForward,
    sleep,
    turnLeftAndMove,
    turnRightAndMove,
    END_OF_ACTIONS,
    accelerate_mForward,
    decelerate_mForward
};

enum Target {
    home,
    food,
    exploring,
    END
};

struct Home {           //colony base
    Vector2f position;
    CircleShape shape;
    int storedFood;
    //int colonyIndex;  WIP
    //int antAmount;    WIP
};

struct FoodSpot {
    Vector2f position;
    CircleShape shape;
    int storedFood;
};

struct PheromoneBlob {
    Vector2f position;
    CircleShape shape;
    Target type;
    float size;
}; 
/*Every 15 frames, an ant drops a pheromone blob;
This is a point, defined by 
the type:   indicates, whether it leads to a food-source or the base
the size:   the bigger the size, the further away the attracted ant can be
size will decrease over time until the blob disappears
towards which the other ants will go*/

class Ant {
public: 
    Sprite body;
    Vector2f position;
    State currentState;
    Target currentTarget;
    float pointingDirection;        //angel (0° = right)
    float speed;                    //pixel per frame
    float turningSpeed;             //degrees per frame
    float scale;
    //int timeOfLastDrop;           //not used yet
    int carringFood;

    Ant() {
        pointingDirection = rand() % 360;
        currentState = State::moveForward;
        currentTarget = Target::food;
        scale = .1;
        speed = 4;
        turningSpeed = 5;
        carringFood = 0;
    }

    void moveFw() {
        Vector2f pointingVector = angleToVector(pointingDirection);
        position += pointingVector * speed;
    }
    
    void sleep() {
        //do nothing
    }

    void turnRight() {
        Vector2f pointingVector = angleToVector(pointingDirection);
        pointingDirection -= turningSpeed;
        position += pointingVector * (speed / 2);
    }

    void turnLeft() {
        Vector2f pointingVector = angleToVector(pointingDirection);
        pointingDirection += turningSpeed;
        position += pointingVector * (speed / 2);
    }

    void dropPheromone(PheromoneBlob &blob) {
        blob.position = position;
        blob.size = 3;
        blob.shape.setRadius(blob.size);
        if(currentTarget == Target::home) {
            blob.type = Target::food;
            blob.shape.setFillColor(Color::Green);
        } else {
            blob.type = Target::home;
            blob.shape.setFillColor(Color::Blue);
        }
        blob.shape.setPosition(blob.position);
    }
};

#endif