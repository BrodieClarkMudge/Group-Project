#include "raylib-cpp.hpp"

int main() {
    raylib::Window window(8000, 4050, "Raylib C++ Wrapper Example");
    SetTargetFPS(60);

    while (!window.ShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello from raylib-cpp!", 190, 200, 200, LIGHTGRAY);
        EndDrawing();
    }
}
