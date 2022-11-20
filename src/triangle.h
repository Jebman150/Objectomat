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
            float darkness = calculateColorOnTriangle(vertexTable);
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