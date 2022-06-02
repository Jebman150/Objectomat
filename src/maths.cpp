#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <math.h>
#include "ant.h"

#define PI 3.141592568979

float inRadians(float degrees) {
    return degrees * PI / 180;
}

float inDegrees(float radians) {
    return (180 / PI) * radians;
}

float clamp(float max, float min, float val) {
    if(val > max) {
        val = max;
    } else if(val < min) {
        val = min;
    }
    return val;
}

sf::Vector2f angleToVector(float angle) {
    sf::Vector2f directionVector;
    directionVector.x = cos(inRadians(angle));
    directionVector.y = sin(inRadians(angle));
    return directionVector;
}

float vectorToAngle(sf::Vector2f vec) {
    float angle;
    angle = angleBetweenVectors(Vector2f(1, 0), vec);
    return angle;
}

float angleBetweenVectors(Vector2f a, Vector2f b) {
    Vector2f ortA = Vector2f(a.y, -a.x);
    float angle;
    float ortAngle;
    ortAngle = (ortA.x * b.x + ortA.y * b.y);
    ortAngle /= sqrt(ortA.x * ortA.x + ortA.y * ortA.y) * sqrt(b.x * b.x + b.y * b.y);
    ortAngle = acos(clamp(1, -1, ortAngle));
    ortAngle = inDegrees(ortAngle);

    if(ortAngle < 90) {
        a = -a;
        angle = (a.x * b.x + a.y * b.y);
        angle /= sqrt(a.x * a.x + a.y * a.y) * sqrt(b.x * b.x + b.y * b.y);
        angle = acos(clamp(1, -1, angle));
        angle = inDegrees(angle) + 180;
    } else {
        angle = (a.x * b.x + a.y * b.y);
        angle /= sqrt(a.x * a.x + a.y * a.y) * sqrt(b.x * b.x + b.y * b.y);
        angle = acos(clamp(1, -1, angle));
        angle = inDegrees(angle);
    }
    return angle;
}

float angleBetweenVectors_signed(Vector2f a, Vector2f b) {
    Vector2f ortA = Vector2f(a.y, -a.x);
    float angle;
    float ortAngle;
    ortAngle = (ortA.x * b.x + ortA.y * b.y);
    ortAngle /= sqrt(ortA.x * ortA.x + ortA.y * ortA.y) * sqrt(b.x * b.x + b.y * b.y);
    ortAngle = acos(clamp(1, -1, ortAngle));
    ortAngle = inDegrees(ortAngle);

    if(ortAngle < 90) {
        angle = (a.x * b.x + a.y * b.y);
        angle /= sqrt(a.x * a.x + a.y * a.y) * sqrt(b.x * b.x + b.y * b.y);
        angle = acos(clamp(1, -1, angle));
        angle = -inDegrees(angle);
    } else {
        angle = (a.x * b.x + a.y * b.y);
        angle /= sqrt(a.x * a.x + a.y * a.y) * sqrt(b.x * b.x + b.y * b.y);
        angle = acos(clamp(1, -1, angle));
        angle = inDegrees(angle);
    }
    return angle;
}

float dist(Vector2f a, Vector2f b) {
    return abs(sqrt(((a.x - b.x) * (a.x - b.x))
                + ((a.y - b.y) * (a.y - b.y))));
}