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
float timeScale = 1.0;
float dehydrateTick = 0.0f;




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

    InitAudioDevice();

    SetTargetFPS(60);

    bool hoeing = false;
    bool watering = false;
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

    if (menu.getSoundFX())
    SetMasterVolume(1.0f);
else
    SetMasterVolume(0.0f);
    int gridCols = 17, gridRows = 10;
    const int minGrid = 10, maxGrid = 25;

// Calculate initial positions
int winWidth = GetScreenWidth();
int winHeight = GetScreenHeight();
int availableWidth = winWidth;
int availableHeight = winHeight;
int areaWidth = availableWidth;
int areaHeight = (int)(areaWidth / aspectRatio);
if (areaHeight > availableHeight) {
    areaHeight = availableHeight;
    areaWidth = (int)(areaHeight * aspectRatio);
}

int gridHeight = areaHeight - barHeight;
int offsetX = (winWidth - areaWidth) / 2;
int offsetY = (winHeight - areaHeight) / 2 + barHeight;

    // Load textures FIRST (before menu)
    FarmTextures tex = LoadFarmTextures();



    timeScale = menu.getTimeScale();

    // Declare tiles and selections
    std::vector<Tile> tiles;
    int prevCols = gridCols, prevRows = gridRows;

    enum class Selected { COW, SHEEP, CHICKEN, PIG };
    Selected selectedAnimal = Selected::COW;

    enum class SelectedCrop { BERRY, POTATO, PUMPKIN };
    SelectedCrop selectedCrop = SelectedCrop::BERRY;

    // NOW rebuild grid AFTER menu closes
    RebuildGrid(tiles, gridCols, gridRows, areaWidth, gridHeight, offsetX, offsetY);

// Load textures
    Texture2D grassTex = LoadTexture("assets/grassPixel.png");
    Texture2D yardTex = LoadTexture("assets/yardPixel.png");
    Texture2D hoedTex = LoadTexture("assets/hoedPixel.png");
    

    Texture2D coinTex = LoadTexture("assets/coinPixel.png");
    Texture2D waterCanTex = LoadTexture("assets/waterCanPixel.png");
   // Texture2D hoeTex = LoadTexture("assets/hoePixel.png");
    // Texture2D shopTex = LoadTexture("assets/shopPixel.png");

    Texture2D cowTex = LoadTexture("assets/cowPixel.png");
    Texture2D sheepTex = LoadTexture("assets/sheepPixel.png");
    Texture2D chickenTex = LoadTexture("assets/chickenPixel.png");
    Texture2D pigTex = LoadTexture("assets/pigPixel.png");
    
    

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



    // ----------------------
    // UI positions
    // ----------------------
    const int uiMargin = 10;
    int coins = 150;
    int water = 1000;
    int currentDay = 1;


    
    while (!WindowShouldClose()) {
        // calculates time to be used in animals and crops
        // GetFrameTime gives total time since window was initialised.
        dt = GetFrameTime() * timeScale; 


        // ---- Thirst drain over time ----
        static float thirstTick = 0.0f; // static so it tracks between frames
        thirstTick += dt;
        if (thirstTick >= 0.7f) { // every 0.7 sec
            for (auto& tile : tiles) {
                if (tile.animal) {
                    int t = tile.animal->getThirst() - 2; // consume water
                    if (t < 0) t = 0;
                    tile.animal->setThirst(t);
                }
            }
            thirstTick = 0.0f;
        }
        // Apply health damage to dehydrated animals once per second
        dehydrateTick += dt;
        if (dehydrateTick >= 0.7f) {
            for (auto& tile : tiles) {
                if (tile.animal && tile.animal->getThirst() == 0) {
                    tile.animal->calculateHealth();
                    tile.animal->checkSurvival();
                    if (!tile.animal->isAlive()) {
                        tile.animal.reset();   // delete
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
        if (shop.getExpanding() == true && gridRows < maxGrid && gridCols < maxGrid) {
            gridRows += 2; gridCols += 3;
            shop.setExpandingFalse();
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
                if (CheckCollisionPointRec(mouse, tile.rect) && ui.hoeing == false && watering == false) {
                    if (tile.type == TileType::YARD) {
                        if (tile.animal) {
                            tile.animal.reset();
                        } else {
                            switch (selectedAnimal) {
                                // make_unique makes a unique smart pointer.
                                case Selected::COW:tile.animal = std::make_unique<Cow>(tex.cowTex, tex.cowSound);         
                                break;
                                case Selected::SHEEP:tile.animal = std::make_unique<Sheep>(tex.sheepTex, tex.sheepSound);   
                                break;
                                case Selected::CHICKEN:tile.animal = std::make_unique<Chicken>(tex.chickenTex, tex.chickenSound); 
                                break;
                                case Selected::PIG:tile.animal = std::make_unique<Pig>(tex.pigTex, tex.pigSound);
                                break;

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
                if (CheckCollisionPointRec(mouse, tile.rect) && ui.hoeing == false && watering == false && tile.type == TileType::HOED) {
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

                       
                        
                        // Assign crop textures based on type
                        switch (selectedCrop) {
                            case SelectedCrop::BERRY: // Using tomato for berry
                                tile.crop->SetTexture(SEED,  tomatoSeed);
                                tile.crop->SetTexture(SEMI1, tomatoSprout);
                                tile.crop->SetTexture(SEMI2, tomatoMid);
                                tile.crop->SetTexture(FULL,  tomatoFull);
                                break;
                            case SelectedCrop::POTATO:
                                tile.crop->SetTexture(SEED,  potatoSeed);
                                tile.crop->SetTexture(SEMI1, potatoSprout);
                                tile.crop->SetTexture(SEMI2, potatoMid);
                                tile.crop->SetTexture(FULL,  potatoFull);
                                break;
                            case SelectedCrop::PUMPKIN:
                                tile.crop->SetTexture(SEED,  pumpkinSeed);
                                tile.crop->SetTexture(SEMI1, pumpkinSprout);
                                tile.crop->SetTexture(SEMI2, pumpkinMid);
                                tile.crop->SetTexture(FULL,  pumpkinFull);
                                break;
}
                        tile.cropTimer = 0.0f; // start growth clock after planting.
                    }
                }
            }
        }

        // WATERING BRANCH 
        if (watering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && water > 0) {
            Vector2 mouse = GetMousePosition();
            for (auto& tile : tiles) {
                if (!CheckCollisionPointRec(mouse, tile.rect)) continue;

                bool didWater = false; // has water action hre been done

                // Animal on yard - water
                if (tile.type == TileType::YARD && tile.animal) {
                    water = water - (100 - tile.animal->getThirst());
                    tile.animal->drink();
                    didWater = true;

                }
                // Crop on hoed - water
                else if (tile.type == TileType::HOED && tile.crop) {
                    water = water - (100 - tile.crop->getWaterAmount());
                    tile.crop->Water();
                    //tile.waterCharges +=1;
                    didWater = true;
                }

                if (didWater) {
                    tile.wateredGlow = 2.0f; // simple glow visual for feedback
                    break;
                }
            }
        }








        // Shop
        if (!shop.getOpen() && !shop.getSelectedItem().empty()) {
            Vector2 mouse = GetMousePosition();
            for (auto& tile : tiles) {
                if (CheckCollisionPointRec(mouse, tile.rect)) {
                    if (shop.getSelectedItem() == "YARD" && coins >= 20 && tile.type != TileType::YARD) {
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
        // water border logic before drawing starts
        for (auto& tile : tiles) {
            if (tile.wateredGlow > 0.0) {
                tile.wateredGlow -= dt;
                if (tile.wateredGlow < 0.0f) {tile.wateredGlow = 0.0;}
            }   
        }
        for (auto& tile : tiles) {
            // skip
            if (!tile.crop) {continue;}

            // accumulate time into the timer
            tile.waterDecayTimer += dt;

            // drain water 2 times  each second
            while (tile.waterDecayTimer >= .2f) {
                int w = tile.crop->getWaterAmount();
                if (w > 0) {
                    w -= tile.crop->getWaterConsumption();
                    if (w < 0) w = 0;  // in case it goes negative
                    tile.crop->SetWaterAmount(w);
                }
                tile.waterDecayTimer = 0; // push timer
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

        //hoe rectangle / UI
        Rectangle hoe = { topBar.x + 160, topBar.y + 10, 40, 40};
        DrawRectangleLinesEx(hoe, 2, BROWN);
        DrawTexturePro(tex.hoeTex, Rectangle{0,0, (float)tex.hoeTex.width, (float)tex.hoeTex.height},
            hoe, Vector2{0, 0}, 0.0f, WHITE
        );

        Rectangle waterBtn = { topBar.x + 210, topBar.y + 10, 40, 40 };
        DrawRectangleLinesEx(waterBtn, 2, BROWN);
        DrawTexturePro(waterCanTex, {0,0,(float)waterCanTex.width,(float)waterCanTex.height}, 
            waterBtn, {0,0}, 0.0, WHITE);

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

        // Water amount
        const char* waterText = TextFormat("Water: %d", water);
        int waterTextWidth = MeasureText(waterText, 20);
        DrawText(waterText, coinX - waterTextWidth - 30, topBar.y + 20, 20, BLACK);




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
            
            // watering borders
            // Recently watered? show a simple cyan outline
            if (tile.wateredGlow > 0.0f) {
                DrawRectangleLinesEx(tile.rect, 2, SKYBLUE);
            }

            // Animals: blue square = not thirsty, red = thirsty bout to take dmg
            if (tile.animal) {
                Rectangle mark = { tile.rect.x + 6, tile.rect.y + 6, 10, 10 };
                if (tile.animal->getThirst() == 0) {
                    DrawRectangleRec(mark, RED);
                } else {
                    DrawRectangleRec(mark, SKYBLUE);
                }
            }

            // Crops: tiny blue square if watered
            if (tile.crop && tile.crop->getWaterAmount() != 0) {
                Rectangle drop = { tile.rect.x + tile.rect.width - 16, tile.rect.y + 6, 10, 10 };
                DrawRectangleRec(drop, BLUE);
            }





            // ANIMALS 
            if (tile.animal) {
                tile.animal->updateSoundTimer(dt * 1000); // ms to sec
                tile.animal->draw(tile.rect.x, tile.rect.y, tile.rect.width, tile.rect.height);
            }

            // CROPS
            if (tile.crop) {
                // every ~2 seconds go to next stage
                tile.cropTimer += dt;
                float growEvery = 5.0;
                if (tile.cropTimer >= growEvery && tile.crop->getWaterAmount() > 0) {
                    tile.crop->Grow();
                    tile.cropTimer = 0.0f;
                  //  tile.waterCharges -= 1; // spend one charge
                }
                if (tile.crop->IsWithering() && tile.crop->IsMature()) {
                    tile.ripeTimer += dt;
                    if (tile.ripeTimer >= 5.0f) { // <-- set time window here or inside Withering
                        tile.crop.reset();        // auto destroy
                        continue;
                    }
                } else {
                    tile.ripeTimer = 0.0f;
                }
                tile.crop->Draw(tile.rect);

            }
        }
        // kill animals
        for (auto& tile : tiles) {
            if (tile.animal && !tile.animal->isAlive()) {
                tile.animal.reset();
            }
        }



        DrawText("1:Cow  2:Sheep  3:Chicken  4:Pig  LMB:Place/Remove, 5:Berry  6:Potato  7:Pumpkin", 20, winHeight - 60, 20, RAYWHITE);
        DrawText("", 20, winHeight - 80, 20, RAYWHITE);

        DrawText("Arrow Up/Down: Resize Grid", 20, winHeight - 40, 20, RAYWHITE);
        DrawText("BetterFarm++ OOP (16:10)", 20, winHeight - 20, 20, RAYWHITE);

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
            if (ui.hoeing) watering = false; // keep modes exclusive
        }


        // water mode
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, waterBtn)) {
            watering = !watering;   // flip on/off
            if (watering) {
                ui.hoeing = false; // keep modes exclusive
            }
        }

        // hoe to follow cursour 
        if(ui.hoeing == true && coins >= 5 && shop.getPlacing() == false) {
            DrawTexturePro(
                tex.hoeTex,
                Rectangle{0, 0, (float)tex.hoeTex.width, (float)tex.hoeTex.height},
                Rectangle{mousePos.x, mousePos.y, 80, 80}, Vector2{80 / 2.0f, 80 / 2.0f}, 0.0, WHITE
            );
        }

    
        for (auto &tile : tiles) {
            if (ui.hoeing && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && tile.type == TileType::GRASS
                && CheckCollisionPointRec(mousePos, tile.rect)) {
                     tile.type = TileType::HOED;
                     coins = coins - 5;
                }
            }

            // can to follow cursor
            if (watering) {
                DrawTexturePro(
                    waterCanTex,
                    {0, 0, (float)waterCanTex.width, (float)waterCanTex.height},
                    {mousePos.x, mousePos.y, 80, 80},
                    {80/2.0f, 80/2.0f},
                    0.0, WHITE);
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
    


    // Cleanup

    UnloadFarmTextures(tex);
    // unload crop textures
// Unload crop textures
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
    CloseWindow();
    return 0;


}
