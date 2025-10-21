#ifndef UI_H
#define UI_H

#include "raylib.h"
#include "grid.h"
#include "options.h"
#include "shop.h"

// Struct for holding game UI state
struct UIState {
    int coins = 150;
    int currentDay = 1;
    bool hoeing = false;

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

    Texture2D cowTex;
    Texture2D sheepTex;
    Texture2D chickenTex;
    Texture2D pigTex;

    //sounds
    Sound cowSound;
    Sound sheepSound;
    Sound chickenSound;
    Sound pigSound;

    public:
    FarmTextures();

    ~FarmTextures();

    Texture2D& getGrassTex();

    Texture2D& getYardTex();

    Texture2D& getHoedTex();

    Texture2D& getCoinTex();

    Texture2D& getHoeTex();
         
    Texture2D& getShopTex();

    Texture2D& getCowTex();

    Texture2D& getSheepTex();

    Texture2D& getChickenTex();

    Texture2D& getPigTex();

    Sound& getCowSound();

    Sound& getSheepSound();

    Sound& getChickenSound();

    Sound& getPigSound();

};

#endif
