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

#include <vector>
#include <string>
#include <memory>


int main() {
    const int initialWidth = 1280;
    const int initialHeight = 800;
    const float aspectRatio = 16.0f / 10.0f;
    const int barHeight = 60;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
 
    InitWindow(initialWidth, initialHeight, "BetterFarm++ OOP (unique_ptr)");
    SetWindowMinSize(640, 400);

    InitAudioDevice();

    SetTargetFPS(60);

    Shop shop;
    UIState ui;

    // ----------------------
    // Show main menu
    // ----------------------
    Menu menu;
    menu.ShowMenu(menu);
    Options option;
    
    if (menu.getSoundFX()) {
        option.setSoundFXTrue();
    } else {
        option.setSoundFXFalse();
    }

    int gridCols = 17, gridRows = 10;
    const int minGrid = 10, maxGrid = 25;

    enum class Selected { COW, SHEEP, CHICKEN, PIG };
    Selected selectedAnimal = Selected::COW;

    FarmTextures tex = LoadFarmTextures();

    std::vector<Tile> tiles;
    int prevCols = gridCols, prevRows = gridRows;

    // ----------------------
    // UI positions
    // ----------------------
    const int uiMargin = 10;
    int coins = 10000;
    int currentDay = 1;

    while (!WindowShouldClose()) {
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
        if (shop.getExpanding() == true && gridRows < maxGrid && gridCols < maxGrid) {
            gridRows += 2; gridCols += 3;
            shop.setExpandingFalse();
        }

        if (gridCols != prevCols || gridRows != prevRows || tiles.empty()) {
            if (!tiles.empty()) {
                ExpandGrid(tiles, prevCols, prevRows, gridCols, gridRows, areaWidth, areaHeight, offsetX, offsetY);
            } else {
                RebuildGrid(tiles, gridCols, gridRows, areaWidth, areaHeight, offsetX, offsetY);
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


        float timeChange = GetFrameTime(); // time since last frame

    for (auto& tile : tiles) {
        if (tile.animal) {
            tile.animal->updateSoundTimer(timeChange);
        }
        if (tile.animal && IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            tile.animal->makeSound();
        }
    }

    

        // Inside your main loop, after handling other inputs:
    // Handle placement mode
    if (shop.getPlacing() && shop.getSelectedItem() != "") {
        Vector2 mouse = GetMousePosition();

        // Exit placement mode
        if (IsKeyPressed(KEY_ENTER)) {
            shop.setPlacingFalse();
            shop.setSelectedItem("");
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (auto& tile : tiles) {
                if (CheckCollisionPointRec(mouse, tile.rect)) {
                    if (shop.getSelectedItem() == "YARD" && coins >= 20 & tile.type != TileType::YARD) {
                        tile.type = TileType::YARD;
                        coins -= 20;
                    } else if (shop.getSelectedItem() == "COW" && coins >= 50 && tile.type == TileType::YARD && !tile.animal) {
                        tile.animal = std::make_unique<Cow>(tex.cowTex,tex.cowSound);
                        coins -= 50;
                    } else if (shop.getSelectedItem() == "SHEEP" && coins >= 40 && tile.type == TileType::YARD && !tile.animal) {
                        tile.animal = std::make_unique<Sheep>(tex.sheepTex, tex.sheepSound);
                        coins -= 40;
                    } else if (shop.getSelectedItem() == "CHICKEN" && coins >= 30 && tile.type == TileType::YARD && !tile.animal) {
                        tile.animal = std::make_unique<Chicken>(tex.chickenTex, tex.chickenSound);
                        coins -= 30;
                    } else if (shop.getSelectedItem() == "PIG" && coins >= 60 && tile.type == TileType::YARD && !tile.animal) {
                        tile.animal = std::make_unique<Pig>(tex.pigTex, tex.pigSound);
                        coins -= 60;
                    }

                    break; // place only once per click
                }
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

        Rectangle shopUI = { topBar.x + 230, topBar.y + 10, 40, 40};
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
            if (tile.animal) {
                tile.animal->draw(tile.rect.x, tile.rect.y, tile.rect.width, tile.rect.height);
            }
        }

        Vector2 mousePos = GetMousePosition();

        for (auto& tile : tiles) {
            if (tile.animal && CheckCollisionPointRec(mousePos, tile.rect)) {
                // Get the animal's state string
                std::string stateText = tile.animal->getState();

                // Determine tooltip position
                float textWidth = MeasureText(stateText.c_str(), 14);
                float textHeight = 18;
                float tooltipX = mousePos.x + 10;
                float tooltipY = mousePos.y + 10;

                // Optional: prevent tooltip from going off screen
                if (tooltipX + textWidth > GetScreenWidth()) tooltipX = GetScreenWidth() - textWidth - 10;
                if (tooltipY + textHeight > GetScreenHeight()) tooltipY = GetScreenHeight() - textHeight - 10;

                // Draw tooltip background
                DrawRectangle(tooltipX - 4, tooltipY - 4, textWidth + 8, textHeight + 8, Fade(BLACK, 0.8f));

                // Draw text
                DrawText(stateText.c_str(), tooltipX, tooltipY, 14, WHITE);
            }
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, optionsBackground) && 
        ui.hoeing == false && shop.getOpen() == false && shop.getPlacing() == false) {
            option.ShowOptions(option);
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, shopUI) && option.getOptionsOpen() == false && ui.hoeing == false) {
            shop.Open();
        }

        shop.Update(tiles, coins, tex.yardTex, tex.cowTex, tex.sheepTex, tex.chickenTex, tex.pigTex);
        shop.Draw();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, hoe) && option.getOptionsOpen() == false && shop.getPlacing() == false && shop.getOpen() == false) {
            ui.hoeing = !ui.hoeing;
        }

        if(ui.hoeing == true && coins >= 5 && shop.getPlacing() == false) {
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

        if(shop.getPlacing()) {
            Rectangle placingShop = { topBar.x + 300, topBar.y + 10, 290, 40 };
            DrawRectangleRec(placingShop, GOLD);
            DrawRectangleLinesEx(placingShop, 2, BROWN);
            DrawText("PLACING - to exit ENTER", (int)placingShop.x + 10, (int)placingShop.y + 10, 20, DARKPURPLE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos,placingShop)) {
                shop.setPlacingFalse();
            }
        }

        EndDrawing();
        
    }

    // ----------------------
    // Cleanup
    // ----------------------
    UnloadFarmTextures(tex);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
