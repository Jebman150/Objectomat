#include "fileReader.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>

using namespace std;

/*
int read_ShapeFile_Content() {
    ifstream dataFile;
    string dataLine;
    dataFile.open("D:\\qwer\\objectomat\\meshData\\shapeData.txt", ios::in);
    if(!dataFile.is_open()) return EXIT_FAILURE;

    getline(dataFile, dataLine);

    //move to data of selectedBody
    while((dataLine[0] == '!' && (dataLine[1] - '0') != selectedBody) || dataLine[0] != '!') {
        getline(dataFile, dataLine);
        //remove spaces
        std::string::iterator end_pos = std::remove(dataLine.begin(), dataLine.end(), ' ');
        dataLine.erase(end_pos, dataLine.end());
        if(dataFile.eof()) return EXIT_FAILURE;
    }

    getline(dataFile, dataLine);

    int currentArea = 0;
    int counter = 0;

    std::vector<std::vector<double>> vertexTable;
    std::vector<std::vector<std::vector<double>>> sortedTable;

    while(dataLine[0] != '!') {
        //remove spaces
        std::string::iterator end_pos = std::remove(dataLine.begin(), dataLine.end(), ' ');
        dataLine.erase(end_pos, dataLine.end());

        if(dataLine[0] == '$') {
            getline(dataFile, dataLine);
            continue;
        }
        if(dataLine[0] == '-') {
            getline(dataFile, dataLine);
            counter = 0;
            currentArea++;
            continue;
        }

        //remove {, }
        end_pos = std::remove(dataLine.begin(), dataLine.end(), '{');
        dataLine.erase(end_pos, dataLine.end());
        end_pos = std::remove(dataLine.begin(), dataLine.end(), '}');
        dataLine.erase(end_pos, dataLine.end());

        switch (currentArea)
        {
        case 0:
            {
            triangleTable.resize(stoi(dataLine));
            sortedTable.resize(stoi(dataLine));
            for(int i = 0; i < sortedTable.size(); i++) {
                sortedTable[i].resize(3);
                for(int j = 0; j < sortedTable[i].size(); j++) {
                    sortedTable[i][j].resize(3);
                }
            }
            }
            break;
        case 1:
            {
            vertexTable.resize(stoi(dataLine));
            for(int i = 0; i < vertexTable.size(); i++) {
                vertexTable[i].resize(3);
            }
            }
            break;
        case 2:
            {
            std::vector<string> temp;
            split(dataLine, ",", temp);
            for(int i = 0; i < 3; i++) {
                vertexTable[counter][i] = stoi(temp[i]);
            }
            counter++;
            }
            break;
        case 3:
            {
            std::vector<string> temp;
            split(dataLine, ",", temp);
            for(int i = 0; i < 3; i++) {
                sortedTable[counter][i] = vertexTable[stoi(temp[i])];
            }
            counter++;
            }
            break;
        default:
            break;
        }
        if(dataFile.eof()) break;
        getline(dataFile, dataLine);
    }

    for(int i = 0; i < triangleTable.size(); i++) {
        triangleTable[i].recreateTriangle(sortedTable[i]);
    }

    dataFile.close();
    return EXIT_SUCCESS;
}

*/

/*int read_StaticMesh_Content() {
    ifstream dataFile;
    string dataLine;
    dataFile.open("D:\\qwer\\objectomat\\meshData\\staticMeshData.txt", ios::in);
    if(!dataFile.is_open()) return EXIT_FAILURE;

    getline(dataFile, dataLine);

    int currentArea = 0;
    int counter = 0;

    std::vector<std::vector<double>> vertexTable;
    std::vector<std::vector<std::vector<double>>> sortedTable;

    while(true) {   //will be exited explicitly
        //remove spaces
        std::string::iterator end_pos = std::remove(dataLine.begin(), dataLine.end(), ' ');
        dataLine.erase(end_pos, dataLine.end());

        if(dataLine[0] == '$') {
            getline(dataFile, dataLine);
            continue;
        }
        if(dataLine[0] == '-') {
            getline(dataFile, dataLine);
            counter = 0;
            currentArea++;
            continue;
        }

        //remove {, }
        end_pos = std::remove(dataLine.begin(), dataLine.end(), '{');
        dataLine.erase(end_pos, dataLine.end());
        end_pos = std::remove(dataLine.begin(), dataLine.end(), '}');
        dataLine.erase(end_pos, dataLine.end());

        switch (currentArea)
        {
        case 0:
            {
            staticMesh.resize(stoi(dataLine));
            sortedTable.resize(stoi(dataLine));
            for(int i = 0; i < sortedTable.size(); i++) {
                sortedTable[i].resize(3);
                for(int j = 0; j < sortedTable[i].size(); j++) {
                    sortedTable[i][j].resize(3);
                }
            }
            }
            break;
        case 1:
            {
            vertexTable.resize(stoi(dataLine));
            for(int i = 0; i < vertexTable.size(); i++) {
                vertexTable[i].resize(3);
            }
            }
            break;
        case 2:
            {
            std::vector<string> temp;
            split(dataLine, ",", temp);
            for(int i = 0; i < 3; i++) {
                vertexTable[counter][i] = stoi(temp[i]);
            }
            counter++;
            }
            break;
        case 3:
            {
            std::vector<string> temp;
            split(dataLine, ",", temp);
            for(int i = 0; i < 3; i++) {
                sortedTable[counter][i] = vertexTable[stoi(temp[i])];
            }
            counter++;
            }
            break;
        default:
            break;
        }
        if(dataFile.eof()) break;
        getline(dataFile, dataLine);
    }

    for(int i = 0; i < staticMesh.size(); i++) {
        staticMesh[i].recreateTriangle(sortedTable[i]);
    }

    dataFile.close();
    return EXIT_SUCCESS;
}*/