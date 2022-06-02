#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include "ant.h"
#include "actions.h"
#include <thread>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <array>
#include <math.h>

#define WINDOW_SIZE_X 1920
#define WINDOW_SIZE_Y 1080
#define ANT_AMOUNT 20
#define DROP_INTERVAL 5

using namespace std;
using namespace sf;

void setup();
void updateShape(Ant &currentAnt);
void startBlobThread(int threadNum);
void chooseAction(Ant &currentAnt, RenderWindow &window);
void updateBlob(int i);
float checkForPheromone(Ant &ant);
void manageFood(Ant &currentAnt);
float angleBetweenVectors(Vector2f a, Vector2f b);
float dist(Vector2f a, Vector2f b);

int frame;

int blobNum = 0;
int maxBlobNum = ANT_AMOUNT * 5;

bool deleteBlobs = false;

Ant ant[ANT_AMOUNT];
Home colonyBase;
FoodSpot foodSpot[3];
Texture antTexture;
std::vector<PheromoneBlob> pheromone;
Font font;
Text info;
array<thread, 4> worker;

Vertex pheromoneLine[] = {
    Vertex(ant[0].position),
    Vertex(ant[0].position)
};

int main() {
    //// Test space ///////

    /*{
        Vector2f a = Vector2f(2.12, -3.39);
        Vector2f b = Vector2f(-0.534, 0.84);
        cout << "Input a: (" << a.x << "|" << a.y << ")" << endl;
        cout << "Input b: (" << b.x << "|" << b.y << ")" << endl;
        cout << "Result: " << angleBetweenVectors_signed(a, b) << endl;
        return EXIT_SUCCESS;
    }*/

    ///////////////////////
    RenderWindow window(VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "Ant Simulation"); 
    antTexture.loadFromFile("D:\\antSim\\textures\\ant.jpg");
    pheromone.resize(maxBlobNum);
    pheromone[ANT_AMOUNT-1].size = .1;
    setup();
    if(!font.loadFromFile("Cabin-Regular.ttf")) {
        cout << "Exiting..." << endl;
        return EXIT_FAILURE;
    }
    window.setFramerateLimit(60);

    while(window.isOpen()) {
        Event event;
        int someTimer = 0;
        while(window.pollEvent(event)) {
            switch(event.type) {
                case Event::Closed:
                    window.close();
                    break;
                case Event::Resized:
                    window.setSize(Vector2u(WINDOW_SIZE_X, WINDOW_SIZE_Y));   //lock the size of the window
                    break;
            }
        }
        //////////// Update //////////////
        window.clear(Color(100, 50, 0));
        if(blobNum >= pheromone.size()-ANT_AMOUNT) {
            maxBlobNum += ANT_AMOUNT;
            pheromone.resize(maxBlobNum);
            //cout << "Resizing: " << blobNum << endl;
        }

        if(frame >= 1500 && frame % DROP_INTERVAL == 0) {
            for(int i = ANT_AMOUNT; i < blobNum; i++) {
                pheromone[i-ANT_AMOUNT] = pheromone[i];
            }
            //cout << "Deleting blobs 0 - " << ANT_AMOUNT << endl;
            blobNum -= ANT_AMOUNT;
        }

        for (int j = 0; j < blobNum; j++) {
            if(pheromone[j].size <= 0) {
                for(int i = j; i < blobNum-1; i++) {
                    pheromone[i] = pheromone[i+1];
                }
                blobNum--;
            }
        }

        for(auto &oneAnt : ant) {
            chooseAction(oneAnt, window);
            manageFood(oneAnt);
            updateShape(oneAnt);
        }

        if(frame % DROP_INTERVAL == 0) {
            worker[0] = thread([] {startBlobThread(1);});
            worker[1] = thread([] {startBlobThread(2);});
            worker[2] = thread([] {startBlobThread(3);});
            worker[3] = thread([] {startBlobThread(4);});
            
            worker[0].join();
            worker[1].join();
            worker[2].join();
            worker[3].join();
            blobNum += ANT_AMOUNT;
        }

        for(int i = 0; i < blobNum; i++) {
            updateBlob(i);
        }

        Vertex directionLine[] = {
            Vertex(ant[0].position),
            Vertex(ant[0].position + angleToVector(ant[0].pointingDirection) * 100.f),
            Vertex(ant[0].position),
            Vertex(ant[0].position + angleToVector(vectorToAngle(Vector2f(Mouse::getPosition(window)) - ant[0].position)) * 100.f)
        };

        directionLine[0].color = Color::Blue;
        directionLine[1].color = Color::Blue;
        directionLine[2].color = Color::Red;
        directionLine[3].color = Color::Red;

        /////////// Draw visuals ////////////
        info.setString("Frame: " + to_string(frame));
        window.draw(info);
        for(auto &&oneAnt : ant) {
            window.draw(oneAnt.body);
        }
        for(auto &&oneBlob : pheromone) {
            window.draw(oneBlob.shape);
        }
        for(auto &&oneFoodSpot : foodSpot) {
            window.draw(oneFoodSpot.shape);
        }
        window.draw(colonyBase.shape);
        window.draw(directionLine, 4, Lines);
        window.draw(pheromoneLine, 2, Lines);
        window.display();
        frame++;
    }
    return EXIT_SUCCESS;
}

///////////// Methods ///////////////

void setup() {
    frame = 0;

    colonyBase.position = Vector2f(WINDOW_SIZE_X/2, WINDOW_SIZE_Y/2);
    colonyBase.shape = CircleShape(20);
    colonyBase.shape.setOrigin(20, 20);
    colonyBase.shape.setPosition(colonyBase.position);
    colonyBase.shape.setFillColor(Color::Red);
    colonyBase.storedFood = 0;

    for (int i = 0; i < ANT_AMOUNT; i++)
    {
        ant[i].position = colonyBase.position;
        ant[i].body.setOrigin(Vector2f(antTexture.getSize()) * .5f);
        ant[i].body.setTexture(antTexture);
        ant[i].body.setScale(ant[i].scale, ant[i].scale);
    }

    foodSpot[0].position = Vector2f(WINDOW_SIZE_X/4, WINDOW_SIZE_Y/2);
    foodSpot[1].position = Vector2f(WINDOW_SIZE_X/2, WINDOW_SIZE_Y/5);
    foodSpot[2].position = Vector2f(WINDOW_SIZE_X - WINDOW_SIZE_X/4, WINDOW_SIZE_Y/3);
    for(int i = 0; i < 3; i++) {
        foodSpot[i].storedFood = 100;
        foodSpot[i].shape = CircleShape(foodSpot[i].storedFood/3);
        foodSpot[i].shape.setOrigin(foodSpot[i].storedFood/3, foodSpot[i].storedFood/3);
        foodSpot[i].shape.setPosition(foodSpot[i].position);
        foodSpot[i].shape.setFillColor(Color::Green);
    }
    
    info.setFont(font);
    info.setPosition(WINDOW_SIZE_X/100, WINDOW_SIZE_Y/100);
    info.setCharacterSize(16);
}


void updateShape(Ant &currentAnt) {
    currentAnt.body.setPosition(currentAnt.position);
    currentAnt.body.setRotation(currentAnt.pointingDirection+90);
}

void startBlobThread(int threadNum) {
    switch(threadNum) {
        case 1:
        {
            for(int i = blobNum; i < blobNum + ANT_AMOUNT / 4; i++) {
                ant[i-blobNum].dropPheromone(pheromone[i]);
            }
        }
        case 2:
        {
            for(int i = blobNum + ANT_AMOUNT / 4; i < blobNum + ANT_AMOUNT / 2; i++) {
                ant[i-blobNum].dropPheromone(pheromone[i]);
            }
        }
        case 3:
        {
            for(int i = blobNum + ANT_AMOUNT / 2; i < blobNum + (ANT_AMOUNT - ANT_AMOUNT / 4); i++) {
                ant[i-blobNum].dropPheromone(pheromone[i]);
            }
        }
        case 4:
        {
            for(int i = blobNum + (ANT_AMOUNT - ANT_AMOUNT / 4); i < blobNum + ANT_AMOUNT; i++) {
                ant[i-blobNum].dropPheromone(pheromone[i]);
            }
        }
    }
}

void chooseAction(Ant &currentAnt, RenderWindow &window) {
    switch(currentAnt.currentTarget) {
        /*case Target::exploring:
            if(rand() % 30 == 1) {
                currentAnt.currentState = State(rand() % State::END_OF_ACTIONS);
            }
            currentAnt = doAction(currentAnt.currentState, currentAnt);
            break;

            /*currentAnt.pointingDirection = vectorToAngle(Vector2f(Mouse::getPosition(window)) - currentAnt.position);
            currentAnt.moveFw();
            break;*/
        case Target::food:
        {
            float targetAngle = checkForPheromone(currentAnt);
            currentAnt.pointingDirection += clamp(currentAnt.turningSpeed, -currentAnt.turningSpeed, targetAngle) + rand() % int(currentAnt.turningSpeed * 2) - currentAnt.turningSpeed;
            currentAnt.moveFw();
            break;
        }
        case Target::home:
        {
            float targetAngle = checkForPheromone(currentAnt);
            currentAnt.pointingDirection += clamp(currentAnt.turningSpeed, -currentAnt.turningSpeed, targetAngle);
            currentAnt.moveFw();
            break;
        }
        default:
            break;
    }
}

void updateBlob(int i) {
    pheromone[i].size -= .002;
    pheromone[i].shape.setRadius(pheromone[i].size);
}

float checkForPheromone(Ant &ant) {     //it checks for the nearest pheromone in the FOV of the given ant
    int range = 50;
    float targetAngle = 0;
    float blobAmountInRange = 1;
    for(auto &&blob : pheromone) {
        if(dist(blob.position, ant.position) < range && blob.type == ant.currentTarget && dist(blob.position, ant.position) > .5) {
            float angleToBlob;
            float distance = dist(blob.position, ant.position);
            angleToBlob = angleBetweenVectors_signed(blob.position - ant.position, angleToVector(ant.pointingDirection));

            if(targetAngle > -50 && targetAngle < 50) {
                pheromoneLine[0] = Vertex(ant.position);
                pheromoneLine[1] = Vertex(blob.position);
                targetAngle += angleToBlob;
                blobAmountInRange += blob.size;
            }
        }
    }
    
    return -targetAngle;
}

void manageFood(Ant &currentAnt) {
    if(dist(currentAnt.position, colonyBase.position) < colonyBase.shape.getRadius() && currentAnt.currentTarget == Target::home) {
        colonyBase.storedFood += currentAnt.carringFood;
        currentAnt.carringFood = 0;
        currentAnt.currentTarget = Target::food;
        currentAnt.pointingDirection -= 180;
        return;
    }

    for(int i = 0; i < 3; i++) {
        if(dist(foodSpot[i].position, currentAnt.position) <= foodSpot[i].shape.getRadius()) {
            cout << "Food found!" << endl;
            foodSpot[i].storedFood--;
            currentAnt.carringFood++;
            currentAnt.currentTarget = Target::home;
            currentAnt.pointingDirection -= 180;
            currentAnt.moveFw();
        }
    }
}