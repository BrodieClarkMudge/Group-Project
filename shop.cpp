#include "shop.h"
#include "ui.h"
#include <string>

Shop::Shop() {
    open = false;
    buttonWidth = 200;
    buttonHeight = 40;
    windowRect = { 300, 150, 800, 400 }; // Example window size
    FarmTextures tex;
}

void Shop::Update(std::vector<Tile>& tiles, int& coins, int& water,
                  Texture2D& yardTex, Texture2D& cowTex, Texture2D& sheepTex,
                  Texture2D& chickenTex, Texture2D& pigTex) {
    if (!open) return;

    Vector2 mousePos = GetMousePosition();
    placing = false;

    // Yard button
    Rectangle yardBtn = { windowRect.x + 20, windowRect.y + 50, (float)buttonWidth, (float)buttonHeight };
    if (CheckCollisionPointRec(mousePos, yardBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && coins >= 20) {
        setSelectedItem("YARD");
        placing = true;   // now in placement mode
        open = false;     // close shop
    }

    // Cow button
    Rectangle cowBtn = { windowRect.x + 20, windowRect.y + 100, (float)buttonWidth, (float)buttonHeight };
    if (CheckCollisionPointRec(mousePos, cowBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && coins >= 50) {
        setSelectedItem("COW");
        placing = true;
        open = false;
    }

    // Sheep button
    Rectangle sheepBtn = { windowRect.x + 20, windowRect.y + 150, (float)buttonWidth, (float)buttonHeight };
    if (CheckCollisionPointRec(mousePos, sheepBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && coins >= 40) {
        setSelectedItem("SHEEP");
        placing = true;
        open = false;
    }

    // Chicken button
    Rectangle chickenBtn = { windowRect.x + 20, windowRect.y + 200, (float)buttonWidth, (float)buttonHeight };
    if (CheckCollisionPointRec(mousePos, chickenBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && coins >= 30) {
        setSelectedItem("CHICKEN");
        placing = true;
        open = false;
    }

    // Pig button
    Rectangle pigBtn = { windowRect.x + 20, windowRect.y + 250, (float)buttonWidth, (float)buttonHeight };
    if (CheckCollisionPointRec(mousePos, pigBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && coins >= 60) {
        setSelectedItem("PIG");
        placing = true;
        open = false;
    }

    Rectangle pumpkinBtn = { windowRect.x + 300, windowRect.y + 50, (float)buttonWidth, (float)buttonHeight };
    if (CheckCollisionPointRec(mousePos, pumpkinBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && coins >= 10) {
        setSelectedItem("PUMPKIN");
        placing = true;
        open = false;
    }

    Rectangle tomatoBtn = { windowRect.x + 300, windowRect.y + 100, (float)buttonWidth, (float)buttonHeight };
    if (CheckCollisionPointRec(mousePos, tomatoBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && coins >= 10) {
        setSelectedItem("TOMATO");
        placing = true;
        open = false;
    }

    Rectangle potatoBtn = { windowRect.x + 300, windowRect.y + 150, (float)buttonWidth, (float)buttonHeight };
    if (CheckCollisionPointRec(mousePos, potatoBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && coins >= 10) {
        setSelectedItem("POTATO");
        placing = true;
        open = false;
    }

    Rectangle addWaterBtn = { windowRect.x + 300, windowRect.y + 200, (float)buttonWidth, (float)buttonHeight };
    if (CheckCollisionPointRec(mousePos, addWaterBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && coins >= 50) {
        water += 10;
        coins -= 50;
    }

    Rectangle expand = { windowRect.x + 20, windowRect.y + 300, (float)buttonWidth, (float)buttonHeight };
    if (CheckCollisionPointRec(mousePos, expand) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (expandedGrid == 0 && coins >= 500) {
            expandingTrue = true;
            expandedGrid = 1;
            coins -= 500;
        } else if (expandedGrid == 1 && coins >= 2000) {
            expandingTrue = true;
            expandedGrid = 2;
            coins -= 2000;
        } else if (expandedGrid == 2 && coins >= 5000) {
            expandingTrue = true;
            expandedGrid = 3;
            coins -= 5000;
        }
    }

    if (IsKeyPressed(KEY_ENTER)) {
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
    DrawText("Buy Pumpkin Seed - 10 coins", windowRect.x + 310, windowRect.y + 60, 20, ORANGE);
    DrawText("Buy Tomato Seed - 10 coins", windowRect.x + 310, windowRect.y + 110, 20, RED);
    DrawText("Buy Potato Seed - 10 coins", windowRect.x + 310, windowRect.y + 160, 20, YELLOW);
    DrawText("Buy 10 Water - 50 coins", windowRect.x + 310, windowRect.y + 210, 20, BLUE);
    if (expandedGrid == 0) {
        DrawText("Expand farm - 500 coins", windowRect.x + 30, windowRect.y + 310, 20, GOLD);
    } else if (expandedGrid == 1) {
        DrawText("Expand farm 2 - 2000 coins", windowRect.x + 30, windowRect.y + 310, 20, GOLD);
    } else if (expandedGrid == 2) {
        DrawText("Expand farm 3 - 5000 coins", windowRect.x + 30, windowRect.y + 310, 20, GOLD);
    } else {
        DrawText("Max farm expansion reached", windowRect.x + 30, windowRect.y + 310, 20, GOLD);
    }
    DrawText("Press ENTER to exit",windowRect.x + 60, windowRect.y + 360, 20, BLACK);
}
