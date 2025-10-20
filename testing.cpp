#include <string>

#include "Autumn.h"
#include "Spring.h"
#include "Summer.h"
#include "WeatherSystem.h"
#include "Winter.h"
#include "raylib-cpp.hpp"

int main() {
  const int screenWidth = 420;
  const int screenHeight = 320;

  raylib::Window window(screenWidth, screenHeight,
                        "Weather Panel - Cozy Farm UI");
  SetTargetFPS(60);

  WeatherSystem weatherSystem;

  // Timing
  float dt = 0.0f;
  float dayTimer = 0.0f;
  float cropTimer = 0.0f;

  float dayLength = 12.0f;  // 1 in-game day = 12 real seconds
  float growEvery = 6.0f;   //  crops grow every 6 seconds
  float timeScale = 1.0f;   // speed up/slow down time if needed

  // Colors
  Color bg = {245, 243, 232, 255};
  Color panel = {250, 250, 245, 255};
  Color border = {200, 200, 180, 255};
  Color iconCol = {60, 110, 180, 255};
  Color textDark = {80, 80, 80, 255};
  Color subText = {100, 90, 80, 255};
  Color greenCol = {64, 145, 75, 255};
  Color redCol = {180, 50, 50, 255};

  while (!window.ShouldClose()) {
    //  Update timers
    dt = GetFrameTime() * timeScale;
    dayTimer += dt;

    //  apply season multiplier to crop growth
    float multiplier = weatherSystem.getCurrentSeason()->getGrowthMultiplier();
    cropTimer += dt * multiplier;

    //  Advance to next day
    if (dayTimer >= dayLength) {
      weatherSystem.updateDaily();
      dayTimer = 0.0f;
    }

    // 🌱 Trigger crop growth
    if (cropTimer >= growEvery) {
      // crop->Grow();  // integrate this with your real crop object
      cropTimer = 0.0f;
    }

    BeginDrawing();
    ClearBackground(bg);

    // Main rounded panel
    raylib::Rectangle panelRect(20, 30, 380, 230);
    panelRect.DrawRounded(0.12f, 10, panel);
    panelRect.DrawRoundedLines(0.12f, 10, 2, border);

    // Divider line
    DrawLine(210, 50, 210, 240, border);

    //  LEFT SIDE: WEATHER 
    std::string weather = weatherSystem.getCurrentWeather();
    int weatherTextWidth = MeasureText(weather.c_str(), 28);
    DrawText(weather.c_str(), 30 + (160 - weatherTextWidth) / 2, 100, 28,
             textDark);

    std::string dayStr = "Day " + std::to_string(weatherSystem.getCurrentDay());
    int dayWidth = MeasureText(dayStr.c_str(), 20);
    DrawText(dayStr.c_str(), 30 + (160 - dayWidth) / 2, 145, 20, subText);

    //  RIGHT SIDE: SEASON INFO 
    auto* season = weatherSystem.getCurrentSeason();
    std::string seasonLabel = "Season: " + season->getName();
    int seasonWidth = MeasureText(seasonLabel.c_str(), 22);
    DrawText(seasonLabel.c_str(), 230 + (160 - seasonWidth) / 2, 80, 22,
             iconCol);

    DrawText(TextFormat("Growth x%.1f", season->getGrowthMultiplier()), 230,
             130, 20, greenCol);
    DrawText(TextFormat("Wither: %.0f%%", season->getWitheringChance() * 100),
             230, 165, 20, redCol);

    // FOOTER    
    DrawText(
        TextFormat("[Day: %.1fs | Crop: %.1fs x%.1f]", dayLength - dayTimer,
                   growEvery - cropTimer, multiplier),
        20, screenHeight - 30, 18, border);

    EndDrawing();
  }

  return 0;
}