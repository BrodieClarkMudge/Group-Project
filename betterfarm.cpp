#include "raylib.h"
#include "animal.h"
#include "chicken.h"
#include "cow.h"
#include "sheep.h"
#include "pig.h"

#include <vector>
#include <string>
#include <memory>

// ----------------------
// Tile holds unique_ptr<Animal>
// ----------------------
struct Tile {
    Rectangle rect;
    std::unique_ptr<Animal> animal = nullptr;
};

// ----------------------
// Grid builder
// ----------------------
void RebuildGrid(std::vector<Tile>& tiles, int cols, int rows,
                 int areaWidth, int areaHeight, int offsetX, int offsetY)
{
    std::vector<std::unique_ptr<Animal>> oldAnimals;
    oldAnimals.reserve(tiles.size());
    for (auto& tile : tiles) {
    DrawRectangleRec(tile.rect, DARKGREEN);
    DrawRectangleLinesEx(tile.rect, 1, BLACK);
    if (tile.animal) {
        tile.animal->draw(tile.rect.x, tile.rect.y, tile.rect.width, tile.rect.height);
    }
}

    tiles.clear();
    float tileWidth = static_cast<float>(areaWidth) / cols;
    float tileHeight = static_cast<float>(areaHeight) / rows;

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            Tile t;
            t.rect = { offsetX + x * tileWidth, offsetY + y * tileHeight, tileWidth, tileHeight };
            int index = y * cols + x;
            if (index < (int)oldAnimals.size()) {
                t.animal = std::move(oldAnimals[index]);
            }
            tiles.push_back(std::move(t));
        }
    }
}

// Expand grid keeping existing animals
void ExpandGrid(std::vector<Tile>& tiles, int oldCols, int oldRows,
                int newCols, int newRows,
                int areaWidth, int areaHeight, int offsetX, int offsetY)
{
    float tileWidth = static_cast<float>(areaWidth) / newCols;
    float tileHeight = static_cast<float>(areaHeight) / newRows;

    std::vector<Tile> newTiles;
    newTiles.reserve(newCols * newRows);

    for (int y = 0; y < newRows; y++) {
        for (int x = 0; x < newCols; x++) {
            Tile t;
            t.rect = { offsetX + x * tileWidth, offsetY + y * tileHeight, tileWidth, tileHeight };

            if (x < oldCols && y < oldRows) {
                t.animal = std::move(tiles[y * oldCols + x].animal);
            }
            newTiles.push_back(std::move(t));
        }
    }
    tiles = std::move(newTiles);
}

// ----------------------
// Main
// ----------------------
int main() {
    const int initialWidth = 1280;
    const int initialHeight = 800;
    const float aspectRatio = 16.0f / 10.0f;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(640, 400);
    InitWindow(initialWidth, initialHeight, "BetterFarm++ OOP (unique_ptr)");

    int gridCols = 16, gridRows = 10;
    const int minGrid = 10, maxGrid = 24;

    enum class Selected { COW, SHEEP, CHICKEN, PIG };
    Selected selectedAnimal = Selected::COW;

    // load textures
    Texture2D cowTex = LoadTexture("assets/cowPixel.png");
    Texture2D sheepTex = LoadTexture("assets/sheepPixel.png");
    Texture2D chickenTex = LoadTexture("assets/chickenPixel.png");
    Texture2D pigTex = LoadTexture("assets/pigPixel.png");

    std::vector<Tile> tiles;
    int prevCols = gridCols, prevRows = gridRows;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        int winWidth = GetScreenWidth();
        int winHeight = GetScreenHeight();

        if (winWidth < 800 || winHeight < 500) {
            SetWindowSize((winWidth < 800) ? 800 : winWidth,
                          (winHeight < 500) ? 500 : winHeight);
        }

        int areaWidth = winWidth;
        int areaHeight = static_cast<int>(winWidth / aspectRatio);
        if (areaHeight > winHeight) {
            areaHeight = winHeight;
            areaWidth = static_cast<int>(winHeight * aspectRatio);
        }
        int offsetX = (winWidth - areaWidth) / 2;
        int offsetY = (winHeight - areaHeight) / 2;

        // grid controls
        if (IsKeyPressed(KEY_UP) && gridRows < maxGrid && gridCols < maxGrid) {
            gridRows += 2; gridCols += 3;
        }
        if (IsKeyPressed(KEY_DOWN) && gridRows > minGrid && gridCols > minGrid) {
            gridRows -= 2; gridCols -= 3;
        }

        // rebuild grid if size changed
        if (gridCols != prevCols || gridRows != prevRows || tiles.empty()) {
            if (!tiles.empty()) {
                ExpandGrid(tiles, prevCols, prevRows, gridCols, gridRows, areaWidth, areaHeight, offsetX, offsetY);
            } else {
                RebuildGrid(tiles, gridCols, gridRows, areaWidth, areaHeight, offsetX, offsetY);
            }
            prevCols = gridCols; prevRows = gridRows;
        } else {
            // update rect positions on resize
            float tileWidth = static_cast<float>(areaWidth) / gridCols;
            float tileHeight = static_cast<float>(areaHeight) / gridRows;
            for (int y = 0; y < gridRows; y++) {
                for (int x = 0; x < gridCols; x++) {
                    int i = y * gridCols + x;
                    tiles[i].rect = { offsetX + x * tileWidth, offsetY + y * tileHeight, tileWidth, tileHeight };
                }
            }
        }

        // animal selection keys
        if (IsKeyPressed(KEY_ONE)) selectedAnimal = Selected::COW;
        if (IsKeyPressed(KEY_TWO)) selectedAnimal = Selected::SHEEP;
        if (IsKeyPressed(KEY_THREE)) selectedAnimal = Selected::CHICKEN;

        // placing/removing animals
        if (IsKeyPressed(KEY_ONE)) selectedAnimal = Selected::COW;
    if (IsKeyPressed(KEY_TWO)) selectedAnimal = Selected::SHEEP;
    if (IsKeyPressed(KEY_THREE)) selectedAnimal = Selected::CHICKEN;
    if (IsKeyPressed(KEY_FOUR)) selectedAnimal = Selected::PIG;

    // placing/removing animals
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        for (auto& tile : tiles) {
            if (CheckCollisionPointRec(mouse, tile.rect)) {
                if (tile.animal) {
                    tile.animal.reset(); // remove animal
                } else {
                    switch (selectedAnimal) {
                        case Selected::COW: tile.animal = std::make_unique<Cow>(cowTex); break;
                        case Selected::SHEEP: tile.animal = std::make_unique<Sheep>(sheepTex); break;
                        case Selected::CHICKEN: tile.animal = std::make_unique<Chicken>(chickenTex); break;
                        case Selected::PIG: tile.animal = std::make_unique<Pig>(pigTex); break;
                    }
                }
            }
        }
    }

        // drawing
        BeginDrawing();
        ClearBackground(DARKGREEN);

        // gray bars
        DrawRectangle(0, 0, winWidth, offsetY, GRAY);
        DrawRectangle(0, winHeight - offsetY, winWidth, offsetY, GRAY);
        DrawRectangle(0, offsetY, offsetX, areaHeight, GRAY);
        DrawRectangle(winWidth - offsetX, offsetY, offsetX, areaHeight, GRAY);

        DrawRectangleLines(offsetX, offsetY, areaWidth, areaHeight, BLACK);

        for (auto& tile : tiles) {
        DrawRectangleRec(tile.rect, DARKGREEN);
        DrawRectangleLinesEx(tile.rect, 1, BLACK);
        if (tile.animal) {
            tile.animal->draw(tile.rect.x, tile.rect.y, tile.rect.width, tile.rect.height);
        }
}


        // UI
        DrawRectangle(0, 0, winWidth, 70, Fade(BLACK, 0.6f));
        DrawText("BetterFarm++ OOP (16:10)", 20, 10, 24, RAYWHITE);
        DrawText("1:Cow  2:Sheep  3:Chicken  4:Pig  LMB:Place/Remove", 20, 40, 20, RAYWHITE);
        DrawText("Arrow Up/Down: Resize Grid", 20, 60, 20, RAYWHITE);


        EndDrawing();
    }

    UnloadTexture(cowTex);
    UnloadTexture(sheepTex);
    UnloadTexture(chickenTex);

    CloseWindow();
    return 0;
}
