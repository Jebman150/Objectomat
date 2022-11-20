#pragma once

#include "triangle.h"   // -> common.h

class MeshVertex {
    public:
    sf::Vector3f position;
    std::vector<Triangle*> adjacentTriangles;

    MeshVertex();
    MeshVertex(std::vector<Triangle> &adjacents, int numAdj, sf::Vector3f vertexInit);

    void addTriangle(Triangle &adjacent);
};