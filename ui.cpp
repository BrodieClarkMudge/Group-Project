#include "ui.h"


FarmTextures LoadFarmTextures() {
    FarmTextures tex;

    tex.grassTex   = LoadTexture("assets/grassPixel.png");
    tex.yardTex    = LoadTexture("assets/yardPixel.png");
    tex.hoedTex    = LoadTexture("assets/hoedPixel.png");

    tex.coinTex    = LoadTexture("assets/coinPixel.png");
    tex.hoeTex     = LoadTexture("assets/hoePixel.png");
    tex.shopTex    = LoadTexture("assets/shopPixel.png");

    tex.cowTex     = LoadTexture("assets/cowPixel.png");
    tex.sheepTex   = LoadTexture("assets/sheepPixel.png");
    tex.chickenTex = LoadTexture("assets/chickenPixel.png");
    tex.pigTex     = LoadTexture("assets/pigPixel.png");


    tex.cowSound = LoadSound("assets/cowSound.wav");
    tex.sheepSound = LoadSound("assets/sheepSound.wav");
    tex.chickenSound = LoadSound("assets/chickenSound.wav");
    tex.pigSound = LoadSound("assets/pigSound.wav");

    return tex;
}

void UnloadFarmTextures(FarmTextures& tex) {
    UnloadTexture(tex.grassTex);
    UnloadTexture(tex.yardTex);
    UnloadTexture(tex.hoedTex);

    UnloadTexture(tex.coinTex);
    UnloadTexture(tex.hoeTex);
    UnloadTexture(tex.shopTex);

    UnloadTexture(tex.cowTex);
    UnloadTexture(tex.sheepTex);
    UnloadTexture(tex.chickenTex);
    UnloadTexture(tex.pigTex);


    UnloadSound(tex.cowSound);
    UnloadSound(tex.sheepSound);
    UnloadSound(tex.chickenSound);
    UnloadSound(tex.pigSound);

}