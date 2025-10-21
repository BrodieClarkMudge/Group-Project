#ifndef UI_H
#define UI_H

#include "raylib.h"
#include "grid.h"
#include "options.h"
#include "shop.h"

// Struct for holding game UI state
struct UIState {
    int coins = 10000;
    int currentDay = 1;
    int water = 1000;
    bool hoeing = false;
    bool watering = false;
    float dt = 0.0f;
    float timeScale = 1.0f;
    float dehydrateTick = 0.0f;
    float dayTimer = 0.0f;
    float dayLength = 0.5f;
    const int initialWidth = 1280;
    const int initialHeight = 800;
    const float aspectRatio = 16.0f / 10.0f;
    const int barHeight = 60;

    // rectangles for interactive UI
    Rectangle optionsBackground;
    Rectangle hoe;
    Rectangle shopBackground;
    Rectangle topBar;
};


void InitUI();


void DrawUI();


void HandleUIInput();

class FarmTextures {
    private:
    Texture2D grassTex;
    Texture2D yardTex;
    Texture2D hoedTex;

    Texture2D coinTex;
    Texture2D hoeTex;
    Texture2D shopTex;
    Texture2D waterCanTex;
    Texture2D optionTex;

    Texture2D cowTex;
    Texture2D sheepTex;
    Texture2D chickenTex;
    Texture2D pigTex;

    //sounds
    Sound cowSound;
    Sound sheepSound;
    Sound chickenSound;
    Sound pigSound;

    //crops
    Texture2D potatoSeed;
    Texture2D potatoSprout;
    Texture2D potatoMid;
    Texture2D potatoFull;

    Texture2D pumpkinSeed;
    Texture2D pumpkinSprout;
    Texture2D pumpkinMid;
    Texture2D pumpkinFull;

    Texture2D tomatoSeed;
    Texture2D tomatoSprout;
    Texture2D tomatoMid;
    Texture2D tomatoFull;

    public:
    FarmTextures();

    ~FarmTextures();

    Texture2D& getGrassTex();

    Texture2D& getYardTex();

    Texture2D& getHoedTex();

    Texture2D& getCoinTex();

    Texture2D& getHoeTex();
         
    Texture2D& getShopTex();

    Texture2D& getWaterCanTex();

    Texture2D& getOptionTex();

    Texture2D& getCowTex();

    Texture2D& getSheepTex();

    Texture2D& getChickenTex();

    Texture2D& getPigTex();

    Sound& getCowSound();

    Sound& getSheepSound();

    Sound& getChickenSound();

    Sound& getPigSound();

    Texture2D& getTomatoSeed();

    Texture2D& getTomatoSprout();

    Texture2D& getTomatoMid();

    Texture2D& getTomatoFull();

    Texture2D& getPumpkinSeed();

    Texture2D& getPumpkinSprout();

    Texture2D& getPumpkinMid();

    Texture2D& getPumpkinFull();

    Texture2D& getPotatoSeed();

    Texture2D& getPotatoSprout();

    Texture2D& getPotatoMid();

    Texture2D& getPotatoFull();

};

#endif
