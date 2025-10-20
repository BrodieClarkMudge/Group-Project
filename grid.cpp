#include "grid.h"

void RebuildGrid(std::vector<Tile>& tiles, int cols, int rows,
                 int areaWidth, int gridHeight, int offsetX, int offsetY)
{
    std::vector<std::unique_ptr<Animal>> oldAnimals;
    std::vector<std::unique_ptr<Crop>>   oldCrops;
    std::vector<float>                   oldCropTimers;

    oldAnimals.reserve(tiles.size());
    oldCrops.reserve(tiles.size());
    oldCropTimers.reserve(tiles.size());

    for (auto& tile : tiles) {
        oldAnimals.push_back(std::move(tile.animal));
        oldCrops.push_back(std::move(tile.crop));
        oldCropTimers.push_back(tile.cropTimer);
    }

    tiles.clear();

    float tileWidth  = static_cast<float>(areaWidth) / cols;
    float tileHeight = static_cast<float>(gridHeight) / rows;

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            Tile t;
            t.rect = { offsetX + x * tileWidth, offsetY + y * tileHeight,
                       tileWidth, tileHeight };

            int index = y * cols + x;

            if (index < static_cast<int>(oldAnimals.size())) {
                t.animal = std::move(oldAnimals[index]);
            }
            if (index < static_cast<int>(oldCrops.size())) {
                t.crop = std::move(oldCrops[index]);
                t.cropTimer = oldCropTimers[index];
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
                auto& old = tiles[y * oldCols + x];
                t.animal= std::move(old.animal); 
                t.crop= std::move(old.crop);
                t.cropTimer = old.cropTimer;
                t.type= old.type;
            } 

            newTiles.push_back(std::move(t));  // push for every new
        }
    }

    tiles = std::move(newTiles);
}
