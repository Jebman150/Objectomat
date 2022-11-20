#pragma once

#include <SFML\Graphics.hpp>
#include <math.h>
#include <iostream>
#include "common.h"

class Triangle {
    private:
        const int vertexNum = 3;
        std::vector<sf::Vector3f> vertexTable;
        bool isVisible = true;
        sf::Color color;
    public:
        //constructor:
        Triangle() {
            vertexTable.resize(vertexNum);
            color = sf::Color::Green;
        }

        //methods:

        void recreateTriangle(std::vector<sf::Vector3f> vertices) {
            vertexTable = vertices;

        }

        sf::Vertex* createTriangleOnScreen(sf::Vertex* input) {
            for(int i = 0; i < vertexNum; i++) {
                input[i] = projectOnScreen(vertexTable[i]);
                input[i].color = color;
            }
            float darkness = calculateColorOnTriangle();
            if(darkness > 1)  isVisible = false;
            else isVisible = true;
            if(!isVisible)  return NULL;

            for(int vertex = 0; vertex < 3; vertex++) {
                input[vertex].color.r *= 1 - .8f * darkness;
                input[vertex].color.g *= 1 - .8f * darkness;
                input[vertex].color.b *= 1 - .8f * darkness;
                input[vertex].color.a = 255;
            }
            return input;
        }

        void rotateTriangle(std::vector<sf::Vector3f> &rotor) {
            for(int i = 0; i < vertexNum; i++) {
                rotate(vertexTable[i], rotor);
            }
        }

        sf::Vector3f calcNormal() {
            sf::Vector3f normal;
            normal.x = vertexTable[0].y*vertexTable[1].z - vertexTable[0].z*vertexTable[1].y;
            normal.y = vertexTable[0].z*vertexTable[1].x - vertexTable[0].x*vertexTable[1].z;
            normal.z = vertexTable[0].x*vertexTable[1].y - vertexTable[0].y*vertexTable[1].x;

            normal = normalize(normal);
        }

        float calculateColorOnTriangle() {
            sf::Vector3f normal = calcNormal();
            //Centroid of triangle

            sf::Vector3f centreOfTriangle;
            sf::Vector3f s = {0, 0, 0};
            for(int i = 0; i < 3; i++) {
                s += vertexTable[i];
            }
            centreOfTriangle.x = s.x/3;
            centreOfTriangle.y = s.y/3;
            centreOfTriangle.z = s.z/3;

            //Check visibility:

            sf::Vector3f vectorToCamera;
            vectorToCamera.x = -centreOfTriangle.x;
            vectorToCamera.y = -centreOfTriangle.y;
            vectorToCamera.z = -centreOfTriangle.z -focalLength -(WINDOW_SIZE/2);

            vectorToCamera = normalize(vectorToCamera);

            double angleToCam = std::acos(
                (normal.x * vectorToCamera.x + normal.y * vectorToCamera.y + normal.z * vectorToCamera.z) /
                (std::pow(std::pow(normal.x, 2) + std::pow(normal.y, 2) + std::pow(normal.z, 2), 1.f/2.f) * 
                std::pow(std::pow(vectorToCamera.x, 2) + std::pow(vectorToCamera.y, 2) + std::pow(vectorToCamera.z, 2), 1.f/2.f))
            ) * 180/3.141592;

            //Calculate light level:
            sf::Vector3f vectorToLight;
            vectorToLight = lightSource - centreOfTriangle;
            vectorToLight = normalize(vectorToLight);
            double angleToLight = std::acos(
                (normal.x * vectorToLight.x + normal.y * vectorToLight.y + normal.z * vectorToLight.z) /
                (std::pow(std::pow(normal.x, 2) + std::pow(normal.y, 2) + std::pow(normal.z, 2), 1.f/2.f) * 
                std::pow(std::pow(vectorToLight.x, 2) + std::pow(vectorToLight.y, 2) + std::pow(vectorToLight.z, 2), 1.f/2.f))
            ) * 180/3.141592;

            if(angleToCam >= 90) {
                return 2;
            }

            return angleToLight/180.f;
        }

        //getter, setter:
        std::vector<sf::Vector3f> getVertexTable() {
            return vertexTable;
        }

        void setVertexTable(std::vector<sf::Vector3f> vertices) {
            vertexTable = vertices;
        }

        void setColor(sf::Color colorInit) {
            color = colorInit;
        }
};