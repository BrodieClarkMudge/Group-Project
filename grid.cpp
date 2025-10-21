#include "grid.h"

void Tile::RebuildGrid(std::vector<Tile>& tiles, int cols, int rows,
                       int areaWidth, int gridHeight, int offsetX,
                       int offsetY) {
  // Save old tile data (type + animal + crops)
  struct TileData {
    TileType type;
    std::unique_ptr<Animal> animal;
    std::unique_ptr<Crop> crop;
    float cropTimer;
    int waterCharges;
    float wateredGlow;
    float waterDecayTimer;
    float ripeTimer;
  };
  std::vector<TileData> oldTiles;
  oldTiles.reserve(tiles.size());
  for (auto& tile : tiles) {
    oldTiles.push_back({tile.type, std::move(tile.animal), std::move(tile.crop),
                        tile.cropTimer, tile.waterCharges, tile.wateredGlow,
                        tile.waterDecayTimer, tile.ripeTimer});
  }

  tiles.clear();
  float tileWidth = static_cast<float>(areaWidth) / cols;
  float tileHeight = static_cast<float>(gridHeight) / rows;

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      Tile t;
      t.rect = {offsetX + x * tileWidth, offsetY + y * tileHeight, tileWidth,
                tileHeight};

      int index = y * cols + x;
      if (index < static_cast<int>(oldTiles.size())) {
        t.type = oldTiles[index].type;
        t.animal = std::move(oldTiles[index].animal);
        t.crop = std::move(oldTiles[index].crop);
        t.cropTimer = oldTiles[index].cropTimer;
        t.waterCharges = oldTiles[index].waterCharges;
        t.wateredGlow = oldTiles[index].wateredGlow;
        t.waterDecayTimer = oldTiles[index].waterDecayTimer;
        t.ripeTimer = oldTiles[index].ripeTimer;
      } else {
        t.type = TileType::GRASS;  // default
      }

      tiles.push_back(std::move(t));
    }
  }
}

void Tile::ExpandGrid(std::vector<Tile>& tiles, int oldCols, int oldRows,
                      int newCols, int newRows, int areaWidth, int gridHeight,
                      int offsetX, int offsetY) {
  float tileWidth = static_cast<float>(areaWidth) / newCols;
  float tileHeight = static_cast<float>(gridHeight) / newRows;

  std::vector<Tile> newTiles;
  newTiles.reserve(newCols * newRows);

  for (int y = 0; y < newRows; y++) {
    for (int x = 0; x < newCols; x++) {
      Tile t;
      t.rect = {offsetX + x * tileWidth, offsetY + y * tileHeight, tileWidth,
                tileHeight};

      if (x < oldCols && y < oldRows) {
        Tile& oldTile = tiles[y * oldCols + x];
        t.type = oldTile.type;
        t.animal = std::move(oldTile.animal);
        t.crop = std::move(oldTile.crop);
        t.cropTimer = oldTile.cropTimer;
        t.waterCharges = oldTile.waterCharges;
        t.wateredGlow = oldTile.wateredGlow;
        t.waterDecayTimer = oldTile.waterDecayTimer;
        t.ripeTimer = oldTile.ripeTimer;
      } else {
        t.type = TileType::GRASS;  // default
      }

      newTiles.push_back(std::move(t));
    }
  }

  tiles = std::move(newTiles);
}

void Tile::setGridCols(int colGrid) { gridCols = colGrid; }

int Tile::getGridCols() { return gridCols; }

void Tile::setGridRows(int rowGrid) { gridRows = rowGrid; }

int Tile::getGridRows() { return gridRows; }

int Tile::getMinGrid() { return minGrid; }

int Tile::getMaxGrid() { return maxGrid; }

void Tile::setPrevCols(int colPrev) { prevCols = colPrev; }

int Tile::getPrevCols() { return prevCols; }

void Tile::setPrevRows(int rowPrev) { prevRows = rowPrev; }

int Tile::getPrevRows() { return prevRows; }

void Tile::setRect(Rectangle newRect) { rect = newRect; }

Rectangle Tile::getRect() { return rect; }

void Tile::setAnimal(std::unique_ptr<Animal> newAnimal) {
  animal = std::move(newAnimal);
}

Animal* Tile::getAnimal() { return animal.get(); }

bool Tile::hasAnimal() { return animal != nullptr; }

void Tile::removeAnimal() { animal.reset(); }

void Tile::setType(TileType newType) { type = newType; }

TileType Tile::getType() { return type; }

void Tile::setTimeChange() { timeChange = GetFrameTime(); }

float Tile::getTimeChange() { return timeChange; }

// Crop-related methods
void Tile::setCrop(std::unique_ptr<Crop> newCrop) { crop = std::move(newCrop); }

Crop* Tile::getCrop() { return crop.get(); }

bool Tile::hasCrop() { return crop != nullptr; }

void Tile::removeCrop() { crop.reset(); }

void Tile::setWateredGlow(float glow) { wateredGlow = glow; }

float Tile::getWateredGlow() { return wateredGlow; }

void Tile::setWaterDecayTimer(float timer) { waterDecayTimer = timer; }

float Tile::getWaterDecayTimer() { return waterDecayTimer; }

void Tile::setCropTimer(float timer) { cropTimer = timer; }

float Tile::getCropTimer() { return cropTimer; }

void Tile::setWaterCharges(int charges) { waterCharges = charges; }

int Tile::getWaterCharges() { return waterCharges; }

void Tile::setRipeTimer(float timer) { ripeTimer = timer; }

float Tile::getRipeTimer() { return ripeTimer; }