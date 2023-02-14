#include <iostream>

#define RAYGUI_IMPLEMENTATION

#include "raylib.h"
#include "raygui.h"
#include "raymath.h"

#include "Simulation.h"
#include "Edge.h"

bool inControls(const Vector2 &mouse) {
    return mouse.x <= 320 || mouse.y < 0;
}

bool inRange(const Vector2 &mouse, const Vector2 &bulb, float range) {
    return Vector2Distance(mouse, bulb) <= range;
}

int main() {
    int maxPathCount = 0;
    float guideRadius = 10;

    InitWindow(1920, 1080, "Bulb Rendering");
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    SetTargetFPS(60);

    Simulation sim;

    sim.edges = {
            {{320,  0}, {1920, 0}},
            {{1920, 0}, {1920, 1080}},
            {{1920, 1080}, {320, 1080}},
            {{320, 1080}, {320, 0}}
    };
    sim.lightPath.push_back(sim.lightPoint);

    bool createEdge = false;
    bool showDirection = false;
    bool useGuides = false;
    int pathCount = sim.generatePath();
    Vector2 direction = sim.getDirection();
    Vector2 *selectedVector = nullptr;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground({223, 223, 223, 255});


        for (auto &edge: sim.edges)
            DrawLineEx(edge.start, edge.end, 5, {16, 16, 16, 255});


        for (int i = 1; i < pathCount + 1; ++i)
            DrawLineEx(sim.lightPath[i - 1], sim.lightPath[i], 2, {255, 0, 0, 192});

        if (showDirection) {
            for (int i = 0; i < 50; i += 2) {
                DrawLineEx({sim.lightPoint.x + direction.x * i * 10, sim.lightPoint.y + direction.y * i * 10},
                           {sim.lightPoint.x + direction.x * (i + 1) * 10,
                            sim.lightPoint.y + direction.y * (i + 1) * 10},
                           5, {0, 0, 0, 128});
            }
        }

        DrawCircleV(sim.lightPoint, 20, {191, 31, 31, 255});
        DrawCircleV(sim.directionPoint, 15, {127, 127, 127, 255});

        GuiWindowBox({0, 0, 320, 1080}, "Controls");

        int tempCount = sim.pathMaxCount;
        GuiSpinner({100, 50, 200, 20}, "Path Count", &sim.pathMaxCount, 0, 0, true);
        if (sim.pathMaxCount > maxPathCount) {
            for (int i = maxPathCount; i < sim.pathMaxCount; ++i)
                    sim.lightPath.push_back({});
            maxPathCount = sim.pathMaxCount;
        }
        if (sim.pathMaxCount < 0) sim.pathMaxCount = 0;
        if (sim.pathMaxCount > tempCount) pathCount = sim.generatePath();
        else if (sim.pathMaxCount < tempCount) pathCount = std::min(pathCount, sim.pathMaxCount);

        showDirection = GuiToggle({100, 110, 100, 30}, "Show Guide", showDirection);
        createEdge = GuiToggle({100, 150, 100, 30}, "Create Edge", createEdge);
        useGuides = GuiToggle({100, 190, 100, 30}, "Use Guides", useGuides);
        if(GuiButton({100, 230, 100, 30}, "Clear Edges")) {
            sim.edges.clear();
            sim.edges = {
                    {{320,  0}, {1920, 0}},
                    {{1920, 0}, {1920, 1080}},
                    {{1920, 1080}, {320, 1080}},
                    {{320, 1080}, {320, 0}}
            };
            pathCount = sim.generatePath();
        }
        if(GuiButton({100, 270, 100, 30}, "Undo Edges")) {
            if (sim.edges.size() > 4)
                sim.edges.pop_back();
            pathCount = sim.generatePath();
        }

        DrawFPS(1800, 20);

        Vector2 mouse = GetMousePosition();
        if (useGuides) {
            for (auto &edge: sim.edges) {
                if (selectedVector != &edge.start && inRange(mouse, edge.start, guideRadius)) mouse = edge.start;
                else if (selectedVector != &edge.end && inRange(mouse, edge.end, guideRadius)) mouse = edge.end;
            }
        }
        GuiLabel({100, 80, 50, 20}, "Cursor");
        GuiLabel({180, 80, 50, 20}, std::to_string((int) mouse.x).c_str());
        GuiLabel({240, 80, 50, 20}, std::to_string((int) mouse.y).c_str());
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !inControls(mouse)) {
            if (selectedVector == nullptr) {
                if (createEdge) {
                    sim.edges.emplace_back(mouse, mouse);
                    selectedVector = &sim.edges[sim.edges.size() - 1].end;
                } else {
                    if (inRange(mouse, sim.lightPoint, 20))
                        selectedVector = &sim.lightPoint;
                    else if (inRange(mouse, sim.directionPoint, 15))
                        selectedVector = &sim.directionPoint;
                    else {
                        for (auto & edge: sim.edges) {
                            if (inRange(mouse, edge.start, guideRadius)) { selectedVector = &edge.start; break;}
                            if (inRange(mouse, edge.end, guideRadius)) { selectedVector = &edge.end; break; }
                        }
                    }
                }
            } else {
                selectedVector->x = mouse.x;
                selectedVector->y = mouse.y;

                pathCount = sim.generatePath();
                direction = sim.getDirection();
            }
        } else
            selectedVector = nullptr;

        EndDrawing();
    }
    CloseWindow();

    return 0;
}
