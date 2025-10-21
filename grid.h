#ifndef GRID_H
#define GRID_H

#include <memory>
#include <vector>

#include "Crop.h"
#include "animal.h"
#include "raylib.h"

enum class TileType { GRASS, YARD, HOED };

class Tile {
 private:
  Rectangle rect;
  TileType type = TileType::GRASS;
  std::unique_ptr<Animal> animal = nullptr;
  std::unique_ptr<Crop> crop = nullptr;
  int gridCols = 17, gridRows = 10;
  const int minGrid = 10, maxGrid = 25;
  int prevCols = gridCols, prevRows = gridRows;
  float timeChange;
  float wateredGlow = 0.0f;
  float waterDecayTimer = 0.0f;
  float cropTimer = 0.0f;
  int waterCharges = 0;
  float ripeTimer = 0.0f;

 public:
  void ExpandGrid(std::vector<Tile>& tiles, int oldCols, int oldRows,
                  int newCols, int newRows, int areaWidth, int gridHeight,
                  int offsetX, int offsetY);

  void RebuildGrid(std::vector<Tile>& tiles, int cols, int rows, int areaWidth,
                   int gridHeight, int offsetX, int offsetY);

  void setGridCols(int colGrid);
  int getGridCols();

  void setGridRows(int rowGrid);
  int getGridRows();

  int getMinGrid();
  int getMaxGrid();

  void setPrevCols(int colPrev);
  int getPrevCols();

  void setPrevRows(int rowPrev);
  int getPrevRows();

  void setRect(Rectangle newRect);
  Rectangle getRect();

  void setAnimal(std::unique_ptr<Animal> newAnimal);
  Animal* getAnimal();
  bool hasAnimal();
  void removeAnimal();

  void setType(TileType newType);
  TileType getType();

  void setTimeChange();
  float getTimeChange();

  // Crop-related methods
  void setCrop(std::unique_ptr<Crop> newCrop);
  Crop* getCrop();
  bool hasCrop();
  void removeCrop();

  void setWateredGlow(float glow);
  float getWateredGlow();

  void setWaterDecayTimer(float timer);
  float getWaterDecayTimer();

  void setCropTimer(float timer);
  float getCropTimer();

  void setWaterCharges(int charges);
  int getWaterCharges();

  void setRipeTimer(float timer);
  float getRipeTimer();
};

#endif