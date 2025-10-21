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
    FarmTextures tex;
    Tile tile;
    Menu menu;
    Options option;

    menu.ShowMenu(menu);
    
    if (menu.getSoundFX()) {
        option.setSoundFXTrue();
    } else {
        option.setSoundFXFalse();
    }

    std::vector<Tile> tiles;

    // ----------------------
    // UI positions
    // ----------------------
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
        if (shop.getExpanding() == true && tile.getGridRows() < tile.getMaxGrid() && tile.getGridCols() < tile.getMaxGrid()) {
            tile.setGridRows(tile.getGridRows() + 2); tile.setGridCols(tile.getGridCols() + 3);
            shop.setExpandingFalse();
        }

        if (tile.getGridCols() != tile.getPrevCols() || tile.getGridRows() != tile.getPrevRows() || tiles.empty()) {
            if (!tiles.empty()) {
                tile.ExpandGrid(tiles, tile.getPrevCols(), tile.getPrevRows(), tile.getGridCols(), tile.getGridRows(), areaWidth, areaHeight, offsetX, offsetY);
            } else {
                tile.RebuildGrid(tiles, tile.getGridCols(), tile.getGridRows(), areaWidth, areaHeight, offsetX, offsetY);
            }
            tile.setPrevCols(tile.getGridCols()); tile.setPrevRows(tile.getGridRows());
        } else {
            // Update rect positions on resize
            float tileWidth  = static_cast<float>(areaWidth) / tile.getGridCols();
            float tileHeight = static_cast<float>(gridHeight) / tile.getGridRows();

            for (int y = 0; y < tile.getGridRows(); y++) {
                for (int x = 0; x < tile.getGridCols(); x++) {
                    int i = y * tile.getGridCols() + x;
                    tiles[i].setRect({ offsetX + x * tileWidth, offsetY + y * tileHeight, tileWidth, tileHeight });
                }
            }
        }

    tile.setTimeChange();

    for (auto& tile : tiles) {
        if (tile.hasAnimal()) {
            tile.getAnimal()->updateSoundTimer(tile.getTimeChange());
        }
        if (tile.hasAnimal() && IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            tile.getAnimal()->makeSound();
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
                if (CheckCollisionPointRec(mouse, tile.getRect())) {
                    if (shop.getSelectedItem() == "YARD" && coins >= 20 & tile.getType() != TileType::YARD) {
                        tile.setType(TileType::YARD);
                        coins -= 20;
                    } else if (shop.getSelectedItem() == "COW" && coins >= 50 && tile.getType() == TileType::YARD && !tile.getAnimal()) {
                        tile.setAnimal(std::make_unique<Cow>(tex.getCowTex(),tex.getCowSound()));
                        coins -= 50;
                    } else if (shop.getSelectedItem() == "SHEEP" && coins >= 40 && tile.getType() == TileType::YARD && !tile.getAnimal()) {
                        tile.setAnimal(std::make_unique<Sheep>(tex.getSheepTex(), tex.getSheepSound()));
                        coins -= 40;
                    } else if (shop.getSelectedItem() == "CHICKEN" && coins >= 30 && tile.getType()== TileType::YARD && !tile.getAnimal()) {
                        tile.setAnimal(std::make_unique<Chicken>(tex.getChickenTex(), tex.getChickenSound()));
                        coins -= 30;
                    } else if (shop.getSelectedItem() == "PIG" && coins >= 60 && tile.getType() == TileType::YARD && !tile.getAnimal()) {
                        tile.setAnimal(std::make_unique<Pig>(tex.getPigTex(), tex.getPigSound()));
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
        DrawTexturePro(tex.getHoeTex(), Rectangle{0,0, (float)tex.getHoeTex().width, (float)tex.getHoeTex().height},
            hoe, Vector2{0, 0}, 0.0f, WHITE
        );

        Rectangle shopUI = { topBar.x + 230, topBar.y + 10, 40, 40};
        DrawRectangleLinesEx(shopUI, 2, BROWN);
        DrawTexturePro(tex.getShopTex(), Rectangle{-10,-10, (float)tex.getShopTex().width+20, (float)tex.getShopTex().height+20},
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
            if (tile.getType() == TileType::GRASS) {
                DrawTexturePro(tex.getGrassTex(), Rectangle{0, 0, (float)tex.getGrassTex().width, (float)tex.getGrassTex().height},
                tile.getRect(), Vector2{0, 0}, 0.0f, WHITE);
            } else if (tile.getType() == TileType::YARD) {
                DrawTexturePro(tex.getYardTex(), Rectangle{0, 0, (float)tex.getYardTex().width, (float)tex.getYardTex().height},
                tile.getRect(), Vector2{0, 0}, 0.0f, WHITE);
            } else if (tile.getType() == TileType::HOED) {
                DrawTexturePro(tex.getHoedTex(), Rectangle{0, 0, (float)tex.getHoedTex().width, (float)tex.getHoedTex().height},
                tile.getRect(), Vector2{0, 0}, 0.0f, WHITE);
            }

            DrawRectangleLinesEx(tile.getRect(), 1, BLACK);
            if (tile.hasAnimal()) {
                tile.getAnimal()->draw(tile.getRect().x, tile.getRect().y, tile.getRect().width, tile.getRect().height);
            }
        }

        Vector2 mousePos = GetMousePosition();

        for (auto& tile : tiles) {
            if (tile.getAnimal() && CheckCollisionPointRec(mousePos, tile.getRect())) {
                // Get the animal's state string
                std::string stateText = tile.getAnimal()->getState();

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

        shop.Update(tiles, coins, tex.getYardTex(), tex.getCowTex(), tex.getSheepTex(), tex.getChickenTex(), tex.getPigTex());
        shop.Draw();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, hoe) && option.getOptionsOpen() == false && shop.getPlacing() == false && shop.getOpen() == false) {
            ui.hoeing = !ui.hoeing;
        }

        if(ui.hoeing == true && coins >= 5 && shop.getPlacing() == false) {
            DrawTexturePro(
                tex.getHoeTex(),
                Rectangle{0, 0, (float)tex.getHoeTex().width, (float)tex.getHoeTex().height},
                Rectangle{mousePos.x, mousePos.y, 80, 80}, Vector2{80 / 2.0f, 80 / 2.0f}, 0.0f, WHITE
            );
            for (auto &tile : tiles) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && tile.getType() == TileType::GRASS && CheckCollisionPointRec(mousePos, tile.getRect())) {
                     tile.setType(TileType::HOED);
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

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
