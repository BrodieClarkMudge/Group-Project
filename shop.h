#ifndef SHOP_H
#define SHOP_H

#include <memory>
#include <vector>

#include "animal.h"
#include "chicken.h"
#include "cow.h"
#include "grid.h"
#include "pig.h"
#include "raylib.h"
#include "sheep.h"
#include "ui.h"

class Shop {
 private:
  Rectangle windowRect;
  int buttonWidth;
  int buttonHeight;
  bool open = false;
  bool placing = false;  // true while user chooses where to place
  bool expandingTrue = false;
  int expandedGrid = 0;
  std::string selectedItem = "";

 public:
  Shop();
  void Open() { open = true; }
  void Close() { open = false; }

  void Update(std::vector<Tile>& tiles, int& coins, Texture2D& yardTex,
              Texture2D& cowTex, Texture2D& sheepTex, Texture2D& chickenTex,
              Texture2D& pigTex);
  void Draw();

  void setSelectedItem(std::string inputUser) { selectedItem = inputUser; }

  std::string getSelectedItem() { return selectedItem; }

  void setOpenTrue() { open = true; }

  void setOpenFalse() { open = false; }
  bool getOpen() { return open; }

  void setPlacingTrue() { placing = true; }

  void setPlacingFalse() { placing = false; }
  bool getPlacing() { return placing; }

  void setExpandingTrue() { expandingTrue = true; }

  void setExpandingFalse() { expandingTrue = false; }
  bool getExpanding() { return expandingTrue; }

  void increaseExpanded() { expandedGrid++; }

  int getExpanded() { return expandedGrid; }
};

#endif
