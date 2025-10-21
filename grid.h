#ifndef GRID_H
#define GRID_H

#include "raylib.h"
#include "animal.h"
#include <vector>
#include <memory>

enum class TileType {GRASS, YARD, HOED};

class Tile {
    private:
    Rectangle rect;
    TileType type = TileType::GRASS;
    std::unique_ptr<Animal> animal = nullptr;
    int gridCols = 17, gridRows = 10;
    const int minGrid = 10, maxGrid = 25;
    int prevCols = gridCols, prevRows = gridRows;
    float timeChange;

    public:
    void ExpandGrid(std::vector<Tile>& tiles, int oldCols, int oldRows,
                    int newCols, int newRows,
                    int areaWidth, int gridHeight, int offsetX, int offsetY);

    void RebuildGrid(std::vector<Tile>& tiles, int cols, int rows,
                    int areaWidth, int gridHeight, int offsetX, int offsetY);
    
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

    void setType(TileType newType);

    TileType getType();

    void setTimeChange();

    float getTimeChange();
};

#endif