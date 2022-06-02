#ifndef ACTIONS_H
#define ACTIONS_H

#include "ant.h"

Ant doAction(int action, Ant &ant);
void dropPheromone(Ant &ant, PheromoneBlob &pheromoneBlob);

#endif