

#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include "raylib-cpp.hpp"
#include "raylib.h"
#include "Crop.h"
#include "menu.h"

Crop::Crop(float dt)
{
    dtPlanted = dt; // records time of creation for growth calculations
    stage = SEED;
    waterAmount = 0;
    isWatered = false;
    waterConsumption = 100;

    // make the texture array empty, to be filled in gameloop where textures are loaded
    for (int i = 0; i < 4; i++)
    {
        stageTex[i].id = 0;
    }
}
Crop::~Crop() = default;
void Crop::Grow()
{
    if (stage < FULL)
    {
        stage = (CropStage)(stage + 1); // need to cast to increment enum
    }
}

void Crop::Draw(Rectangle area)
{
    // draw current stage at the coords of area
    // Vector2{0,0} is for rotation, not needed
    DrawTexturePro(stageTex[stage],
                   Rectangle{0, 0, (float)stageTex[stage].width, (float)stageTex[stage].height},
                   area, Vector2{0, 0}, 0, WHITE);
}

void Crop::SetTexture(CropStage s, Texture2D tex)
{
    stageTex[s] = tex; // tex is the loaded texture
}

CropStage Crop::GetStage() const
{
    return stage;
}

bool Crop::IsMature() const
{
    return stage == FULL;
}

void Crop::Water()
{
    waterAmount = 100;
    isWatered = true;
}

int Crop::getWaterAmount()
{
    return waterAmount;
}

void Crop::SetWaterAmount(int x)
{
    waterAmount = x;
    if (waterAmount < 0)
        waterAmount = 0;
}
int Crop::getWaterConsumption()
{
    return waterConsumption;
}

bool Crop::IsWithering()
{
    return false;
}

std::string Crop::getState()
{
    std::ostringstream oss;
    switch (stage)
    {
    case SEED:
        oss << ", Stage: Seed";
        break;
    case SEMI1:
        oss << ", Stage: Sprout";
        break;
    case SEMI2:
        oss << ", Stage: Mid";
        break;
    case FULL:
        oss << ", Stage: Full";
        break;
    default:
        oss << ", Stage: Unknown";
        break;
    }

    oss << (isWatered ? " (Watered)" : " (Dry)");

    if (IsWithering())
    {
        oss << ", Withering";
    }

    if (IsMature())
    {
        oss << ", Ready to Harvest";
    }

    return oss.str();
}