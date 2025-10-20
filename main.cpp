#include "raylib-cpp.hpp"
#include "raylib.h"

// Farm
#include "animal.h"
#include "chicken.h"
#include "cow.h"
#include "grid.h"
#include "menu.h"
#include "options.h"
#include "pig.h"
#include "sheep.h"
#include "shop.h"
#include "ui.h"

// Crops
#include "Berry.h"
#include "Crop.h"
#include "Large.h"
#include "Potato.h"
#include "Pumpkin.h"
#include "Regenerating.h"
#include "Stalk.h"

// Weather
#include <memory>
#include <string>
#include <vector>

#include "Autumn.h"
#include "Spring.h"
#include "Summer.h"
#include "WeatherSystem.h"
#include "Winter.h"

// Timing globals
float dt = 0.0f;
float timeScale = 1.0f;

int main() {
  const int initialWidth = 1280;
  const int initialHeight = 800;
  const float aspectRatio = 16.0f / 10.0f;
  const int barHeight = 60;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(initialWidth, initialHeight, "BetterFarm++ with Weather");
  SetWindowMinSize(640, 400);
  SetTargetFPS(60);

  bool hoeing = false;
  Shop shop;

  // Main menu
  MainMenu menu;
  ShowMainMenu(menu);
  timeScale = menu.timeScale;

  int gridCols = 17, gridRows = 10;
  const int minGrid = 10, maxGrid = 25;

  enum class Selected { COW, SHEEP, CHICKEN, PIG };
  Selected selectedAnimal = Selected::COW;

  enum class SelectedCrop { BERRY, POTATO, PUMPKIN };
  SelectedCrop selectedCrop = SelectedCrop::BERRY;

  FarmTextures tex = LoadFarmTextures();

  // Load animal textures (can be merged into FarmTextures later if needed)
  Texture2D cowTex = LoadTexture("assets/cowPixel.png");
  Texture2D sheepTex = LoadTexture("assets/sheepPixel.png");
  Texture2D chickenTex = LoadTexture("assets/chickenPixel.png");
  Texture2D pigTex = LoadTexture("assets/pigPixel.png");

  std::vector<Tile> tiles;
  int prevCols = gridCols, prevRows = gridRows;

  const int uiMargin = 10;
  int coins = 150;
  int currentDay = 1;

  // Weather system
  WeatherSystem weatherSystem;
  float dayTimer = 0.0f;
  float dayLength = 12.0f;  // seconds per day

  // Game Loop
  while (!WindowShouldClose()) {
    dt = GetFrameTime() * timeScale;
    dayTimer += dt;

    // Advance day + update weather
    if (dayTimer >= dayLength) {
      weatherSystem.updateDaily();
      currentDay++;
      dayTimer = 0.0f;
    }

    // Screen size + layout
    int winWidth = GetScreenWidth();
    int winHeight = GetScreenHeight();

    if (winWidth < 800 || winHeight < 500) {
      SetWindowSize((winWidth < 800) ? 800 : winWidth,
                    (winHeight < 500) ? 500 : winHeight);
      winWidth = GetScreenWidth();
      winHeight = GetScreenHeight();
    }

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

    // Grid controls
    if (IsKeyPressed(KEY_UP) && gridRows < maxGrid && gridCols < maxGrid) {
      gridRows += 2;
      gridCols += 3;
    }
    if (IsKeyPressed(KEY_DOWN) && gridRows > minGrid && gridCols > minGrid) {
      gridRows -= 2;
      gridCols -= 3;
    }

    if (gridCols != prevCols || gridRows != prevRows || tiles.empty()) {
      if (!tiles.empty()) {
        ExpandGrid(tiles, prevCols, prevRows, gridCols, gridRows, areaWidth,
                   areaHeight, offsetX, offsetY);
      } else {
        RebuildGrid(tiles, gridCols, gridRows, areaWidth, areaHeight, offsetX,
                    offsetY);
      }
      prevCols = gridCols;
      prevRows = gridRows;
    } else {
      float tileWidth = static_cast<float>(areaWidth) / gridCols;
      float tileHeight = static_cast<float>(gridHeight) / gridRows;
      for (int y = 0; y < gridRows; y++) {
        for (int x = 0; x < gridCols; x++) {
          int i = y * gridCols + x;
          tiles[i].rect = {offsetX + x * tileWidth, offsetY + y * tileHeight,
                           tileWidth, tileHeight};
        }
      }
    }

    // Animal selection
    if (IsKeyPressed(KEY_ONE)) selectedAnimal = Selected::COW;
    if (IsKeyPressed(KEY_TWO)) selectedAnimal = Selected::SHEEP;
    if (IsKeyPressed(KEY_THREE)) selectedAnimal = Selected::CHICKEN;
    if (IsKeyPressed(KEY_FOUR)) selectedAnimal = Selected::PIG;

    // Crop selection
    if (IsKeyPressed(KEY_FIVE)) selectedCrop = SelectedCrop::BERRY;
    if (IsKeyPressed(KEY_SIX)) selectedCrop = SelectedCrop::POTATO;
    if (IsKeyPressed(KEY_SEVEN)) selectedCrop = SelectedCrop::PUMPKIN;

    // Place/remove animals
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      Vector2 mouse = GetMousePosition();
      for (auto& tile : tiles) {
        if (CheckCollisionPointRec(mouse, tile.rect) && !hoeing) {
          if (tile.type == TileType::YARD) {
            if (tile.animal) {
              tile.animal.reset();
            } else {
              switch (selectedAnimal) {
                case Selected::COW:
                  tile.animal = std::make_unique<Cow>(cowTex);
                  break;
                case Selected::SHEEP:
                  tile.animal = std::make_unique<Sheep>(sheepTex);
                  break;
                case Selected::CHICKEN:
                  tile.animal = std::make_unique<Chicken>(chickenTex);
                  break;
                case Selected::PIG:
                  tile.animal = std::make_unique<Pig>(pigTex);
                  break;
              }
            }
          }
        }
      }
    }

    // Crops — apply growth multiplier
    float multiplier = weatherSystem.getCurrentSeason()->getGrowthMultiplier();
    for (auto& tile : tiles) {
      if (tile.crop) {
        tile.cropTimer += dt * multiplier;
        float growEvery = 2.0f;
        if (tile.cropTimer >= growEvery) {
          tile.crop->Grow();
          tile.cropTimer = 0.0f;
        }
      }
    }

    // Drawing
    BeginDrawing();
    ClearBackground(BLACK);

    Rectangle topBar = {(float)offsetX, (float)(offsetY - barHeight),
                        (float)areaWidth, (float)barHeight};
    DrawRectangleRec(topBar, GOLD);
    DrawRectangleLinesEx(topBar, 2, BROWN);

    const char* dayText = TextFormat("Day: %d", currentDay);
    int dayTextWidth = MeasureText(dayText, 20);
    int dayX = offsetX + areaWidth / 2 - dayTextWidth / 2;
    DrawText(dayText, dayX, topBar.y + 20, 20, BLACK);

    const char* coinText = TextFormat("Coins: %d", coins);
    int coinTextWidth = MeasureText(coinText, 20);
    int coinX = offsetX + areaWidth - coinTextWidth - 20;
    DrawText(coinText, coinX, topBar.y + 20, 20, BLACK);

    // Draw tiles
    for (auto& tile : tiles) {
      if (tile.type == TileType::GRASS) {
        DrawTexturePro(
            tex.grassTex,
            {0, 0, (float)tex.grassTex.width, (float)tex.grassTex.height},
            tile.rect, {0, 0}, 0, WHITE);
      } else if (tile.type == TileType::YARD) {
        DrawTexturePro(
            tex.yardTex,
            {0, 0, (float)tex.yardTex.width, (float)tex.yardTex.height},
            tile.rect, {0, 0}, 0, WHITE);
      } else if (tile.type == TileType::HOED) {
        DrawTexturePro(
            tex.hoedTex,
            {0, 0, (float)tex.hoedTex.width, (float)tex.hoedTex.height},
            tile.rect, {0, 0}, 0, WHITE);
      }

      DrawRectangleLinesEx(tile.rect, 1, BLACK);
      if (tile.animal)
        tile.animal->draw(tile.rect.x, tile.rect.y, tile.rect.width,
                          tile.rect.height);
      if (tile.crop) tile.crop->Draw(tile.rect);
    }

    // Weather Box UI
    const int boxWidth = 250;
    const int boxHeight = 140;
    int boxX = 20;
    int boxY = GetScreenHeight() - boxHeight - 20;

    raylib::Rectangle panelRect(boxX, boxY, boxWidth, boxHeight);
    panelRect.DrawRounded(0.12f, 10, RAYWHITE);
    panelRect.DrawRoundedLines(0.12f, 10, 2, GRAY);

    std::string weather = weatherSystem.getCurrentWeather();
    int weatherTextWidth = MeasureText(weather.c_str(), 24);
    DrawText(weather.c_str(), boxX + (boxWidth - weatherTextWidth) / 2,
             boxY + 20, 24, DARKGRAY);

    auto* season = weatherSystem.getCurrentSeason();
    std::string seasonLabel = "Season: " + season->getName();
    int seasonWidth = MeasureText(seasonLabel.c_str(), 20);
    DrawText(seasonLabel.c_str(), boxX + (boxWidth - seasonWidth) / 2,
             boxY + 60, 20, DARKBLUE);

    DrawText(TextFormat("Growth x%.1f", season->getGrowthMultiplier()),
             boxX + 20, boxY + 100, 18, DARKGREEN);

    EndDrawing();
  }

  UnloadFarmTextures(tex);
  CloseWindow();
  return 0;
}