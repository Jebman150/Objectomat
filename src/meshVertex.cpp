#include "meshVertex.h"

MeshVertex::MeshVertex() {
    position = {0, 0, 0};
    adjacentTriangles.resize(0);
}

MeshVertex::MeshVertex(std::vector<Triangle> &adjacents, int numAdj, sf::Vector3f posInit) {
    position = posInit;
    adjacentTriangles.resize(numAdj);
    for(int i = 0; i < numAdj; i++) {
        adjacentTriangles[i] = &adjacents[i];
    }
}

void MeshVertex::addTriangle(Triangle &adjacent) {
    adjacentTriangles.resize(adjacentTriangles.size()+1);
    adjacentTriangles[adjacentTriangles.size()-1] = &adjacent;
}