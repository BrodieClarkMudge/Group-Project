#include "raylib-cpp.hpp"
#include "raylib.h"

// run with g++ Control.cpp -o main -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

int main() {
  // ******** game window initilisation ************

  // define dimensions of screen
  const int screenWidth = 800;
  const int screenHeight = 600;

  // Initilise
  InitWindow(screenWidth, screenHeight, "My window");

  // Loads the texture as a built in class
  Texture2D test = LoadTexture("resources/4 - Potato Full.png");
  int frameWidth = test.width;
  int frameHeight = test.height;

  /*
  // Source rectangle, defining which part of the texture to draw
  // since in Texture2D values are stored as ints, they need to be cast
  // (converted) to floats
  Rectangle sourceRec = {0.0f, 0.0f, (float)frameWidth, (float)frameHeight};

  // Destination rectangle where on the screen to draw it
  Rectangle destRec = {
      screenWidth / 2.0f, screenHeight / 2.0f, frameWidth * 5.0f,
      frameHeight * 5.0f};  // frame dimensions are the scale of the texture
 */

  // Drawing using DrawTextureEX
  float scale = 10.0;
  float rotation = 0;

  // position to draw - will be centred
  Vector2 position = {screenWidth / 2.0f - (test.width * scale) / 2.0f,
                      screenHeight / 2.0f - (test.height * scale) / 2.0f};
  SetTargetFPS(60);

  // actually draw everything - while window is not closed / closing
  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(WHITE);
    DrawTextureEx(test, position, rotation, scale, WHITE);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}