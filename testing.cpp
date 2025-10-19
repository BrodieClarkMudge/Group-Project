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
    if (IsKeyPressed(KEY_SPACE)) {
      weatherSystem.updateDaily();
    }

    BeginDrawing();
    ClearBackground(bg);

    // Main rounded panel
    raylib::Rectangle panelRect(20, 30, 380, 230);
    panelRect.DrawRounded(0.12f, 10, panel);
    panelRect.DrawRoundedLines(0.12f, 10, 2, border);

    // Divider line
    DrawLine(210, 50, 210, 240, border);

    // ---------------- LEFT SIDE: WEATHER ----------------
    std::string weather = weatherSystem.getCurrentWeather();

    // Weather name (bigger font)
    int weatherTextWidth = MeasureText(weather.c_str(), 28);
    DrawText(weather.c_str(), 30 + (160 - weatherTextWidth) / 2, 100, 28,
             textDark);

    // Day number below it
    std::string dayStr = "Day " + std::to_string(weatherSystem.getCurrentDay());
    int dayWidth = MeasureText(dayStr.c_str(), 20);
    DrawText(dayStr.c_str(), 30 + (160 - dayWidth) / 2, 145, 20, subText);

    // ---------------- RIGHT SIDE: SEASON INFO ----------------
    auto* season = weatherSystem.getCurrentSeason();

    // Season label + name on the same line
    std::string seasonLabel = "Season: " + season->getName();
    int seasonWidth = MeasureText(seasonLabel.c_str(), 22);
    DrawText(seasonLabel.c_str(), 230 + (160 - seasonWidth) / 2, 80, 22,
             iconCol);

    DrawText(TextFormat("Growth x%.1f", season->getGrowthMultiplier()), 230,
             130, 20, greenCol);
    DrawText(TextFormat("Wither: %.0f%%", season->getWitheringChance() * 100),
             230, 165, 20, redCol);

    // ---------------- FOOTER ----------------
    DrawText("[SPACE] Next Day", 20, screenHeight - 30, 18, border);

    EndDrawing();
  }

  return 0;
}