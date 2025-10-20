#ifndef SHOP_H
#define SHOP_H

#include "raylib.h"
#include "ui.h"
#include "grid.h"
#include "animal.h"
#include "cow.h"
#include "sheep.h"
#include "chicken.h"
#include "pig.h"
#include <vector>
#include <memory>

class Shop {
private:
    Rectangle windowRect;
    int buttonWidth;
    int buttonHeight;

public:
    Shop();
    void Open() { open = true; }
    void Close() { open = false; }

    void Update(std::vector<Tile>& tiles, int& coins,
                Texture2D& yardTex, Texture2D& cowTex, Texture2D& sheepTex,
                Texture2D& chickenTex, Texture2D& pigTex);
    void Draw();

    std::string selectedItem = "";  // what the user clicked
    bool open = false;
    bool placing = false;           // true while user chooses where to place
    bool expandingTrue = false;
};

#endif
