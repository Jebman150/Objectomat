#pragma once

#include <cmath>
#include "meshVertex.h"     // -> triangle.h   common.h
#include "transform.h"      // -> common.h

class Mesh {
    public:
    int xRes;
    int yRes;
    int edgeAmount; //currently not changeable
    std::vector<sf::Vector3f> edges;
    std::vector<Triangle> triangles;
    std::vector<MeshVertex> vertices;
    sf::Color color;

    Transform transform;
    Mesh();
    Mesh(sf::Vector3f position, sf::Vector3f size, std::vector<sf::Vector3f> edgesInit, sf::Color colorInit = sf::Color::Green, int xResol = 10, int yResol = 10);

    void createMeshVertices(int xResol, int yResol, std::vector<sf::Vector3f> edgePos);
    void repositionVertices();
    void updateMesh();
    void firstLink();
    void rotateMesh_transform(sf::Vector3f center = {0, 0, 0});
    void rotateTransform_center();
    sf::VertexArray createMeshOnScreen(sf::VertexArray &input);
};