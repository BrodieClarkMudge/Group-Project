#include "animal.h"
#include "chicken.h"
#include "cow.h"
#include "menu.h"
#include "options.h"
#include "pig.h"
#include "raylib.h"
#include "sheep.h"

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

// ----------------------
// Tile
// ----------------------
enum class TileType { GRASS, YARD, HOED };

struct Tile {
  Rectangle rect;
  TileType type = TileType::GRASS;
  std::unique_ptr<Animal> animal = nullptr;
  std::unique_ptr<Crop> crop = nullptr;
  float cropTimer = 0.0f;
};

// ----------------------
// Grid functions
// ----------------------
void RebuildGrid(std::vector<Tile>& tiles, int cols, int rows, int areaWidth,
                 int gridHeight, int offsetX, int offsetY) {
  std::vector<std::unique_ptr<Animal>> oldAnimals;
  oldAnimals.reserve(tiles.size());
  for (auto& tile : tiles) oldAnimals.push_back(std::move(tile.animal));

  tiles.clear();
  float tileWidth = static_cast<float>(areaWidth) / cols;
  float tileHeight = static_cast<float>(gridHeight) / rows;

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      Tile t;
      t.rect = {offsetX + x * tileWidth, offsetY + y * tileHeight, tileWidth,
                tileHeight};
      int index = y * cols + x;
      if (index < static_cast<int>(oldAnimals.size()))
        t.animal = std::move(oldAnimals[index]);
      tiles.push_back(std::move(t));
    }
  }
}

void ExpandGrid(std::vector<Tile>& tiles, int oldCols, int oldRows, int newCols,
                int newRows, int areaWidth, int gridHeight, int offsetX,
                int offsetY) {
  float tileWidth = static_cast<float>(areaWidth) / newCols;
  float tileHeight = static_cast<float>(gridHeight) / newRows;
  std::vector<Tile> newTiles;
  newTiles.reserve(newCols * newRows);

  for (int y = 0; y < newRows; y++) {
    for (int x = 0; x < newCols; x++) {
      Tile t;
      t.rect = {offsetX + x * tileWidth, offsetY + y * tileHeight, tileWidth,
                tileHeight};
      if (x < oldCols && y < oldRows)
        t.animal = std::move(tiles[y * oldCols + x].animal);
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
  const int barHeight = 60;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(initialWidth, initialHeight, "BetterFarm++ OOP (with Weather)");
  SetWindowMinSize(640, 400);
  SetTargetFPS(60);

  bool hoeing = false;

  MainMenu menu;
  ShowMainMenu(menu);
  timeScale = menu.timeScale;

  int gridCols = 17, gridRows = 10;
  const int minGrid = 10, maxGrid = 25;

  enum class Selected { COW, SHEEP, CHICKEN, PIG };
  Selected selectedAnimal = Selected::COW;

  enum class SelectedCrop { BERRY, POTATO, PUMPKIN };
  SelectedCrop selectedCrop = SelectedCrop::BERRY;

  // Load textures
  Texture2D grassTex = LoadTexture("assets/grassPixel.png");
  Texture2D yardTex = LoadTexture("assets/yardPixel.png");
  Texture2D hoedTex = LoadTexture("assets/hoedPixel.png");
  Texture2D coinTex = LoadTexture("assets/coinPixel.png");
  Texture2D hoeTex = LoadTexture("assets/hoePixel.png");
  Texture2D shopTex = LoadTexture("assets/shopPixel.png");
  Texture2D cowTex = LoadTexture("assets/cowPixel.png");
  Texture2D sheepTex = LoadTexture("assets/sheepPixel.png");
  Texture2D chickenTex = LoadTexture("assets/chickenPixel.png");
  Texture2D pigTex = LoadTexture("assets/pigPixel.png");

  std::vector<Tile> tiles;
  int prevCols = gridCols, prevRows = gridRows;

  const int uiMargin = 10;
  int coins = 123;
  int currentDay = 1;

  // 🌦️ Weather system
  WeatherSystem weatherSystem;
  float dayTimer = 0.0f;
  float dayLength = 12.0f;

  while (!WindowShouldClose()) {
    dt = GetFrameTime() * timeScale;
    dayTimer += dt;

    // ⏳ advance day + update weather
    if (dayTimer >= dayLength) {
      weatherSystem.updateDaily();
      dayTimer = 0.0f;
      currentDay++;
    }

    // grid handling (same as before)...

    BeginDrawing();
    ClearBackground(BLACK);

    // top bar UI (same as your original code)...

    for (auto& tile : tiles) {
      // draw textures
      if (tile.type == TileType::GRASS)
        DrawTexturePro(grassTex,
                       {0, 0, (float)grassTex.width, (float)grassTex.height},
                       tile.rect, {0, 0}, 0, WHITE);
      else if (tile.type == TileType::YARD)
        DrawTexturePro(yardTex,
                       {0, 0, (float)yardTex.width, (float)yardTex.height},
                       tile.rect, {0, 0}, 0, WHITE);
      else if (tile.type == TileType::HOED)
        DrawTexturePro(hoedTex,
                       {0, 0, (float)hoedTex.width, (float)hoedTex.height},
                       tile.rect, {0, 0}, 0, WHITE);

      DrawRectangleLinesEx(tile.rect, 1, BLACK);

      // 🐄 animals
      if (tile.animal)
        tile.animal->draw(tile.rect.x, tile.rect.y, tile.rect.width,
                          tile.rect.height);

      // 🌱 crops
      if (tile.crop) {
        float multiplier =
            weatherSystem.getCurrentSeason()->getGrowthMultiplier();
        tile.cropTimer += dt * multiplier;
        float growEvery = 6.0f;

        if (tile.cropTimer >= growEvery) {
          tile.crop->Grow();
          tile.cropTimer = 0.0f;
        }
        tile.crop->Draw(tile.rect);
      }
    }

    // 🌤️ WEATHER PANEL
    const int boxWidth = 250;
    const int boxHeight = 140;
    int boxX = GetScreenWidth() - boxWidth - 20;
    int boxY = GetScreenHeight() - boxHeight - 20;
    DrawRectangleRounded(
        {(float)boxX, (float)boxY, (float)boxWidth, (float)boxHeight}, 0.15f, 8,
        RAYWHITE);
    DrawRectangleRoundedLines(
        {(float)boxX, (float)boxY, (float)boxWidth, (float)boxHeight}, 0.15f, 8,
        2, GRAY);

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

  UnloadTexture(cowTex);
  UnloadTexture(sheepTex);
  UnloadTexture(chickenTex);
  UnloadTexture(pigTex);
  UnloadTexture(grassTex);

  CloseWindow();
  return 0;
}