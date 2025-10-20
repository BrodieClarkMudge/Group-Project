#ifndef GRID_H
#define GRID_H

#include "raylib.h"
#include "animal.h"
#include <vector>
#include <memory>

enum class TileType {GRASS, YARD, HOED};

struct Tile {
    Rectangle rect;
    TileType type = TileType::GRASS;
    std::unique_ptr<Animal> animal = nullptr;
};

void ExpandGrid(std::vector<Tile>& tiles, int oldCols, int oldRows,
                int newCols, int newRows,
                int areaWidth, int gridHeight, int offsetX, int offsetY);

void RebuildGrid(std::vector<Tile>& tiles, int cols, int rows,
                 int areaWidth, int gridHeight, int offsetX, int offsetY);

#endif