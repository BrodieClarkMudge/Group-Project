#include "raylib.h"
#include <vector>
#include <string>

enum class AnimalType { NONE, COW, SHEEP, CHICKEN };

struct Tile {
    Rectangle rect;
    AnimalType animal = AnimalType::NONE;
};

//creates animals
void DrawAnimal(AnimalType type, 
                const Rectangle& rect, //plain tile
                const Texture2D& cowTex,
                const Texture2D& sheepTex,
                const Texture2D& chickenTex)
{
    float cx = rect.x + rect.width / 2.0f;
    float cy = rect.y + rect.height / 2.0f;
    float size = rect.width * 0.9f; //fit image inside tile

    Texture2D tex{};
    switch (type) {
        case AnimalType::COW: tex = cowTex; break;
        case AnimalType::SHEEP: tex = sheepTex; break;
        case AnimalType::CHICKEN: tex = chickenTex; break;
        default: return;
    }

    float scale = size / tex.width;
    DrawTextureEx(tex, { cx - tex.width * scale / 2.0f, cy - tex.height * scale / 2.0f },
                  0.0f, scale, WHITE);
}

//rebuilds grid
void RebuildGrid(std::vector<Tile>& tiles, int cols, int rows,
                 int areaWidth, int areaHeight, int offsetX, int offsetY)
{
    std::vector<AnimalType> oldAnimals;
    oldAnimals.reserve(tiles.size());
    for (const auto& t : tiles) oldAnimals.push_back(t.animal);

    tiles.clear();
    float tileWidth = static_cast<float>(areaWidth) / cols;
    float tileHeight = static_cast<float>(areaHeight) / rows;

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            Tile t;
            t.rect = { offsetX + x * tileWidth, offsetY + y * tileHeight, tileWidth, tileHeight };
            int index = y * cols + x;
            if (index < (int)oldAnimals.size())
                t.animal = oldAnimals[index];
            tiles.push_back(t);
        }
    }
}

//expands grid but keeps animal place
void ExpandGrid(std::vector<Tile>& tiles, int oldCols, int oldRows, int newCols, int newRows,
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
                // Copy existing animal
                t.animal = tiles[y * oldCols + x].animal;
            } else {
                t.animal = AnimalType::NONE; // new tile
            }

            newTiles.push_back(t);
        }
    }

    tiles = std::move(newTiles);
}



int main() {
    const int initialWidth = 1280;
    const int initialHeight = 800;
    const float aspectRatio = 16.0f / 10.0f;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(640, 400);
    InitWindow(initialWidth, initialHeight, "BetterFarm++ test");

    int gridCols = 16;
    int gridRows = 10;
    const int minGrid = 10;
    const int maxGrid = 24;

    Texture2D cowTex = LoadTexture("assets/cowPixel.png");
    Texture2D sheepTex = LoadTexture("assets/sheepPixel.png");
    Texture2D chickenTex = LoadTexture("assets/chickenPixel.png");

    AnimalType selectedAnimal = AnimalType::COW;
    std::vector<Tile> tiles;

    int prevCols = gridCols;
    int prevRows = gridRows;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        int winWidth = GetScreenWidth();
        int winHeight = GetScreenHeight();

        //stops user from making window too small
        if (winWidth < 800 || winHeight < 500) {
            SetWindowSize((winWidth < 800) ? 800 : winWidth, (winHeight < 500) ? 500 : winHeight);
        }

        //maintain aspect ratio (roughly)
        int areaWidth = winWidth;
        int areaHeight = static_cast<int>(winWidth / aspectRatio);
        if (areaHeight > winHeight) {
            areaHeight = winHeight;
            areaWidth = static_cast<int>(winHeight * aspectRatio);
        }

        int offsetX = (winWidth - areaWidth) / 2;
        int offsetY = (winHeight - areaHeight) / 2;

        // --- Grid resizing controls ---
        if (IsKeyPressed(KEY_UP) && gridRows < maxGrid && gridCols < maxGrid) {
            gridRows += 2;
            gridCols += 3;
        }
        if (IsKeyPressed(KEY_DOWN) && gridRows > minGrid && gridCols > minGrid) {
            gridRows -= 2;
            gridCols -= 3;
        }

        // --- Rebuild grid only if size changed ---
        if (gridCols != prevCols || gridRows != prevRows || tiles.empty()) {
            if (!tiles.empty()) {
                ExpandGrid(tiles, prevCols, prevRows, gridCols, gridRows, areaWidth, areaHeight, offsetX, offsetY);
            } else {
                RebuildGrid(tiles, gridCols, gridRows, areaWidth, areaHeight, offsetX, offsetY);
            }
            prevCols = gridCols;
            prevRows = gridRows;

        } else {
            // Update tile positions for resizing window (no reset)
            float tileWidth = static_cast<float>(areaWidth) / gridCols;
            float tileHeight = static_cast<float>(areaHeight) / gridRows;
            for (int y = 0; y < gridRows; y++) {
                for (int x = 0; x < gridCols; x++) {
                    int i = y * gridCols + x;
                    tiles[i].rect = { offsetX + x * tileWidth, offsetY + y * tileHeight, tileWidth, tileHeight };
                }
            }
        }

        // --- Animal selection ---
        if (IsKeyPressed(KEY_ONE)) selectedAnimal = AnimalType::COW;
        if (IsKeyPressed(KEY_TWO)) selectedAnimal = AnimalType::SHEEP;
        if (IsKeyPressed(KEY_THREE)) selectedAnimal = AnimalType::CHICKEN;

        // --- Mouse click interaction ---
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            for (auto& tile : tiles) {
                if (CheckCollisionPointRec(mouse, tile.rect)) {
                    tile.animal = (tile.animal == selectedAnimal) ? AnimalType::NONE : selectedAnimal;
                }
            }
        }

        // --- Drawing ---
        BeginDrawing();
        ClearBackground(DARKGREEN);

        // Gray bars (letterbox)
        DrawRectangle(0, 0, winWidth, offsetY, GRAY);
        DrawRectangle(0, winHeight - offsetY, winWidth, offsetY, GRAY);
        DrawRectangle(0, offsetY, offsetX, areaHeight, GRAY);
        DrawRectangle(winWidth - offsetX, offsetY, offsetX, areaHeight, GRAY);

        // Draw grid area border
        DrawRectangleLines(offsetX, offsetY, areaWidth, areaHeight, BLACK);

        // Draw tiles and animals
        for (auto& tile : tiles) {
            DrawRectangleRec(tile.rect, DARKGREEN);
            DrawRectangleLinesEx(tile.rect, 1, BLACK);
            if (tile.animal != AnimalType::NONE) {
                DrawAnimal(tile.animal, tile.rect, cowTex, sheepTex, chickenTex);
            }
        }


        // --- UI ---
        DrawRectangle(0, 0, winWidth, 70, Fade(BLACK, 0.6f));

        std::string title = "BetterFarm++ - Persistent Animal Grid (16:10)";
        DrawText(title.c_str(), 20, 10, 24, RAYWHITE);

        std::string gridInfo = std::to_string(gridCols) + " x " + std::to_string(gridRows) +
            "  (UP/DOWN to resize)";
        DrawText(gridInfo.c_str(), 20, 40, 18, RAYWHITE);

        DrawText("Press [1] Cow, [2] Sheep, [3] Chicken | Click tiles to place/remove",
                 20, 65, 18, RAYWHITE);

        EndDrawing();

        
    }

    UnloadTexture(cowTex);
    UnloadTexture(sheepTex);
    UnloadTexture(chickenTex);
    
    CloseWindow();

    return 0;
}
