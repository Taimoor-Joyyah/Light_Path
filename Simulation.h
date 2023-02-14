//
// Created by Muhammad Al Fahad on 2/12/2023.
//

#ifndef LIGHT_PATH_SIMULATION_H
#define LIGHT_PATH_SIMULATION_H

#include "raylib.h"
#include "Edge.h"
#include <vector>

class Simulation {
public:
    std::vector<Edge> edges;
    std::vector<Vector2> lightPath;
    Vector2 lightPoint{1120, 540};
    Vector2 directionPoint{1620, 340};
    int pathMaxCount = 0;

    int generatePath();

    Vector2 getDirection();
};


#endif //LIGHT_PATH_SIMULATION_H
