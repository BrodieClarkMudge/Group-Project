#include "raylib.h"
#include "grid.h"
#include "animal.h"
#include "chicken.h"
#include "cow.h"
#include "sheep.h"
#include "pig.h"
#include "menu.h"
#include "options.h"
#include "shop.h"
#include "ui.h"
// Crops
#include "Crop.h"
#include "Regenerating.h"
#include "Stalk.h"
#include "Large.h"
#include "Berry.h"
#include "Potato.h"
#include "Pumpkin.h"

#include <vector>
#include <string>
#include <memory>

// Timing global variables to be used in animals and crops
float dt = 0.0; // delta time
float timeScale = 1.0; // 




// ----------------------
// Tile holds unique_ptr<Animal>
// ----------------------




// ----------------------
// Main
// ----------------------
int main() {
    const int initialWidth = 1280;
    const int initialHeight = 800;
    const float aspectRatio = 16.0f / 10.0f;
    const int barHeight = 60;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
 
    InitWindow(initialWidth, initialHeight, "BetterFarm++ OOP (unique_ptr)");
    SetWindowMinSize(640, 400);

    SetTargetFPS(60);

    bool hoeing = false;
    Shop shop;

    // ----------------------
    // Show main menu
    // ----------------------
    MainMenu menu;
    ShowMainMenu(menu);

    timeScale = menu.timeScale; // updates global time scale based on the slider within the main menu.

    int gridCols = 17, gridRows = 10;
    const int minGrid = 10, maxGrid = 25;

    enum class Selected { COW, SHEEP, CHICKEN, PIG };
    Selected selectedAnimal = Selected::COW;

    // crops
    enum class SelectedCrop { BERRY, POTATO, PUMPKIN };
    SelectedCrop selectedCrop = SelectedCrop::BERRY;



    FarmTextures tex = LoadFarmTextures();

// Load textures
    Texture2D grassTex = LoadTexture("assets/grassPixel.png");
    Texture2D yardTex = LoadTexture("assets/yardPixel.png");
    Texture2D hoedTex = LoadTexture("assets/hoedPixel.png");

    Texture2D coinTex = LoadTexture("assets/coinPixel.png");
   // Texture2D hoeTex = LoadTexture("assets/hoePixel.png");
    // Texture2D shopTex = LoadTexture("assets/shopPixel.png");

    Texture2D cowTex = LoadTexture("assets/cowPixel.png");
    Texture2D sheepTex = LoadTexture("assets/sheepPixel.png");
    Texture2D chickenTex = LoadTexture("assets/chickenPixel.png");
    Texture2D pigTex = LoadTexture("assets/pigPixel.png")
    
    
    ;

    std::vector<Tile> tiles;
    int prevCols = gridCols, prevRows = gridRows;

    // ----------------------
    // UI positions
    // ----------------------
    const int uiMargin = 10;
    int coins = 150;
    int currentDay = 1;


    
    while (!WindowShouldClose()) {
        // calculates time to be used in animals and crops
        // GetFrameTime gives total time since window was initialised.
        dt = GetFrameTime() * timeScale; 

        // Get current screen size
        int winWidth = GetScreenWidth();
        int winHeight = GetScreenHeight();

        // Prevent too small
        if (winWidth < 800 || winHeight < 500) {
            SetWindowSize((winWidth < 800) ? 800 : winWidth,
                          (winHeight < 500) ? 500 : winHeight);
            winWidth = GetScreenWidth();
            winHeight = GetScreenHeight();
        }

        int availableWidth  = winWidth;
        int availableHeight = winHeight;

        // First try: fit width
        int areaWidth = availableWidth;
        int areaHeight = (int)(areaWidth / aspectRatio);

        // If too tall for window, fit by height instead
        if (areaHeight > availableHeight) {
            areaHeight = availableHeight;
            areaWidth  = (int)(areaHeight * aspectRatio);
        }

        // Now split between bar + grid
        int gridHeight = areaHeight - barHeight;

        // Offsets to center horizontally + vertically
        int offsetX = (winWidth - areaWidth) / 2;
        int offsetY = (winHeight - areaHeight) / 2 + barHeight; // grid starts below bar



        // ----------------------
        // Grid controls
        // ----------------------
        if (IsKeyPressed(KEY_UP) && gridRows < maxGrid && gridCols < maxGrid) {
            gridRows += 2; gridCols += 3;
        }
        if (IsKeyPressed(KEY_DOWN) && gridRows > minGrid && gridCols > minGrid) {
            gridRows -= 2; gridCols -= 3;
        }

        if (gridCols != prevCols || gridRows != prevRows || tiles.empty()) {
            if (!tiles.empty()) {
                ExpandGrid(tiles, prevCols, prevRows, gridCols, gridRows, areaWidth, gridHeight, offsetX, offsetY);
            } else {
                RebuildGrid(tiles, gridCols, gridRows, areaWidth, gridHeight, offsetX, offsetY);
            }
            prevCols = gridCols; prevRows = gridRows;
        } else {
            // Update rect positions on resize
            float tileWidth  = static_cast<float>(areaWidth) / gridCols;
            float tileHeight = static_cast<float>(gridHeight) / gridRows;

            for (int y = 0; y < gridRows; y++) {
                for (int x = 0; x < gridCols; x++) {
                    int i = y * gridCols + x;
                    tiles[i].rect = { offsetX + x * tileWidth, offsetY + y * tileHeight, tileWidth, tileHeight };
                }
            }
        }


        if (IsKeyPressed(KEY_ONE)) selectedAnimal = Selected::COW;
        if (IsKeyPressed(KEY_TWO)) selectedAnimal = Selected::SHEEP;
        if (IsKeyPressed(KEY_THREE)) selectedAnimal = Selected::CHICKEN;
        if (IsKeyPressed(KEY_FOUR)) selectedAnimal = Selected::PIG;

        // crops
        if (IsKeyPressed(KEY_FIVE))   selectedCrop = SelectedCrop::BERRY;
        if (IsKeyPressed(KEY_SIX))    selectedCrop = SelectedCrop::POTATO;
        if (IsKeyPressed(KEY_SEVEN))  selectedCrop = SelectedCrop::PUMPKIN;



        // ANIMAL BRANCH
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            for (auto& tile : tiles) {
                if (CheckCollisionPointRec(mouse, tile.rect) && hoeing == false) {
                    if (tile.type == TileType::YARD) {
                        if (tile.animal) {
                            tile.animal.reset();
                        } else {
                            switch (selectedAnimal) {
                                // make_unique makes a unique smart pointer.
                                case Selected::COW:     tile.animal = std::make_unique<Cow>(cowTex);     break;
                                case Selected::SHEEP:   tile.animal = std::make_unique<Sheep>(sheepTex); break;
                                case Selected::CHICKEN: tile.animal = std::make_unique<Chicken>(chickenTex); break;
                                case Selected::PIG:     tile.animal = std::make_unique<Pig>(pigTex);     break;
                            }
                        }
                    }
                }
            }
        }

        // CROP BRANCH
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            // iterate over all tiles
            for (auto& tile : tiles) {
                // not in hoe mode, clicked on a hoed tile,
                if (CheckCollisionPointRec(mouse, tile.rect) && hoeing == false && tile.type == TileType::HOED) {
                    // is there a crop already
                    if (tile.crop) {
                        // Harvest only if mature
                        if (tile.crop->IsMature()) {
                            int payout = 0; // NEEDS TO BE UPDATED SOON ---------------------------------------
                            bool remove = tile.crop->Harvest(payout);
                            coins += payout;                 // add coins
                            if (remove) tile.crop.reset();   // crops marked for deletion are removed
                            tile.cropTimer = 0.0f;           // reset growth timer (so regen crops are reset)
                        }
                        // If not mature: do nothing


                       // NO CROP SELECTED 
                    } else {
                        // Plant selected crop
                        switch (selectedCrop) {
                            case SelectedCrop::BERRY: tile.crop = std::make_unique<Berry>(0.0); // maybe remove this param from class?
                            break;
                            case SelectedCrop::POTATO: tile.crop = std::make_unique<Potato>(0.0);
                            break;
                            case SelectedCrop::PUMPKIN: tile.crop = std::make_unique<Pumpkin>(0.0); 
                            break;
                        }

                        // PLACEHOLDERS -------------------------------------------
                        // Stage textures
                        tile.crop->SetTexture(SEED,  hoedTex);  // dirt
                        tile.crop->SetTexture(SEMI1, yardTex);
                        tile.crop->SetTexture(SEMI2, grassTex);
                        tile.crop->SetTexture(FULL,  coinTex); 

                        tile.cropTimer = 0.0f; // start growth clock after planting.
                    }
                }
            }
        }







// Shop
if (!shop.open && !shop.selectedItem.empty()) {
    Vector2 mouse = GetMousePosition();
    for (auto& tile : tiles) {
        if (CheckCollisionPointRec(mouse, tile.rect)) {
            if (shop.selectedItem == "YARD" && tile.type == TileType::GRASS && coins >= 20) {
                tile.type = TileType::HOED;
                coins -= 20;
            }
            else if (shop.selectedItem == "COW" && tile.type == TileType::YARD && !tile.animal && coins >= 50) {
                tile.animal = std::make_unique<Cow>(tex.cowTex);
                coins -= 50;
            }
            else if (shop.selectedItem == "SHEEP" && tile.type == TileType::YARD && !tile.animal && coins >= 40) {
                tile.animal = std::make_unique<Sheep>(tex.sheepTex);
                coins -= 40;
            }
            else if (shop.selectedItem == "CHICKEN" && tile.type == TileType::YARD && !tile.animal && coins >= 30) {
                tile.animal = std::make_unique<Chicken>(tex.chickenTex);
                coins -= 30;
            }
            else if (shop.selectedItem == "PIG" && tile.type == TileType::YARD && !tile.animal && coins >= 60) {
                tile.animal = std::make_unique<Pig>(tex.pigTex);
                coins -= 60;
            }

            shop.selectedItem.clear(); // item placed, reset selection
            break;
        }
    }
}

        BeginDrawing();
        ClearBackground(BLACK); // everything outside grid+bar is black



        Rectangle topBar = { (float)offsetX, (float)(offsetY - barHeight), (float)areaWidth, (float)barHeight };
        DrawRectangleRec(topBar, GOLD);
        DrawRectangleLinesEx(topBar, 2, BROWN);

        // Options (top-left inside bar)
        Rectangle optionsBackground = { topBar.x + 10, topBar.y + 10, 120, 40 };
        DrawRectangleRec(optionsBackground, GOLD);
        DrawRectangleLinesEx(optionsBackground, 2, BROWN);
        DrawText("Options", (int)optionsBackground.x + 10, (int)optionsBackground.y + 10, 20, BLACK);

        //hoe rectangle
        Rectangle hoe = { topBar.x + 160, topBar.y + 10, 40, 40};
        DrawRectangleLinesEx(hoe, 2, BROWN);
        DrawTexturePro(tex.hoeTex, Rectangle{0,0, (float)tex.hoeTex.width, (float)tex.hoeTex.height},
            hoe, Vector2{0, 0}, 0.0f, WHITE
        );

        Rectangle shopUI = { topBar.x + 300, topBar.y + 10, 40, 40};
        DrawRectangleLinesEx(shopUI, 2, BROWN);
        DrawTexturePro(tex.shopTex, Rectangle{-10,-10, (float)tex.shopTex.width+20, (float)tex.shopTex.height+20},
            shopUI, Vector2{0, 0}, 0.0f, WHITE
        );

        // Day (centered in bar)
        const char* dayText = TextFormat("Day: %d", currentDay);
        int dayTextWidth = MeasureText(dayText, 20);
        int dayX = offsetX + areaWidth/2 - dayTextWidth/2;
        DrawText(dayText, dayX, topBar.y + 20, 20, BLACK);

        // Coins (top-right inside bar)
        const char* coinText = TextFormat("Coins: %d", coins);
        int coinTextWidth = MeasureText(coinText, 20);
        int coinX = offsetX + areaWidth - coinTextWidth - 20;
        DrawText(coinText, coinX, topBar.y + 20, 20, BLACK);



        for (auto& tile : tiles) {
            if (tile.type == TileType::GRASS) {
                DrawTexturePro(tex.grassTex, Rectangle{0, 0, (float)tex.grassTex.width, (float)tex.grassTex.height},
                tile.rect, Vector2{0, 0}, 0.0f, WHITE);
            } else if (tile.type == TileType::YARD) {
                DrawTexturePro(tex.yardTex, Rectangle{0, 0, (float)tex.yardTex.width, (float)tex.yardTex.height},
                tile.rect, Vector2{0, 0}, 0.0f, WHITE);
            } else if (tile.type == TileType::HOED) {
                DrawTexturePro(tex.hoedTex, Rectangle{0, 0, (float)tex.hoedTex.width, (float)tex.hoedTex.height},
                tile.rect, Vector2{0, 0}, 0.0f, WHITE);
            }

            DrawRectangleLinesEx(tile.rect, 1, BLACK);

            // ANIMALS 
            if (tile.animal) {
                tile.animal->draw(tile.rect.x, tile.rect.y, tile.rect.width, tile.rect.height);
            }

            // CROPS
            if (tile.crop) {
                // every ~2 seconds go to next stage
                tile.cropTimer += dt;
                float growEvery = 2.0;
                if (tile.cropTimer >= growEvery) {
                    tile.crop->Grow();
                    tile.cropTimer = 0.0f;
                }
        
                tile.crop->Draw(tile.rect);

            }
        }



        DrawText("1:Cow  2:Sheep  3:Chicken  4:Pig  LMB:Place/Remove, 5:Berry  6:Potato  7:Pumpkin", 20, winHeight - 60, 20, RAYWHITE);
        DrawText("", 20, winHeight - 80, 20, RAYWHITE);

        DrawText("Arrow Up/Down: Resize Grid", 20, winHeight - 40, 20, RAYWHITE);
        DrawText("BetterFarm++ OOP (16:10)", 20, winHeight - 20, 20, RAYWHITE);

        Vector2 mousePos = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, optionsBackground)) {
            Options option;
            ShowOptions(option);
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, shopUI)) {
            shop.Open();
        }

        shop.Update(tiles, coins, tex.yardTex, tex.cowTex, tex.sheepTex, tex.chickenTex, tex.pigTex);
        shop.Draw();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, hoe)) {
            hoeing = !hoeing;
        }

        if(hoeing == true) {
            DrawTexturePro(
                tex.hoeTex,
                Rectangle{0, 0, (float)tex.hoeTex.width, (float)tex.hoeTex.height},
                Rectangle{mousePos.x, mousePos.y, 80, 80}, Vector2{80 / 2.0f, 80 / 2.0f}, 0.0f, WHITE
            );
            for (auto &tile : tiles) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && tile.type == TileType::GRASS && CheckCollisionPointRec(mousePos, tile.rect)) {
                     tile.type = TileType::HOED;
                     coins = coins - 5;
                }
            }
        }

        EndDrawing();
        
    }

    // ----------------------
    // Cleanup
    // ----------------------
    UnloadFarmTextures(tex);

    CloseWindow();
    return 0;
}
