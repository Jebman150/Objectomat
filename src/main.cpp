#include "bodyPresets.h"    // -> mesh.h   meshVertex.h     transform.h     triangle.h      common.h

using namespace std;

void updateRotationTable(sf::Vector2i mouseMovement, std::vector<sf::Vector3f> &rotor);

sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Objectomat");
sf::Font font;

Mesh surfaceMesh;
Cube cube;
Cube test;

sf::Vector2i lastMousePos;
sf::Vector2i mouseMovement;

int focalLength = 128;
int bodyAmount = 5;
double sizeMultiplier = 10;
int selectedBody = 0;
float yaw = 0;       //Z-Axis
float pitch = .01;     //Y-Axis
float roll = 0;      //X-Axis

bool spacebarPressed = false;
bool enterPressed = false;

int rotatingObject;
const int objectNum = 3;

bool hasFocus = true;

sf::Vector3f lightSource = {
    1000, -1000, 1000
};
std::vector<sf::Vector3f> bodyRotationTable = {
    {std::cos(pitch) * std::cos(yaw), std::cos(yaw) * std::sin(pitch) * std::sin(roll) - std::cos(roll) * std::sin(yaw), std::cos(yaw) * std::sin(pitch) * std::cos(roll) + std::sin(yaw) * std::sin(roll)},
    {std::cos(pitch) * std::sin(yaw), std::sin(yaw) * std::sin(pitch) * std::sin(roll) + std::cos(roll) * std::cos(yaw), std::sin(yaw) * std::sin(pitch) * std::cos(roll) - std::cos(yaw) * std::sin(roll)},
    {         -std::sin(pitch)      ,                            std::sin(roll) * std::cos(pitch)                      ,                              std::cos(roll) * std::cos(pitch)                    }
};  //rotates around transform

std::vector<sf::Vector3f> camRotationTable = {
    {std::cos(pitch) * std::cos(yaw), std::cos(yaw) * std::sin(pitch) * std::sin(roll) - std::cos(roll) * std::sin(yaw), std::cos(yaw) * std::sin(pitch) * std::cos(roll) + std::sin(yaw) * std::sin(roll)},
    {std::cos(pitch) * std::sin(yaw), std::sin(yaw) * std::sin(pitch) * std::sin(roll) + std::cos(roll) * std::cos(yaw), std::sin(yaw) * std::sin(pitch) * std::cos(roll) - std::cos(yaw) * std::sin(roll)},
    {         -std::sin(pitch)      ,                            std::sin(roll) * std::cos(pitch)                      ,                              std::cos(roll) * std::cos(pitch)                    }
};  //rotates transform around center

int main() {
    surfaceMesh = Mesh({0, 100, 0},
                    {3, 3, 3},
                    {
                        {-100, 0, -100},
                        { 100, 0, -100},
                        { 100, 0,  100},
                        {-100, 0,  100}
                    },
                    sf::Color(150, 150, 150));

    test = Cube({100, 100, 0}, {1, 2, 1});

    //cube = Cube({0, 0, 0}, {1, 1, 1}, sf::Color::Red);

    //if(read_ShapeFile_Content() == EXIT_FAILURE) {
    //    cout << "Failed reading from shapeFile!" << endl;
    //    return EXIT_SUCCESS;
    //}
    
    if(!font.loadFromFile("Cabin-Regular.ttf")) {
        cout << "Exiting..." << endl;
        return EXIT_FAILURE;
    }

    window.setFramerateLimit(60);

    while(window.isOpen()) {
        bool rotateCam;

        sf::Event event;
        while(window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    window.setSize(sf::Vector2u(WINDOW_SIZE, WINDOW_SIZE));   //lock the size of the window
                    break;
                case sf::Event::LostFocus:
                    hasFocus = false;
                    break;
                case sf::Event::GainedFocus:
                    hasFocus = true;
                    break;
            }
        }

        if(!hasFocus) continue;

        ///////////User Input/////////////
        
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            mouseMovement = sf::Mouse::getPosition(window) - lastMousePos;
            lastMousePos = sf::Mouse::getPosition(window);
            if(mouseMovement.x != 0 || mouseMovement.y != 0) {
                updateRotationTable(mouseMovement, bodyRotationTable);
            } else {
                bodyRotationTable = {
                    {1, 0, 0},
                    {0, 1, 0},
                    {0, 0, 1}
                };
            }
            rotateCam = false;
        } else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            mouseMovement = sf::Mouse::getPosition(window) - lastMousePos;
            lastMousePos = sf::Mouse::getPosition(window);
            if(mouseMovement.x != 0 || mouseMovement.y != 0) {
                updateRotationTable(mouseMovement, camRotationTable);
            } else {
                camRotationTable = {
                    {1, 0, 0},
                    {0, 1, 0},
                    {0, 0, 1}
                };
            }
            rotateCam = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !spacebarPressed) {
            if(selectedBody == 4) selectedBody = 0;
            else selectedBody++;
            //read_ShapeFile_Content();
            spacebarPressed = true;
        } else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && spacebarPressed) {
            spacebarPressed = false;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !enterPressed) {
            rotatingObject++;
            if(rotatingObject > objectNum) {
                rotatingObject = 0;
            }
            enterPressed = true;
        } else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && enterPressed) {
            enterPressed = false;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp)) {
            sizeMultiplier += (float)(64.f/focalLength);
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown)) {
            sizeMultiplier -= (float)(64.f/focalLength);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && focalLength - 1 != 0) {
            focalLength -= 1;
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            focalLength += 1;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            rotateCam = true;
            updateRotationTable({0, 1}, camRotationTable);
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            rotateCam = true;
            updateRotationTable({0, -1}, camRotationTable);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            rotateCam = true;
            updateRotationTable({1, 0}, camRotationTable);
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            rotateCam = true;
            updateRotationTable({-1, 0}, camRotationTable);
        }

        if( !sf::Keyboard::isKeyPressed(sf::Keyboard::W) && 
            !sf::Keyboard::isKeyPressed(sf::Keyboard::S) && 
            !sf::Keyboard::isKeyPressed(sf::Keyboard::A) && 
            !sf::Keyboard::isKeyPressed(sf::Keyboard::D) &&
            !sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            rotateCam = false;
        }

        //////////// Update //////////////
        window.clear(sf::Color(0, 0, 0));

        switch(rotatingObject) {
            case 0:
                cube.calculateMesh(rotateCam, true);
                test.calculateMesh(rotateCam, false);
                break;
            case 1:
                test.calculateMesh(rotateCam, true);
                cube.calculateMesh(rotateCam, false);
                break;
            case (objectNum-1):
                surfaceMesh.rotateMesh_transform();
                cube.calculateMesh(rotateCam, false);
                test.calculateMesh(rotateCam, false);
                break;
            default:
                rotatingObject = 0;
        }

        if(rotateCam) {
            surfaceMesh.rotateTransform_center();
        }

        sf::VertexArray temp(sf::Triangles);
        window.draw(surfaceMesh.createMeshOnScreen(temp));
        window.draw(cube.createMeshOnScreen(temp));
        window.draw(test.createMeshOnScreen(temp));
        window.display();
    }
    return EXIT_SUCCESS;
}

void updateRotationTable(sf::Vector2i mouseMovement, std::vector<sf::Vector3f> &rotor) {
    yaw = 0;
    pitch = (float)(-mouseMovement.x)/60.f;
    roll = (float)(mouseMovement.y)/60.f;
    rotor = generateRotationMatrix({yaw, pitch, roll});
}