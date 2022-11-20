#pragma once

#include "mesh.h"   // -> meshVertex.h     transform.h     triangle.h      common.h

class Cube {
public:
    Transform transform;
    sf::Color color;
    std::vector<Mesh> sides;
    int xRes;
    int yRes;

    Cube();
    Cube(sf::Vector3f posInit, sf::Vector3f sizeInit, sf::Color colorInit = sf::Color::Green, int xResol = 10, int yResol = 10);

    void setSides();
    void calculateMesh(bool &rotateCam, bool rotateBody);
    sf::VertexArray createMeshOnScreen(sf::VertexArray &input);
};