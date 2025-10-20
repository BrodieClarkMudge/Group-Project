#include "shop.h"
#include <string>

Shop::Shop() {
    open = false;
    buttonWidth = 200;
    buttonHeight = 40;
    windowRect = { 300, 150, 400, 400 }; // Example window size
    FarmTextures tex = LoadFarmTextures();
}

void Shop::Update(std::vector<Tile>& tiles, int& coins,
                  Texture2D& yardTex, Texture2D& cowTex, Texture2D& sheepTex,
                  Texture2D& chickenTex, Texture2D& pigTex) {
    if (!open) return;

    Vector2 mousePos = GetMousePosition();

    // Yard button
    Rectangle yardBtn = { windowRect.x + 20, windowRect.y + 50, (float)buttonWidth, (float)buttonHeight };
    if (CheckCollisionPointRec(mousePos, yardBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (coins >= 20) {
            // Prepare placement logic in main (not here)
            selectedItem = "YARD";  // store what the user clicked
            open = false;           // close shop immediately
        }
    }

    // Cow button
    Rectangle cowBtn = { windowRect.x + 20, windowRect.y + 100, (float)buttonWidth, (float)buttonHeight };
    if (CheckCollisionPointRec(mousePos, cowBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && coins >= 50) {
        selectedItem = "COW";
        open = false;
    }

    // Sheep button
    Rectangle sheepBtn = { windowRect.x + 20, windowRect.y + 150, (float)buttonWidth, (float)buttonHeight };
    if (CheckCollisionPointRec(mousePos, sheepBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && coins >= 40) {
        selectedItem = "SHEEP";
        open = false;
    }

    // Chicken button
    Rectangle chickenBtn = { windowRect.x + 20, windowRect.y + 200, (float)buttonWidth, (float)buttonHeight };
    if (CheckCollisionPointRec(mousePos, chickenBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && coins >= 30) {
        selectedItem = "CHICKEN";
        open = false;
    }

    // Pig button
    Rectangle pigBtn = { windowRect.x + 20, windowRect.y + 250, (float)buttonWidth, (float)buttonHeight };
    if (CheckCollisionPointRec(mousePos, pigBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && coins >= 60) {
        selectedItem = "PIG";
        open = false;
    }
}


void Shop::Draw() {
    if (!open) return;

    DrawRectangleRec(windowRect, RAYWHITE);
    DrawRectangleLinesEx(windowRect, 2, BLACK);

    DrawText("Shop", windowRect.x + 150, windowRect.y + 10, 30, BLACK);
    DrawText("Buy Yard - 20 coins", windowRect.x + 30, windowRect.y + 60, 20, DARKGREEN);
    DrawText("Buy Cow - 50 coins", windowRect.x + 30, windowRect.y + 110, 20, BROWN);
    DrawText("Buy Sheep - 40 coins", windowRect.x + 30, windowRect.y + 160, 20, GRAY);
    DrawText("Buy Chicken - 30 coins", windowRect.x + 30, windowRect.y + 210, 20, DARKGRAY);
    DrawText("Buy Pig - 60 coins", windowRect.x + 30, windowRect.y + 260, 20, MAROON);
}
