#include "mesh.h"

/*******Constructors:**********/

Mesh::Mesh() {      //creates standard mesh
    transform.position = {0, -100, 0};
    transform.size = {1, 1, 1};
    color = sf::Color::Green;
    edgeAmount = 4;
    edges.resize(edgeAmount);
    edges = {
        {-100, 0, -100},
        {-100, 0,  100},
        { 100, 0,  100},
        { 100, 0, -100}
    };
    for(int i = 0; i < edges.size(); i++) {
        edges[i].x *= transform.size.x;
        edges[i].y *= transform.size.y;
        edges[i].z *= transform.size.z;
    }
    triangles.resize(2);
    xRes = 10;
    yRes = 10;
    updateMesh();
    firstLink();
}

Mesh::Mesh(sf::Vector3f position, sf::Vector3f size, std::vector<sf::Vector3f> edgesInit, sf::Color colorInit, int xResol, int yResol) {
    transform.position = position;
    transform.size = size;
    color = colorInit;
    edgeAmount = 4;
    edges.resize(edgeAmount);
    edges = edgesInit;
    for(int i = 0; i < edges.size(); i++) {
        edges[i].x *= transform.size.x;
        edges[i].y *= transform.size.y;
        edges[i].z *= transform.size.z;
    }
    triangles.resize(2);
    xRes = xResol;
    yRes = yResol;
    updateMesh();
    firstLink();
}


/******FUNCTIONS********/

void Mesh::createMeshVertices(int xResol, int yResol, std::vector<sf::Vector3f> edgePos) {
    xRes = xResol;
    yRes = yResol;
    edges = edgePos;

    int totalVertexNum = (xRes+1) * (yRes+1);
    vertices.resize(totalVertexNum);

    for(MeshVertex &v : vertices) {
        v = MeshVertex();
    }

    int totalTriangleNum = xRes*yRes*2;
    triangles.resize(totalTriangleNum);
    repositionVertices();
}

void Mesh::repositionVertices() {
    for(int y = 0; y < yRes+1; y++) {
        //generate a line between the interpolated points on opposite borders
        sf::Vector3f yCoordOne;
        yCoordOne.x = lerp(edges[0].x, edges[1].x, float(y)/yRes);
        yCoordOne.y = lerp(edges[0].y, edges[1].y, float(y)/yRes);
        yCoordOne.z = lerp(edges[0].z, edges[1].z, float(y)/yRes);
        sf::Vector3f yCoordTwo;
        yCoordTwo.x = lerp(edges[3].x, edges[2].x, float(y)/yRes);
        yCoordTwo.y = lerp(edges[3].y, edges[2].y, float(y)/yRes);
        yCoordTwo.z = lerp(edges[3].z, edges[2].z, float(y)/yRes);
        //interpolate all points on that one line in order to create a mesh
        for(int x = 0; x < xRes+1; x++) {
            vertices[y*(xRes+1)+x].position.x = lerp(yCoordOne.x, yCoordTwo.x, float(x)/xRes);
            vertices[y*(xRes+1)+x].position.y = lerp(yCoordOne.y, yCoordTwo.y, float(x)/xRes);
            vertices[y*(xRes+1)+x].position.z = lerp(yCoordOne.z, yCoordTwo.z, float(x)/xRes);
        }
    }
}

void Mesh::updateMesh() {
    createMeshVertices(xRes, yRes, edges);
    for(int y = 0; y < yRes; y++) {
        for(int x = 0; x < xRes; x++) {
            std::vector<sf::Vector3f> triangleCoords;
            triangleCoords.resize(3);

            triangleCoords[0] = transform.position + vertices[y*(xRes+1)+x+1].position;
            triangleCoords[1] = transform.position + vertices[y*(xRes+1)+x].position;
            triangleCoords[2] = transform.position + vertices[(y+1)*(xRes+1)+x].position;

            triangles[(y*xRes+x)*2].recreateTriangle(triangleCoords);
            triangles[(y*xRes+x)*2].setColor(color);

            triangleCoords[0] = transform.position + vertices[y*(xRes+1)+x+1].position;
            triangleCoords[1] = transform.position + vertices[(y+1)*(xRes+1)+x].position;
            triangleCoords[2] = transform.position + vertices[(y+1)*(xRes+1)+x+1].position;

            triangles[(y*xRes+x)*2+1].recreateTriangle(triangleCoords);
            triangles[(y*xRes+x)*2+1].setColor(color);
        }
    }
}

void Mesh::firstLink() {
    for(int y = 0; y < yRes; y++) {
        for(int x = 0; x < xRes; x++) {
            vertices[y*(xRes+1)+x+1].addTriangle(triangles[(y*xRes+x)*2]);
            vertices[y*(xRes+1)+x].addTriangle(triangles[(y*xRes+x)*2]);
            vertices[(y+1)*(xRes+1)+x].addTriangle(triangles[(y*xRes+x)*2]);

            vertices[y*(xRes+1)+x+1].addTriangle(triangles[(y*xRes+x)*2+1]);
            vertices[(y+1)*(xRes+1)+x].addTriangle(triangles[(y*xRes+x)*2+1]);
            vertices[(y+1)*(xRes+1)+x+1].addTriangle(triangles[(y*xRes+x)*2+1]);
        }
    }
}

void Mesh::rotateMesh_transform(sf::Vector3f center) {
    std::vector<sf::Vector3f> edgesRel;
    edgesRel.resize(edges.size());
    for(int i = 0; i < edges.size(); i++) {
        edgesRel[i] = edges[i] + center;
    }
    for(int i = 0; i < edgesRel.size(); i++) {
        rotate(edgesRel[i], bodyRotationTable);
    }
    for(int i = 0; i < edges.size(); i++) {
        edges[i] = edgesRel[i] - center;
    }
    updateMesh();
}

void Mesh::rotateTransform_center() {
    for(int i = 0; i < edges.size(); i++) {
        rotate(edges[i], camRotationTable);
    }
    rotate(transform.position, camRotationTable);
    updateMesh();
}

sf::VertexArray Mesh::createMeshOnScreen(sf::VertexArray &input) {
    input.resize(triangles.size()*3);
    for(int i = 0; i < triangles.size(); i++) {
        sf::Vertex temp[3];
        if(triangles[i].createTriangleOnScreen(temp) == NULL) {
            input[i*3] = temp[0];
            input[i*3+1] = temp[1];
            input[i*3+2] = temp[2];
            input[i*3].color = sf::Color(0, 0, 0, 0);
            input[i*3+1].color = sf::Color(0, 0, 0, 0);
            input[i*3+2].color = sf::Color(0, 0, 0, 0);
        } else {
            input[i*3] = temp[0];
            input[i*3+1] = temp[1];
            input[i*3+2] = temp[2];
        }
    }
    return input;
}