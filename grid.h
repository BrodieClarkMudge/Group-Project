#ifndef GRID_H
#define GRID_H

#include <memory>
#include <vector>

#include "Crop.h"  //  Include Crop so Tile can hold crops
#include "animal.h"
#include "raylib.h"

enum class TileType { GRASS, YARD, HOED };

struct Tile {
  Rectangle rect;
  TileType type = TileType::GRASS;
  std::unique_ptr<Animal> animal = nullptr;

  //Added for crop & weather system integration
  std::unique_ptr<Crop> crop = nullptr;
  float cropTimer = 0.0f;
};

// Functions for managing grid
void ExpandGrid(std::vector<Tile>& tiles, int oldCols, int oldRows, int newCols,
                int newRows, int areaWidth, int gridHeight, int offsetX,
                int offsetY);

void RebuildGrid(std::vector<Tile>& tiles, int cols, int rows, int areaWidth,
                 int gridHeight, int offsetX, int offsetY);

#endif