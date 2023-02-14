//
// Created by Muhammad Al Fahad on 2/14/2023.
//

#ifndef LIGHT_PATH_EDGE_H
#define LIGHT_PATH_EDGE_H

#include "raylib.h"


class Edge {
public:
    Vector2 start;
    Vector2 end;

    Edge(Vector2 start, Vector2 end) : start(start), end(end) {}
};


#endif //LIGHT_PATH_EDGE_H
