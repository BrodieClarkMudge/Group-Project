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
#include "Potato.h"
#include "Pumpkin.h"
#include "Tomato.h"

#include <vector>
#include <string>
#include <memory>

// Timing global variables to be used in animals and crops
float dt = 0.0f;
float timeScale = 1.0f;
float dehydrateTick = 0.0f;

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
        SetMasterVolume(1.0f);
    } else {
        option.setSoundFXFalse();
        SetMasterVolume(0.0f);
    }

    timeScale = menu.getTimeScale();

    std::vector<Tile> tiles;

    enum class SelectedCrop { TOMATO, POTATO, PUMPKIN };
    SelectedCrop selectedCrop = SelectedCrop::TOMATO;

    // Load crop stage textures
    Texture2D potatoSeed = LoadTexture("assets/1 - Potato Seed.png");
    Texture2D potatoSprout = LoadTexture("assets/2 - Potato Sprout.png");
    Texture2D potatoMid = LoadTexture("assets/3 - Potato Mid.png");
    Texture2D potatoFull = LoadTexture("assets/4 - Potato Full.png");

    Texture2D pumpkinSeed = LoadTexture("assets/1 - Pumpkin Seed.png");
    Texture2D pumpkinSprout = LoadTexture("assets/2 - Pumpkin Sprout.png");
    Texture2D pumpkinMid = LoadTexture("assets/3 - Pumpkin Mid.png");
    Texture2D pumpkinFull = LoadTexture("assets/4 - Pumpkin Full.png");

    Texture2D tomatoSeed = LoadTexture("assets/1 - Tomato Seed.png");
    Texture2D tomatoSprout = LoadTexture("assets/2 - Tomato Sprout.png");
    Texture2D tomatoMid = LoadTexture("assets/3 - Tomato Mid.png");
    Texture2D tomatoFull = LoadTexture("assets/4 - Tomato Full.png");

    Texture2D waterCanTex = LoadTexture("assets/waterCanPixel.png");

    // ----------------------
    // UI positions
    // ----------------------
    int coins = 10000;
    int water = 1000;
    int currentDay = 1;
    bool watering = false;

    while (!WindowShouldClose()) {
        // Calculates time to be used in animals and crops
        dt = GetFrameTime() * timeScale;

        // Thirst drain over time
        static float thirstTick = 0.0f;
        thirstTick += dt;
        if (thirstTick >= 0.7f) {
            for (auto& t : tiles) {
                if (t.hasAnimal()) {
                    int thirst = t.getAnimal()->getThirst() - 2;
                    if (thirst < 0) thirst = 0;
                    t.getAnimal()->setThirst(thirst);
                }
            }
            thirstTick = 0.0f;
        }

        // Apply health damage to dehydrated animals once per second
        dehydrateTick += dt;
        if (dehydrateTick >= 0.7f) {
            for (auto& t : tiles) {
                if (t.hasAnimal() && t.getAnimal()->getThirst() == 0) {
                    t.getAnimal()->calculateHealth();
                    t.getAnimal()->checkSurvival();
                    if (!t.getAnimal()->isAlive()) {
                        t.removeAnimal();
                    }
                }
            }
            dehydrateTick = 0.0f;
        }

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
            tile.setGridRows(tile.getGridRows() + 2); 
            tile.setGridCols(tile.getGridCols() + 3);
            shop.setExpandingFalse();
        }

        if (tile.getGridCols() != tile.getPrevCols() || tile.getGridRows() != tile.getPrevRows() || tiles.empty()) {
            if (!tiles.empty()) {
                tile.ExpandGrid(tiles, tile.getPrevCols(), tile.getPrevRows(), tile.getGridCols(), tile.getGridRows(), areaWidth, gridHeight, offsetX, offsetY);
            } else {
                tile.RebuildGrid(tiles, tile.getGridCols(), tile.getGridRows(), areaWidth, gridHeight, offsetX, offsetY);
            }
            tile.setPrevCols(tile.getGridCols()); 
            tile.setPrevRows(tile.getGridRows());
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

        // Crop selection
        if (IsKeyPressed(KEY_FIVE))   selectedCrop = SelectedCrop::TOMATO;
        if (IsKeyPressed(KEY_SIX))    selectedCrop = SelectedCrop::POTATO;
        if (IsKeyPressed(KEY_SEVEN))  selectedCrop = SelectedCrop::PUMPKIN;

        tile.setTimeChange();

        for (auto& t : tiles) {
            if (t.hasAnimal()) {
                t.getAnimal()->updateSoundTimer(tile.getTimeChange());
            }
            if (t.hasAnimal() && IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && option.getSoundFX()) {
                t.getAnimal()->makeSound();
            }
        }

        // Handle placement mode
        if (shop.getPlacing() && shop.getSelectedItem() != "") {
            Vector2 mouse = GetMousePosition();

            // Exit placement mode
            if (IsKeyPressed(KEY_ENTER)) {
                shop.setPlacingFalse();
                shop.setSelectedItem("");
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                for (auto& t : tiles) {
                    if (CheckCollisionPointRec(mouse, t.getRect())) {
                        if (shop.getSelectedItem() == "YARD" && coins >= 20 && t.getType() != TileType::YARD) {
                            t.setType(TileType::YARD);
                            coins -= 20;
                        } else if (shop.getSelectedItem() == "COW" && coins >= 50 && t.getType() == TileType::YARD && !t.getAnimal()) {
                            t.setAnimal(std::make_unique<Cow>(tex.getCowTex(), tex.getCowSound()));
                            coins -= 50;
                        } else if (shop.getSelectedItem() == "SHEEP" && coins >= 40 && t.getType() == TileType::YARD && !t.getAnimal()) {
                            t.setAnimal(std::make_unique<Sheep>(tex.getSheepTex(), tex.getSheepSound()));
                            coins -= 40;
                        } else if (shop.getSelectedItem() == "CHICKEN" && coins >= 30 && t.getType() == TileType::YARD && !t.getAnimal()) {
                            t.setAnimal(std::make_unique<Chicken>(tex.getChickenTex(), tex.getChickenSound()));
                            coins -= 30;
                        } else if (shop.getSelectedItem() == "PIG" && coins >= 60 && t.getType() == TileType::YARD && !t.getAnimal()) {
                            t.setAnimal(std::make_unique<Pig>(tex.getPigTex(), tex.getPigSound()));
                            coins -= 60;
                        }

                        break; // place only once per click
                    }
                }
            }
        }

        // CROP BRANCH
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            for (auto& t : tiles) {
                if (CheckCollisionPointRec(mouse, t.getRect()) && ui.hoeing == false && watering == false && t.getType() == TileType::HOED) {
                    if (t.hasCrop()) {
                        // Harvest only if mature
                        if (t.getCrop()->IsMature()) {
                            int payout = 0;
                            bool remove = t.getCrop()->Harvest(payout);
                            coins += payout;
                            if (remove) t.removeCrop();
                            t.setCropTimer(0.0f);
                        }
                    } else {
                        // Plant selected crop
                        switch (selectedCrop) {
                            case SelectedCrop::TOMATO:
                                t.setCrop(std::make_unique<Tomato>(0.0));
                                break;
                            case SelectedCrop::POTATO:
                                t.setCrop(std::make_unique<Potato>(0.0));
                                break;
                            case SelectedCrop::PUMPKIN:
                                t.setCrop(std::make_unique<Pumpkin>(0.0));
                                break;
                        }

                        // Assign crop textures based on type
                        switch (selectedCrop) {
                            case SelectedCrop::TOMATO:
                                t.getCrop()->SetTexture(SEED,  tomatoSeed);
                                t.getCrop()->SetTexture(SEMI1, tomatoSprout);
                                t.getCrop()->SetTexture(SEMI2, tomatoMid);
                                t.getCrop()->SetTexture(FULL,  tomatoFull);
                                break;
                            case SelectedCrop::POTATO:
                                t.getCrop()->SetTexture(SEED,  potatoSeed);
                                t.getCrop()->SetTexture(SEMI1, potatoSprout);
                                t.getCrop()->SetTexture(SEMI2, potatoMid);
                                t.getCrop()->SetTexture(FULL,  potatoFull);
                                break;
                            case SelectedCrop::PUMPKIN:
                                t.getCrop()->SetTexture(SEED,  pumpkinSeed);
                                t.getCrop()->SetTexture(SEMI1, pumpkinSprout);
                                t.getCrop()->SetTexture(SEMI2, pumpkinMid);
                                t.getCrop()->SetTexture(FULL,  pumpkinFull);
                                break;
                        }
                        t.setCropTimer(0.0f);
                    }
                }
            }
        }

        // WATERING BRANCH
        if (watering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && water > 0) {
            Vector2 mouse = GetMousePosition();
            for (auto& t : tiles) {
                if (!CheckCollisionPointRec(mouse, t.getRect())) continue;

                bool didWater = false;

                // Animal on yard - water
                if (t.getType() == TileType::YARD && t.hasAnimal()) {
                    water = water - (100 - t.getAnimal()->getThirst());
                    t.getAnimal()->drink();
                    didWater = true;
                }
                // Crop on hoed - water
                else if (t.getType() == TileType::HOED && t.hasCrop()) {
                    water = water - (100 - t.getCrop()->getWaterAmount());
                    t.getCrop()->Water();
                    didWater = true;
                }

                if (didWater) {
                    t.setWateredGlow(2.0f);
                    break;
                }
            }
        }

        // Water border logic before drawing starts
        for (auto& t : tiles) {
            if (t.getWateredGlow() > 0.0f) {
                t.setWateredGlow(t.getWateredGlow() - dt);
                if (t.getWateredGlow() < 0.0f) {
                    t.setWateredGlow(0.0f);
                }
            }
        }

        // Water decay for crops
        for (auto& t : tiles) {
            if (!t.hasCrop()) continue;

            float waterDecayTimer = t.getWaterDecayTimer() + dt;
            t.setWaterDecayTimer(waterDecayTimer);

            while (t.getWaterDecayTimer() >= 0.2f) {
                int w = t.getCrop()->getWaterAmount();
                if (w > 0) {
                    w -= t.getCrop()->getWaterConsumption();
                    if (w < 0) w = 0;
                    t.getCrop()->SetWaterAmount(w);
                }
                t.setWaterDecayTimer(0.0f);
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

        // Hoe rectangle
        Rectangle hoe = { topBar.x + 160, topBar.y + 10, 40, 40};
        DrawRectangleLinesEx(hoe, 2, BROWN);
        DrawTexturePro(tex.getHoeTex(), Rectangle{0, 0, (float)tex.getHoeTex().width, (float)tex.getHoeTex().height},
            hoe, Vector2{0, 0}, 0.0f, WHITE
        );

        // Water button
        Rectangle waterBtn = { topBar.x + 210, topBar.y + 10, 40, 40 };
        DrawRectangleLinesEx(waterBtn, 2, BROWN);
        DrawTexturePro(waterCanTex, {0, 0, (float)waterCanTex.width, (float)waterCanTex.height}, 
            waterBtn, {0, 0}, 0.0f, WHITE);

        // Shop UI
        Rectangle shopUI = { topBar.x + 300, topBar.y + 10, 40, 40};
        DrawRectangleLinesEx(shopUI, 2, BROWN);
        DrawTexturePro(tex.getShopTex(), Rectangle{-10, -10, (float)tex.getShopTex().width + 20, (float)tex.getShopTex().height + 20},
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

        // Water amount
        const char* waterText = TextFormat("Water: %d", water);
        int waterTextWidth = MeasureText(waterText, 20);
        DrawText(waterText, coinX - waterTextWidth - 30, topBar.y + 20, 20, BLACK);

        for (auto& t : tiles) {
            if (t.getType() == TileType::GRASS) {
                DrawTexturePro(tex.getGrassTex(), Rectangle{0, 0, (float)tex.getGrassTex().width, (float)tex.getGrassTex().height},
                t.getRect(), Vector2{0, 0}, 0.0f, WHITE);
            } else if (t.getType() == TileType::YARD) {
                DrawTexturePro(tex.getYardTex(), Rectangle{0, 0, (float)tex.getYardTex().width, (float)tex.getYardTex().height},
                t.getRect(), Vector2{0, 0}, 0.0f, WHITE);
            } else if (t.getType() == TileType::HOED) {
                DrawTexturePro(tex.getHoedTex(), Rectangle{0, 0, (float)tex.getHoedTex().width, (float)tex.getHoedTex().height},
                t.getRect(), Vector2{0, 0}, 0.0f, WHITE);
            }

            DrawRectangleLinesEx(t.getRect(), 1, BLACK);

            // Watering borders
            if (t.getWateredGlow() > 0.0f) {
                DrawRectangleLinesEx(t.getRect(), 2, SKYBLUE);
            }

            // Animals: blue square = not thirsty, red = thirsty about to take dmg
            if (t.hasAnimal()) {
                Rectangle mark = { t.getRect().x + 6, t.getRect().y + 6, 10, 10 };
                if (t.getAnimal()->getThirst() == 0) {
                    DrawRectangleRec(mark, RED);
                } else {
                    DrawRectangleRec(mark, SKYBLUE);
                }
            }

            // Crops: tiny blue square if watered
            if (t.hasCrop() && t.getCrop()->getWaterAmount() != 0) {
                Rectangle drop = { t.getRect().x + t.getRect().width - 16, t.getRect().y + 6, 10, 10 };
                DrawRectangleRec(drop, BLUE);
            }

            // ANIMALS
            if (t.hasAnimal()) {
                t.getAnimal()->draw(t.getRect().x, t.getRect().y, t.getRect().width, t.getRect().height);
            }

            // CROPS
            if (t.hasCrop()) {
                float cropTimer = t.getCropTimer() + dt;
                t.setCropTimer(cropTimer);

                float growEvery = 5.0f;
                if (t.getCropTimer() >= growEvery && t.getCrop()->getWaterAmount() > 0) {
                    t.getCrop()->Grow();
                    t.setCropTimer(0.0f);
                }

                if (t.getCrop()->IsWithering() && t.getCrop()->IsMature()) {
                    float ripeTimer = t.getRipeTimer() + dt;
                    t.setRipeTimer(ripeTimer);
                    if (t.getRipeTimer() >= 5.0f) {
                        t.removeCrop();
                        continue;
                    }
                } else {
                    t.setRipeTimer(0.0f);
                }

                t.getCrop()->Draw(t.getRect());
            }
        }

        DrawText("5:Tomato  6:Potato  7:Pumpkin  LMB:Plant/Harvest", 20, winHeight - 60, 20, RAYWHITE);
        DrawText("Arrow Up/Down: Resize Grid", 20, winHeight - 40, 20, RAYWHITE);
        DrawText("BetterFarm++ OOP (16:10)", 20, winHeight - 20, 20, RAYWHITE);

        Vector2 mousePos = GetMousePosition();

        for (auto& t : tiles) {
            if (t.hasAnimal() && CheckCollisionPointRec(mousePos, t.getRect())) {
                // Get the animal's state string
                string stateText = t.getAnimal()->getState();

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
            if (ui.hoeing) watering = false; // keep modes exclusive
        }

        // Water mode
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, waterBtn)) {
            watering = !watering;
            if (watering) {
                ui.hoeing = false; // keep modes exclusive
            }
        }

        // Hoe to follow cursor
        if (ui.hoeing == true && coins >= 5 && shop.getPlacing() == false) {
            DrawTexturePro(
                tex.getHoeTex(),
                Rectangle{0, 0, (float)tex.getHoeTex().width, (float)tex.getHoeTex().height},
                Rectangle{mousePos.x, mousePos.y, 80, 80}, Vector2{80 / 2.0f, 80 / 2.0f}, 0.0f, WHITE
            );
            for (auto &t : tiles) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && t.getType() == TileType::GRASS && CheckCollisionPointRec(mousePos, t.getRect())) {
                     t.setType(TileType::HOED);
                     coins = coins - 5;
                }
            }
        }

        // Water can to follow cursor
        if (watering) {
            DrawTexturePro(
                waterCanTex,
                {0, 0, (float)waterCanTex.width, (float)waterCanTex.height},
                {mousePos.x, mousePos.y, 80, 80},
                {80/2.0f, 80/2.0f},
                0.0f, WHITE);
        }

        if (shop.getPlacing()) {
            Rectangle placingShop = { topBar.x + 300, topBar.y + 10, 290, 40 };
            DrawRectangleRec(placingShop, GOLD);
            DrawRectangleLinesEx(placingShop, 2, BROWN);
            DrawText("PLACING - to exit ENTER", (int)placingShop.x + 10, (int)placingShop.y + 10, 20, DARKPURPLE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, placingShop)) {
                shop.setPlacingFalse();
            }
        }

        EndDrawing();
    }

    // Cleanup
    UnloadTexture(potatoSeed);
    UnloadTexture(potatoSprout);
    UnloadTexture(potatoMid);
    UnloadTexture(potatoFull);

    UnloadTexture(pumpkinSeed);
    UnloadTexture(pumpkinSprout);
    UnloadTexture(pumpkinMid);
    UnloadTexture(pumpkinFull);

    UnloadTexture(tomatoSeed);
    UnloadTexture(tomatoSprout);
    UnloadTexture(tomatoMid);
    UnloadTexture(tomatoFull);

    UnloadTexture(waterCanTex);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}