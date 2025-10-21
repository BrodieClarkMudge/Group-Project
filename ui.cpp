#include "ui.h"

FarmTextures::FarmTextures() {
  grassTex = LoadTexture("assets/grassPixel.png");
  yardTex = LoadTexture("assets/yardPixel.png");
  hoedTex = LoadTexture("assets/hoedPixel.png");

  coinTex = LoadTexture("assets/coinPixel.png");
  hoeTex = LoadTexture("assets/hoePixel.png");
  shopTex = LoadTexture("assets/shopPixel.png");

  cowTex = LoadTexture("assets/cowPixel.png");
  sheepTex = LoadTexture("assets/sheepPixel.png");
  chickenTex = LoadTexture("assets/chickenPixel.png");
  pigTex = LoadTexture("assets/pigPixel.png");

  cowSound = LoadSound("assets/cowSound.wav");
  sheepSound = LoadSound("assets/sheepSound.wav");
  chickenSound = LoadSound("assets/chickenSound.wav");
  pigSound = LoadSound("assets/pigSound.wav");
}

FarmTextures::~FarmTextures() {
  UnloadTexture(grassTex);
  UnloadTexture(yardTex);
  UnloadTexture(hoedTex);

  UnloadTexture(coinTex);
  UnloadTexture(hoeTex);
  UnloadTexture(shopTex);

  UnloadTexture(cowTex);
  UnloadTexture(sheepTex);
  UnloadTexture(chickenTex);
  UnloadTexture(pigTex);

  UnloadSound(cowSound);
  UnloadSound(sheepSound);
  UnloadSound(chickenSound);
  UnloadSound(pigSound);
}

Texture2D& FarmTextures::getGrassTex() { return grassTex; }

Texture2D& FarmTextures::getYardTex() { return yardTex; }

Texture2D& FarmTextures::getHoedTex() { return hoedTex; }

Texture2D& FarmTextures::getCoinTex() { return coinTex; }

Texture2D& FarmTextures::getHoeTex() { return hoeTex; }

Texture2D& FarmTextures::getShopTex() { return shopTex; }

Texture2D& FarmTextures::getCowTex() { return cowTex; }

Texture2D& FarmTextures::getSheepTex() { return sheepTex; }

Texture2D& FarmTextures::getChickenTex() { return chickenTex; }

Texture2D& FarmTextures::getPigTex() { return pigTex; }

Sound& FarmTextures::getCowSound() { return cowSound; }

Sound& FarmTextures::getSheepSound() { return sheepSound; }

Sound& FarmTextures::getChickenSound() { return chickenSound; }

Sound& FarmTextures::getPigSound() { return pigSound; }