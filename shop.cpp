#include "shop.h"
#include "raylib.h"

void ShowShop(Shop &shop) {
    const int shopWidth = 400;
    const int shopHeight = 300;

    int shopX = (GetScreenWidth()  - shopWidth)  / 2;
    int shopY = (GetScreenHeight() - shopHeight) / 2;

    Rectangle shopRect = { (float)shopX, (float)shopY, (float)shopWidth, (float)shopHeight };
    Rectangle soundRect = { shopRect.x + 50, shopRect.y + 50, 300, 50 };
    Rectangle sliderRect = { shopRect.x + 50, shopRect.y + 120, 300, 20 }; // slider background

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
            shop.timeScale = 0.5f + tNorm * (2.0f - 0.5f); // map to 0.5x - 2.0x
        }

        BeginDrawing();

        // Draw shop background
        DrawRectangleRec(shopRect, LIGHTGRAY);

        // Draw sound toggle
        DrawRectangleRec(soundRect, DARKGRAY);
        DrawText(shop.soundFX ? "Sound FX: ON" : "Sound FX: OFF", soundRect.x + 10, soundRect.y + 10, 20, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, soundRect)) {
            shop.soundFX = !shop.soundFX;
        }

        // Draw timeScale slider
        DrawRectangleRec(sliderRect, DARKGRAY);

        // Knob
        float knobX = sliderRect.x + ((shop.timeScale - 0.5f) / (2.0f - 0.5f)) * sliderRect.width;
        Rectangle knobRect = { knobX - 10, sliderRect.y - 5, 20, 30 };
        DrawRectangleRec(knobRect, RED);

        DrawText(TextFormat("Timescale: %.2fx", shop.timeScale), sliderRect.x, sliderRect.y - 30, 20, BLACK);

        DrawText("Press ENTER to start", shopRect.x + 80, shopRect.y + 220, 20, BLACK);

        if (IsKeyPressed(KEY_ENTER)) break;

        EndDrawing();
    }
}
