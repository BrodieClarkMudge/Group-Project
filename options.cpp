#include "options.h"
#include "raylib.h"

void ShowOptions(Options &options) {
    const int optionsWidth = 400;
    const int optionsHeight = 300;

    int optionsX = (GetScreenWidth()  - optionsWidth)  / 2;
    int optionsY = (GetScreenHeight() - optionsHeight) / 2;

    Rectangle optionsRect = { (float)optionsX, (float)optionsY, (float)optionsWidth, (float)optionsHeight };
    Rectangle soundRect = { optionsRect.x + 50, optionsRect.y + 50, 300, 50 };
    Rectangle sliderRect = { optionsRect.x + 50, optionsRect.y + 120, 300, 20 }; // slider background

    bool draggingSlider = false;

    while (!WindowShouldClose()) {
        int winWidth = GetScreenWidth();
        int winHeight = GetScreenHeight();

        if (winWidth < 800 || winHeight < 500) {
            SetWindowSize((winWidth < 800) ? 800 : winWidth,
                          (winHeight < 500) ? 500 : winHeight);
        }
        
        Vector2 mousePos = GetMousePosition();

        // Start dragging when mouse pressed on slider
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, sliderRect)) {
            draggingSlider = true;
        }
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            draggingSlider = false;
        }

        // Update timeScale if dragging
        if (draggingSlider) {
            float newX = mousePos.x;
            if (newX < sliderRect.x) newX = sliderRect.x;
            if (newX > sliderRect.x + sliderRect.width) newX = sliderRect.x + sliderRect.width;

            float tNorm = (newX - sliderRect.x) / sliderRect.width; // 0..1
            options.timeScale = 0.5f + tNorm * (2.0f - 0.5f); // map to 0.5x - 2.0x
        }

        BeginDrawing();

        // Draw options background
        DrawRectangleRec(optionsRect, LIGHTGRAY);

        // Draw sound toggle
        DrawRectangleRec(soundRect, DARKGRAY);
        DrawText(options.soundFX ? "Sound FX: ON" : "Sound FX: OFF", soundRect.x + 10, soundRect.y + 10, 20, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, soundRect)) {
            options.soundFX = !options.soundFX;
        }

        // Draw timeScale slider
        DrawRectangleRec(sliderRect, DARKGRAY);

        // Knob
        float knobX = sliderRect.x + ((options.timeScale - 0.5f) / (2.0f - 0.5f)) * sliderRect.width;
        Rectangle knobRect = { knobX - 10, sliderRect.y - 5, 20, 30 };
        DrawRectangleRec(knobRect, RED);

        DrawText(TextFormat("Timescale: %.2fx", options.timeScale), sliderRect.x, sliderRect.y - 30, 20, BLACK);

        DrawText("Press ENTER to start", optionsRect.x + 80, optionsRect.y + 220, 20, BLACK);

        if (IsKeyPressed(KEY_ENTER)) break;

        EndDrawing();
    }
}
