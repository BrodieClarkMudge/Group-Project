#include "grid.h"

void RebuildGrid(std::vector<Tile>& tiles, int cols, int rows,
                 int areaWidth, int gridHeight, int offsetX, int offsetY)
{
// Save old tile data (type + animal + crops)
struct TileData {
    TileType type;
    std::unique_ptr<Animal> animal;
    std::unique_ptr<Crop> crop;
    float cropTimer;
    int waterCharges;
};
std::vector<TileData> oldTiles;
oldTiles.reserve(tiles.size());
for (auto& tile : tiles) {
    oldTiles.push_back({ tile.type, std::move(tile.animal), std::move(tile.crop), tile.cropTimer, tile.waterCharges });
}
    

    tiles.clear();

    float tileWidth  = static_cast<float>(areaWidth) / cols;
    float tileHeight = static_cast<float>(gridHeight) / rows;

    // restore data
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            Tile t;
            t.rect = { offsetX + x * tileWidth, offsetY + y * tileHeight,
                       tileWidth, tileHeight };

            int index = y * cols + x;
if (index < static_cast<int>(oldTiles.size())) {
    t.type = oldTiles[index].type;
    t.animal = std::move(oldTiles[index].animal);
    t.crop = std::move(oldTiles[index].crop);
    t.cropTimer = oldTiles[index].cropTimer;
    t.waterCharges = oldTiles[index].waterCharges;
} else {
    t.type = TileType::GRASS; // default
}

            tiles.push_back(std::move(t));
        }
    }
}

// Expand grid keeping existing animals & crops
void ExpandGrid(std::vector<Tile>& tiles, int oldCols, int oldRows,
                int newCols, int newRows,
                int areaWidth, int gridHeight, int offsetX, int offsetY)
{
    float tileWidth  = static_cast<float>(areaWidth) / newCols;
    float tileHeight = static_cast<float>(gridHeight) / newRows;

    std::vector<Tile> newTiles;
    newTiles.reserve(newCols * newRows);

    for (int y = 0; y < newRows; y++) {
        for (int x = 0; x < newCols; x++) {
            Tile t;
            t.rect = { offsetX + x * tileWidth, offsetY + y * tileHeight,
                       tileWidth, tileHeight };

            if (x < oldCols && y < oldRows) {
    Tile& oldTile = tiles[y * oldCols + x];
    t.type = oldTile.type;
    t.animal = std::move(oldTile.animal);
    t.crop = std::move(oldTile.crop);
    t.cropTimer = oldTile.cropTimer;
    t.waterCharges = oldTile.waterCharges;
} else {
    t.type = TileType::GRASS; // default
}

newTiles.push_back(std::move(t));
        }
    }

    tiles = std::move(newTiles);
}
