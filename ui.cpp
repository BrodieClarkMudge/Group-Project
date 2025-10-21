#include "ui.h"

FarmTextures::FarmTextures() {

    grassTex = LoadTexture("assets/grassPixel.png");
    yardTex = LoadTexture("assets/yardPixel.png");
    hoedTex = LoadTexture("assets/hoedPixel.png");
    optionTex = LoadTexture("assets/optionsLogo.png");

    coinTex = LoadTexture("assets/coinPixel.png");
    hoeTex = LoadTexture("assets/hoePixel.png");
    shopTex = LoadTexture("assets/shopPixel.png");
    waterCanTex = LoadTexture("assets/waterCanPixel.png");

    cowTex = LoadTexture("assets/cowPixel.png");
    sheepTex = LoadTexture("assets/sheepPixel.png");
    chickenTex = LoadTexture("assets/chickenPixel.png");
    pigTex = LoadTexture("assets/pigPixel.png");

    //sounds
    cowSound = LoadSound("assets/cowSound.wav");
    sheepSound = LoadSound("assets/sheepSound.wav");
    chickenSound = LoadSound("assets/chickenSound.wav");
    pigSound = LoadSound("assets/pigSound.wav");

    //crops
    potatoSeed = LoadTexture("assets/1 - Potato Seed.png");
    potatoSprout = LoadTexture("assets/2 - Potato Sprout.png");
    potatoMid = LoadTexture("assets/3 - Potato Mid.png");
    potatoFull = LoadTexture("assets/4 - Potato Full.png");

    pumpkinSeed = LoadTexture("assets/1 - Pumpkin Seed.png");
    pumpkinSprout = LoadTexture("assets/2 - Pumpkin Sprout.png");
    pumpkinMid = LoadTexture("assets/3 - Pumpkin Mid.png");
    pumpkinFull = LoadTexture("assets/4 - Pumpkin Full.png");

    tomatoSeed = LoadTexture("assets/1 - Tomato Seed.png");
    tomatoSprout = LoadTexture("assets/2 - Tomato Sprout.png");
    tomatoMid = LoadTexture("assets/3 - Tomato Mid.png");
    tomatoFull = LoadTexture("assets/4 - Tomato Full.png");
}

FarmTextures::~FarmTextures() {
    UnloadTexture(grassTex);
    UnloadTexture(yardTex);
    UnloadTexture(hoedTex);

    UnloadTexture(coinTex);
    UnloadTexture(hoeTex);
    UnloadTexture(shopTex);
    UnloadTexture(optionTex);

    UnloadTexture(cowTex);
    UnloadTexture(sheepTex);
    UnloadTexture(chickenTex);
    UnloadTexture(pigTex);


    UnloadSound(cowSound);
    UnloadSound(sheepSound);
    UnloadSound(chickenSound);
    UnloadSound(pigSound);


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

}

Texture2D& FarmTextures::getGrassTex() {
    return grassTex;
}

Texture2D& FarmTextures::getYardTex() {
    return yardTex;
}

Texture2D& FarmTextures::getHoedTex() {
    return hoedTex;
}

Texture2D& FarmTextures::getCoinTex() {
    return coinTex;
}

Texture2D& FarmTextures::getHoeTex() {
    return hoeTex;
}

Texture2D& FarmTextures::getShopTex() {
    return shopTex;
}

Texture2D& FarmTextures::getWaterCanTex() {
    return waterCanTex;
}

Texture2D& FarmTextures::getCowTex() {
    return cowTex;
}

Texture2D& FarmTextures::getSheepTex() {
    return sheepTex;
}

Texture2D& FarmTextures::getChickenTex() {
    return chickenTex;
}

Texture2D& FarmTextures::getPigTex() {
    return pigTex;
}

Sound& FarmTextures::getCowSound() {
    return cowSound;
}

Sound& FarmTextures::getSheepSound() {
    return sheepSound;
}

Sound& FarmTextures::getChickenSound() {
    return chickenSound;
}

Sound& FarmTextures::getPigSound() {
    return pigSound;
}

Texture2D& FarmTextures::getTomatoSeed() {
    return tomatoSeed;
}

Texture2D& FarmTextures::getTomatoSprout() {
    return tomatoSprout;
}

Texture2D& FarmTextures::getTomatoMid() {
    return tomatoMid;
}

Texture2D& FarmTextures::getTomatoFull() {
    return tomatoFull;
}

Texture2D& FarmTextures::getPumpkinSeed() {
    return pumpkinSeed;
}

Texture2D& FarmTextures::getPumpkinSprout() {
    return pumpkinSprout;
}

Texture2D& FarmTextures::getPumpkinMid() {
    return pumpkinMid;
}

Texture2D& FarmTextures::getPumpkinFull() {
    return pumpkinFull;
}

Texture2D& FarmTextures::getPotatoSeed() {
    return potatoSeed;
}

Texture2D& FarmTextures::getPotatoSprout() {
    return potatoSprout;
}

Texture2D& FarmTextures::getPotatoMid() {
    return potatoMid;
}

Texture2D& FarmTextures::getPotatoFull() {
    return potatoFull;
}

Texture2D& FarmTextures::getOptionTex() {
    return optionTex;
}