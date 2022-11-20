#include "bodyPresets.h"


/*******CONSTRUCTORS********/


Cube::Cube() {      //standard cube
    transform.position = {0, 0, 0};
    transform.size = {1, 1, 1};
    color = sf::Color::Green;
    xRes = 10;
    yRes = 10;
    sides.resize(6);
    setSides();
}

Cube::Cube(sf::Vector3f posInit, sf::Vector3f sizeInit, sf::Color colorInit, int xResol, int yResol) {
    transform.position = posInit;
    transform.size = sizeInit;
    color = colorInit;
    xRes = xResol;
    yRes = yResol;
    sides.resize(6);
    setSides();
}

/******SIDE-SETTER******/

void Cube::setSides() {
    float size = 63;
    sf::Vector3f offsetX = {size * transform.size.x, 0, 0};
    sf::Vector3f offsetY = {0, size * transform.size.y, 0};
    sf::Vector3f offsetZ = {0, 0, size * transform.size.z};
    sides[0] = Mesh(
        transform.position + offsetY,
        transform.size, 
        {
            {-size, 0, -size},
            {-size, 0,  size},
            { size, 0,  size},
            { size, 0, -size}
        },
        color,
        xRes,
        yRes);

    sides[1] = Mesh(
        transform.position - offsetY,
        transform.size, 
        {
            { size, 0,  size},
            {-size, 0,  size},
            {-size, 0, -size},
            { size, 0, -size}
        },
        color,
        xRes,
        yRes);

    sides[2] = Mesh(
        transform.position + offsetX,
        transform.size, 
        {
            {0,  size,  size},
            {0, -size,  size},
            {0, -size, -size},
            {0,  size, -size}
        },
        color,
        xRes,
        yRes);

    sides[3] = Mesh(
        transform.position - offsetX,
        transform.size, 
        {
            {0, -size, -size},
            {0, -size,  size},
            {0,  size,  size},
            {0,  size, -size}
        },
        color,
        xRes,
        yRes);

    sides[4] = Mesh(
        transform.position + offsetZ,
        transform.size, 
        {
            { size,  size, 0},
            {-size,  size, 0},
            {-size, -size, 0},
            { size, -size, 0}
        },
        color,
        xRes,
        yRes);

    sides[5] = Mesh(
        transform.position - offsetZ,
        transform.size, 
        {
            {-size, -size, 0},
            {-size,  size, 0},
            { size,  size, 0},
            { size, -size, 0}
        },
        color,
        xRes,
        yRes);
}

/*******RENDERER*******/

sf::VertexArray Cube::createMeshOnScreen(sf::VertexArray &input) {
    input.resize(sides.size()*xRes*yRes*6);
    for(int i = 0; i < sides.size(); i++) {
        sf::VertexArray temp;
        sides[i].createMeshOnScreen(temp);
        for(int j = 0; j < xRes*yRes*6; j++) {
            input[i*(xRes*yRes*6)+j] = temp[j];
        }
    }
    return input;
}

/****OTHER********/

void Cube::calculateMesh(bool &rotateCam, bool rotateBody) {
    if(rotateCam) {
        rotate(transform.position, camRotationTable);
        for(int i = 0; i < sides.size(); i++) {
            sides[i].rotateTransform_center();
        }
    }
    if(!rotateBody) return;

    for(int i = 0; i < sides.size(); i++) {
        sf::Vector3f offsetVec;
        offsetVec = sides[i].transform.position - transform.position;
        sides[i].rotateMesh_transform(offsetVec);
    }
}