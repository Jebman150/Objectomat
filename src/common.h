#pragma once

#include <vector>
#include <SFML\System.hpp>

#define WINDOW_SIZE 1000

/**in fileReader.cpp**/
//int read_ShapeFile_Content();
//int read_StaticMesh_Content();

/**in mathFunc.cpp**/
void rotate(sf::Vector3f &coords, std::vector<sf::Vector3f> &rotor);
std::vector<sf::Vector3f> generateRotationMatrix(sf::Vector3f yaw_pitch_roll);
std::vector<sf::Vector3f> combine_3x3_Matrices(std::vector<sf::Vector3f> &m1, std::vector<sf::Vector3f> &m2);
sf::Vector2f projectOnScreen(sf::Vector3f positionInSpace);
float calculateColorOnTriangle(std::vector<sf::Vector3f> triangle);
sf::Vector3f normalize(sf::Vector3f input);
float lerp(float a, float b, float f);
void split(std::string str, std::string splitBy, std::vector<std::string>& tokens);

extern int focalLength;
extern double sizeMultiplier;

extern std::vector<sf::Vector3f> bodyRotationTable;
extern std::vector<sf::Vector3f> camRotationTable;
extern sf::Vector3f lightSource;