//
// Created by Muhammad Al Fahad on 2/12/2023.
//

#include "Simulation.h"
#include <cmath>
#include <iostream>

float magnitude(Vector2 vector) {
    return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

int Simulation::generatePath() {
    int lastIncidentEdge = -1;

    lightPath[0] = lightPoint;

    for (int i = 1; i < pathMaxCount + 1; ++i) {
        float minTD = std::numeric_limits<float>::max();
        Vector2 &oD = lightPath[i - 1];
        Vector2 dD;

        if (i == 1) {
            dD.x = directionPoint.x - oD.x;
            dD.y = directionPoint.y - oD.y;
        } else {
            Vector2 normal = {edges[lastIncidentEdge].start.y - edges[lastIncidentEdge].end.y,
                              edges[lastIncidentEdge].end.x - edges[lastIncidentEdge].start.x};
            float m = magnitude(normal);
            normal.x /= m;
            normal.y /= m;
            Vector2 vector = {lightPath[i - 1].x - lightPath[i - 2].x, lightPath[i - 1].y - lightPath[i - 2].y};
            float dot = vector.x * normal.x + vector.y * normal.y;
            dD = {vector.x - 2 * dot * normal.x, vector.y - 2 * dot * normal.y};
        }

        int incidentEdge = -1;
        for (int j = 0; j < edges.size(); ++j) {
            if (j == lastIncidentEdge) continue;

            Vector2 &oL = edges[j].start;
            Vector2 dL{edges[j].end.x - oL.x, edges[j].end.y - oL.y};

            float tD = (dL.x * (oL.y - oD.y) - dL.y * (oL.x - oD.x)) / (dL.x * dD.y - dL.y * dD.x);
            float tL = (dD.x * (oD.y - oL.y) - dD.y * (oD.x - oL.x)) / (dD.x * dL.y - dD.y * dL.x);
            if (tD < 0 || tL > 1 || tL < 0) continue;

            if (tD < minTD) {
                minTD = tD;
                incidentEdge = j;
            }
        }


        lastIncidentEdge = incidentEdge;

        lightPath[i].x = oD.x + dD.x * minTD;
        lightPath[i].y = oD.y + dD.y * minTD;

        if (incidentEdge < 4) return i;
    }
    return pathMaxCount;
}

Vector2 Simulation::getDirection() {
    Vector2 direction{directionPoint.x - lightPoint.x, directionPoint.y - lightPoint.y};
    float magnitude = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
    direction.x /= magnitude;
    direction.y /= magnitude;
    return direction;
}
