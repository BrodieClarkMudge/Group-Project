#include <string>

#include "Autumn.h"
#include "Spring.h"
#include "Summer.h"
#include "WeatherSystem.h"
#include "Winter.h"
#include "raylib-cpp.hpp"

int main() {
  const int screenWidth = 680;
  const int screenHeight = 240;

  raylib::Window window(screenWidth, screenHeight,
                        "Weather Panel - BetterFarm++ UI");
  SetTargetFPS(60);

  WeatherSystem weatherSystem;

  float dt = 0.0f;
  float dayTimer = 0.0f;
  float dayLength = 12.0f;

  // Colors
  Color bg = {235, 240, 245, 255};
  Color panel = {255, 255, 255, 255};
  Color shadow = {0, 0, 0, 15};
  Color line = {235, 238, 245, 255};
  Color label = {100, 110, 130, 255};
  Color text = {45, 55, 75, 255};
  Color blue = {70, 130, 220, 255};
  Color green = {75, 180, 100, 255};
  Color barBg = {235, 240, 250, 255};

  while (!window.ShouldClose()) {
    dt = GetFrameTime();
    dayTimer += dt;

    if (dayTimer >= dayLength) {
      weatherSystem.updateDaily();
      dayTimer = 0.0f;
    }

    BeginDrawing();
    ClearBackground(bg);

    // Panel shadow + main
    raylib::Rectangle shadowRect(24, 24, 640, 180);
    shadowRect.DrawRounded(0.12f, 20, shadow);
    raylib::Rectangle panelRect(20, 20, 640, 180);
    panelRect.DrawRounded(0.12f, 20, panel);

    // Header
    DrawText("WEATHER SYSTEM", 40, 35, 18, text);
    DrawText(TextFormat("DAY %d", weatherSystem.getCurrentDay()), 560, 37, 15,
             label);
    DrawLine(30, 60, 650, 60, line);

    // Dividers
    DrawLineEx({200, 75}, {200, 185}, 2, line);
    DrawLineEx({380, 75}, {380, 185}, 2, line);
    DrawLineEx({540, 75}, {540, 185}, 2, line);

    // Weather
    DrawText(weatherSystem.getCurrentWeather().c_str(), 50, 85, 28, text);
    DrawText("Current Condition", 50, 120, 13, label);

    // Season
    auto* season = weatherSystem.getCurrentSeason();
    std::string seasonName = season->getName();
    Color seasonCol = blue;
    if (seasonName == "Spring")
      seasonCol = {100, 200, 120, 255};
    else if (seasonName == "Summer")
      seasonCol = {255, 180, 60, 255};
    else if (seasonName == "Autumn")
      seasonCol = {220, 120, 60, 255};
    else if (seasonName == "Winter")
      seasonCol = {100, 180, 240, 255};

    DrawText(seasonName.c_str(), 230, 85, 24, seasonCol);
    DrawText("Season", 230, 115, 13, label);
    DrawText(TextFormat("%d days left",
                        season->getLength() - weatherSystem.getCurrentDay()),
             230, 140, 16, text);

    // Growth
    float growth = season->getGrowthMultiplier();
    DrawText(TextFormat("%.1fx", growth), 410, 85, 28, green);
    DrawText("Growth Rate", 410, 120, 13, label);

    raylib::Rectangle gBarBg(410, 145, 100, 6);
    gBarBg.DrawRounded(0.5f, 5, barBg);
    raylib::Rectangle gBar(410, 145, 100 * (growth / 2.0f), 6);
    gBar.DrawRounded(0.5f, 5, green);

    // Timer
    float timeLeft = dayLength - dayTimer;
    DrawText(TextFormat("%.1fs", timeLeft), 570, 85, 26, text);
    DrawText("Next Day", 570, 118, 13, label);

    raylib::Rectangle tBarBg(570, 145, 70, 6);
    tBarBg.DrawRounded(0.5f, 5, barBg);
    raylib::Rectangle tBar(570, 145, 70 * (dayTimer / dayLength), 6);
    tBar.DrawRounded(0.5f, 5, blue);

    // Season progress bar
    float seasonProg =
        (float)weatherSystem.getCurrentDay() / season->getLength();
    raylib::Rectangle sBg(30, 165, 620, 8);
    sBg.DrawRounded(0.5f, 5, barBg);
    raylib::Rectangle sBar(30, 165, 620 * seasonProg, 8);
    sBar.DrawRounded(0.5f, 5, seasonCol);

    EndDrawing();
  }

  return 0;
}