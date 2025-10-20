#include "menu.h"
#include "raylib.h"

void ShowMainMenu(MainMenu &menu) {
    const int menuWidth = 400;
    const int menuHeight = 300;

    int menuX = (GetScreenWidth()  - menuWidth)  / 2;
    int menuY = (GetScreenHeight() - menuHeight) / 2 + 60;

    Rectangle menuRect = { (float)menuX, (float)menuY, (float)menuWidth, (float)menuHeight };
    Rectangle soundRect = { menuRect.x + 50, menuRect.y + 50, 300, 50 };
    Rectangle sliderRect = { menuRect.x + 50, menuRect.y + 160, 300, 20 }; // slider background

    bool draggingSlider = false;

    while (!WindowShouldClose()) {
        int winWidth = GetScreenWidth();
        int winHeight = GetScreenHeight();

        if (winWidth < 800 || winHeight < 500) {
            SetWindowSize((winWidth < 800) ? 800 : winWidth,
                          (winHeight < 500) ? 500 : winHeight);
        }
        
        Vector2 mousePos = GetMousePosition();

        if (menu.soundFX) SetMasterVolume(1.0f);
        else       SetMasterVolume(0.0f);

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
            menu.timeScale = 0.5f + tNorm * (2.0f - 0.5f); // map to 0.5x - 2.0x
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw menu background
        DrawRectangleRec(menuRect, LIGHTGRAY);

        // Draw sound toggle
        DrawRectangleRec(soundRect, DARKGRAY);
        DrawText(menu.soundFX ? "Sound FX: ON" : "Sound FX: OFF", soundRect.x + 10, soundRect.y + 10, 20, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, soundRect)) {
            menu.soundFX = !menu.soundFX;
        }

        // Draw timeScale slider
        DrawRectangleRec(sliderRect, DARKGRAY);

        // Knob
        float knobX = sliderRect.x + ((menu.timeScale - 0.5f) / (2.0f - 0.5f)) * sliderRect.width;
        Rectangle knobRect = { knobX - 10, sliderRect.y - 5, 20, 30 };
        DrawRectangleRec(knobRect, RED);

        DrawText(TextFormat("Timescale: %.2fx", menu.timeScale), sliderRect.x, sliderRect.y - 30, 20, BLACK);

        DrawText("Welcome To", menuRect.x + 80, menuRect.y - 200, 40, DARKBROWN);
        DrawText("BETTERFARM++", menuRect.x - 185, menuRect.y - 160, 100, DARKGREEN);
        
        DrawText("Press ENTER to start", menuRect.x + 65, menuRect.y + 220, 20, BLACK);

        if (IsKeyPressed(KEY_ENTER)) break;

        EndDrawing();
    }
}
