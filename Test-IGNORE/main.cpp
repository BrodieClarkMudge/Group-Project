// main.cpp
// BetterFarm++ — OOP refactor / prototype
// Single-file prototype showing Crop inheritance, Farm, Storage, Shop, WaterTank, Control.
// NOTE: expand into separate headers/cpps for the real project.

#include "raylib.h"
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <cmath>
#include <algorithm>
#include <random>
#include <sstream>

// ----------------------------
// Utilities
// ----------------------------
float ClampF(float value, float mn, float mx){ if(value<mn) return mn; if(value>mx) return mx; return value; }
float LerpF(float a,float b,float t){ return a + (b-a)*t; }

// ----------------------------
// Game Constants
// ----------------------------
constexpr int GRID_SIZE = 10;
constexpr int TILE_SIZE = 64;
constexpr int SCREEN_WIDTH = 1000;
constexpr int SCREEN_HEIGHT = 700;
constexpr int FPS = 60;

// forward
struct Particle;
void UpdateParticles(std::vector<Particle>& particles);
void DrawParticles(const std::vector<Particle>& particles);

// ----------------------------
// Storage & Shop & WaterTank
// ----------------------------
struct Storage {
    // cropName -> amount
    std::map<std::string,int> items;
    int capacity = 9999; // simple
    void add(const std::string &name, int amt=1){
        items[name] += amt;
        if(items[name] > capacity) items[name] = capacity;
    }
    int get(const std::string &name) const {
        auto it = items.find(name);
        return it==items.end()?0:it->second;
    }
};

struct Shop {
    std::map<std::string,int> seedPrice;      // seed buying price
    std::map<std::string,int> sellPrice;      // sell price for harvested products
    Shop(){
        // default prices (modify)
        seedPrice["Potato"] = 10;
        seedPrice["Pepper"] = 15;
        seedPrice["Pumpkin"] = 25;
        sellPrice["Potato"] = 30;
        sellPrice["Pepper"] = 40;
        sellPrice["Pumpkin"] = 80;
    }
};

struct WaterTank {
    int capacity = 10;
    int current = 10;
    bool useWater(int amt=1){
        if(current>=amt){ current-=amt; return true;}
        return false;
    }
    void refill(){ current = capacity; }
};

// ----------------------------
// Particles
// ----------------------------
struct Particle{
    Vector2 pos;
    Vector2 velocity;
    float timer;
    Color color;
};

void UpdateParticles(std::vector<Particle>& particles){
    for(auto it=particles.begin(); it!=particles.end();){
        it->pos.x += it->velocity.x;
        it->pos.y += it->velocity.y;
        it->velocity.y += 0.12f;
        it->timer -= 1.0f;
        if(it->timer<=0) it = particles.erase(it);
        else ++it;
    }
}
void DrawParticles(const std::vector<Particle>& particles){
    for(auto &p : particles) DrawCircleV(p.pos, 3, p.color);
}

// ----------------------------
// Crop Hierarchy
// ----------------------------
struct TextureBundle {
    Texture2D tex;
};

// Abstract base crop
class Crop {
public:
    std::string name;
    int cost = 0;
    int growTimeFrames = 0; // total frames to mature
    int timer = 0; // remaining frames
    bool watered = false;
    bool ready = false;
    Texture2D texture{};
    Crop(const std::string &n, int c, int g, Texture2D t)
        : name(n), cost(c), growTimeFrames(g), timer(g), texture(t) {}
    virtual ~Crop() = default;

    // every frame the crop can update (water effects, wither etc.)
    virtual void update(){
        // growth if watered
        if(!ready && watered){
            if(timer>0) timer--;
            if(timer<=0){ timer=0; ready = true; }
        }
    }
    virtual void draw(float x, float y) const {
        // default draw scales with growth progress
        float progress = 1.0f - (timer / (float)std::max(1,growTimeFrames));
        float scale = 0.4f + 0.6f*progress;
        DrawTextureEx(texture, Vector2{ x + TILE_SIZE/2 - texture.width*scale/2.0f,
                                       y + TILE_SIZE/2 - texture.height*scale/2.0f },
                      0.0f, scale, WHITE);
    }

    virtual bool isRegenerating() const { return false; }
    virtual bool occupiesExtraTile() const { return false; } // for stalks
    // called when harvested -> returns how many items produced
    virtual int harvest(Storage &storage){
        if(ready){
            ready = false;
            timer = growTimeFrames;
            watered = false;
            return 1;
        }
        return 0;
    }

    // water the crop (player action or rain)
    virtual void water(){ watered = true; }

    // debug string
    virtual std::string debugStr() const {
        std::ostringstream ss;
        ss<<name<<" r:"<<ready<<" t:"<<timer<<" w:"<<watered;
        return ss.str();
    }
};

// Root: disappears on harvest (one yield)
class RootCrop : public Crop {
public:
    RootCrop(const std::string &n,int c,int g,Texture2D t):Crop(n,c,g,t){}
    int harvest(Storage &storage) override {
        if(!ready) return 0;
        storage.add(name,1);
        // Root disappears after harvest (we'll signal via return >0; farm will delete crop)
        return 1;
    }
};

// Regenerating: remains, produces multiple yields
class RegeneratingCrop : public Crop {
public:
    int regrowDelay = 60*5; // frames to regrow after harvest
    RegeneratingCrop(const std::string &n,int c,int g,Texture2D t):Crop(n,c,g,t){}
    bool isRegenerating() const override { return true; }
    int harvest(Storage &storage) override {
        if(!ready) return 0;
        storage.add(name,1);
        // not removed: revert to pre-grown state
        ready = false;
        timer = growTimeFrames / 2; // example: revert to mid-stage
        watered = false;
        return 1;
    }
};

// Stalk (multi-tile) base class (occupies primary tile and requires adjacent free tile)
class StalkCrop : public Crop {
public:
    // a simple representation: occupies two tiles horizontally (primary + right)
    StalkCrop(const std::string &n,int c,int g,Texture2D t):Crop(n,c,g,t){}
    bool occupiesExtraTile() const override { return true; }
    int harvest(Storage &storage) override {
        if(!ready) return 0;
        storage.add(name,1);
        // after harvest, remove (for simplicity)
        ready = false;
        timer = growTimeFrames;
        return 1;
    }
};

// ----------------------------
// Tile & Farm
// ----------------------------
enum class TileType { EMPTY, FARM };

struct Tile {
    TileType type = TileType::EMPTY;
    std::unique_ptr<Crop> crop; 
    bool occupiedByStalkExtra = false;

    void update(){ if(crop) crop->update(); }
    void draw(int tx,int ty) const { /* ...existing code... */ }
    bool hasCrop() const { return crop!=nullptr; }

    // Fix: make movable
    Tile() = default;
    Tile(Tile&&) = default;
    Tile& operator=(Tile&&) = default;

    Tile(const Tile&) = delete;            // cannot copy
    Tile& operator=(const Tile&) = delete; // cannot copy assign
};

class Farm {
public:
    int width, height;
    std::vector<std::vector<Tile>> grid;

    Farm(int w,int h) : width(w), height(h), grid(h) {
    for(auto &row : grid)
        row.resize(w); // now moves default-constructed Tiles, no copy
}

    void update(){
        for(int y=0;y<height;y++) for(int x=0;x<width;x++) grid[y][x].update();
    }

    void draw() const {
        for(int y=0;y<height;y++) for(int x=0;x<width;x++) grid[y][x].draw(x,y);
    }

    bool inBounds(int x,int y) const { return x>=0 && x<width && y>=0 && y<height; }

    // place a crop (factory provides a new Crop instance); for stalks we require extra tile to the right to be free
    bool placeCropAt(int x,int y, std::unique_ptr<Crop> c){
        if(!inBounds(x,y)) return false;
        Tile &t = grid[y][x];
        if(t.hasCrop()) return false;

        if(c->occupiesExtraTile()){
            // require right tile inside bounds and free and not already marked as extra
            if(!inBounds(x+1,y)) return false;
            if(grid[y][x+1].hasCrop() || grid[y][x+1].occupiedByStalkExtra) return false;
            // place on both tiles: primary holds crop, extra marked
            grid[y][x].crop = std::move(c);
            grid[y][x+1].occupiedByStalkExtra = true;
            grid[y][x].type = TileType::FARM;
            grid[y][x+1].type = TileType::FARM;
            return true;
        } else {
            grid[y][x].crop = std::move(c);
            grid[y][x].type = TileType::FARM;
            return true;
        }
    }

    // harvest returns number of items harvested (and crop type name placed into storage)
    int harvestAt(int x,int y, Storage &storage){
        if(!inBounds(x,y)) return 0;
        Tile &t = grid[y][x];
        if(!t.hasCrop()) return 0;
        // if this tile is an extra stalk tile, find primary left tile (simple approach)
        if(t.occupiedByStalkExtra){
            // look left for primary (assume primary at x-1)
            if(inBounds(x-1,y) && grid[y][x-1].hasCrop()){
                Tile &prim = grid[y][x-1];
                int amt = prim.crop->harvest(storage);
                // after harvest of a stalk we free both tiles (simple behaviour)
                prim.crop.reset();
                grid[y][x].occupiedByStalkExtra = false;
                grid[y][x].type = TileType::EMPTY;
                prim.type = TileType::EMPTY;
                return amt;
            }
            return 0;
        } else {
            int amt = t.crop->harvest(storage);
            // for RootCrop we want to remove the crop after harvest
            if(dynamic_cast<RootCrop*>(t.crop.get())){
                t.crop.reset();
                t.type = TileType::EMPTY;
            }
            // for StalkCrop we remove and clear extra tile (above logic covers primary removal if extra tile was clicked)
            if(dynamic_cast<StalkCrop*>(t.crop.get())){
                // clear extra tile to the right if present
                if(inBounds(x+1,y)) grid[y][x+1].occupiedByStalkExtra = false;
                t.crop.reset();
                t.type = TileType::EMPTY;
            }

            return amt;
        }
    }

    // water tile (consumes water via caller)
    bool waterAt(int x,int y){
        if(!inBounds(x,y)) return false;
        Tile &t = grid[y][x];
        if(!t.hasCrop()) return false;
        t.crop->water();
        return true;
    }
};

// ----------------------------
// CropFactory: create instances by name/id (makes ownership simple)
// ----------------------------
class CropFactory {
public:
    // textures must be set before create()
    Texture2D potatoTex{}, pepperTex{}, pumpkinTex{};
    std::unique_ptr<Crop> create(const std::string &id){
        if(id=="Potato") return std::make_unique<RootCrop>("Potato",10,60*6,potatoTex);
        if(id=="Pepper") return std::make_unique<RegeneratingCrop>("Pepper",15,60*8,pepperTex);
        if(id=="Pumpkin") return std::make_unique<StalkCrop>("Pumpkin",25,60*12,pumpkinTex);
        return nullptr;
    }
};

// ----------------------------
// Control (game runner, UI, state machine minimal)
// ----------------------------
class Control {
public:
    Farm farm;
    Storage storage;
    Shop shop;
    WaterTank tank;
    CropFactory factory;
    std::vector<Particle> particles;

    enum class Mode { PLAY, SHOP } mode = Mode::PLAY;
    std::string selectedSeed = "Potato";
    int coins = 100;

    Control(int w,int h):farm(w,h){}

    void loadAssets(Texture2D potatoTex, Texture2D pepperTex, Texture2D pumpkinTex){
        factory.potatoTex = potatoTex;
        factory.pepperTex = pepperTex;
        factory.pumpkinTex = pumpkinTex;
    }

    void update(){
        // farm and particles
        farm.update();
        UpdateParticles(particles);

        // simple: allow market auto-sell every 5 seconds (optional)
        // (Left out here, handles via shop button)
    }

    void drawUI(){
        // Sidebar
        DrawRectangle(650,0,350,SCREEN_HEIGHT, Fade(SKYBLUE,0.3f));
        DrawText(TextFormat("Coins: %d",coins),660,20,22,BLACK);
        DrawText("Selected seed:",660,60,18,BLACK);
        DrawText(selectedSeed.c_str(),660,80,20,RED);
        DrawText("Shop:",660,120,18,BLACK);

        int y=150;
        for(auto &kv : shop.seedPrice){
            std::string name = kv.first;
            int price = kv.second;
            DrawText(TextFormat("%s - $%d  (press key)", name.c_str(), price), 660, y, 16, (selectedSeed==name?RED:BLACK));
            y+=22;
        }

        // controls
        DrawText("Keys: 1 Potato  2 Pepper  3 Pumpkin", 660, y+10, 14, DARKGRAY);
        DrawText("Left click: plant/harvest   Right click: water", 660, y+30, 14, DARKGRAY);

        // storage display
        DrawText("Storage:", 660, y+60, 18, DARKBLUE);
        int yy = y+90;
        for(auto &s : storage.items){
            DrawText(TextFormat("%s x%d", s.first.c_str(), s.second), 660, yy, 16, DARKGRAY);
            yy += 22;
        }

        // Water tank
        DrawText(TextFormat("Water: %d/%d  (R to refill)", tank.current, tank.capacity), 660, yy+20, 16, DARKGREEN);
    }

    // plant if possible
    void tryPlantAt(int gx,int gy){
        if(!farm.inBounds(gx,gy)) return;
        // buy seed from shop if enough coins
        int price = shop.seedPrice[selectedSeed];
        if(coins < price) return;
        // create instance
        auto c = factory.create(selectedSeed);
        if(!c) return;
        // attempt placement
        if(farm.placeCropAt(gx,gy, std::move(c))){
            coins -= price;
            // small particle burst
            for(int i=0;i<8;i++){
                particles.push_back(Particle { {float(gx*TILE_SIZE+TILE_SIZE/2), float(gy*TILE_SIZE+TILE_SIZE/2)},
                                      { (float)(rand()%20-10)/6.0f, (float)(rand()%20-10)/6.0f },
                                      float(30) + rand()%20, GREEN });
            }
        }
    }

    // harvest tile
    void tryHarvestAt(int gx,int gy){
        if(!farm.inBounds(gx,gy)) return;
        // harvest
        int amt = farm.harvestAt(gx,gy, storage);
        if(amt>0){
            // small coin reward when selling manually (or add to storage and sell at shop)
            // show particles
            for(int i=0;i<10;i++){
                particles.push_back(Particle{ {float(gx*TILE_SIZE+TILE_SIZE/2), float(gy*TILE_SIZE+TILE_SIZE/2)},
                                      { (float)(rand()%20-10)/4.0f, (float)(rand()%20-10)/4.0f },
                                      40, YELLOW });
            }
        }
    }

    void tryWaterAt(int gx,int gy){
        if(!farm.inBounds(gx,gy)) return;
        if(tank.useWater(1)){
            if(farm.waterAt(gx,gy)){
                // water particles
                for(int i=0;i<6;i++){
                    particles.push_back(Particle
                        { {float(gx*TILE_SIZE+TILE_SIZE/2), float(gy*TILE_SIZE+TILE_SIZE/2)},
                                          { (float)(rand()%20-10)/6.0f, (float)(rand()%20-10)/6.0f },
                                          30, SKYBLUE });
                }
            }
        } else {
            // no water - tiny red particles
            for(int i=0;i<6;i++){
                particles.push_back(Particle { {float(gx*TILE_SIZE+TILE_SIZE/2), float(gy*TILE_SIZE+TILE_SIZE/2)},
                                      { (float)(rand()%20-10)/6.0f, (float)(rand()%20-10)/6.0f },
                                      20, RED });
            }
        }
    }

    // simple sell function: sell all of a given item
    void sellAll(const std::string &name){
        int amt = storage.get(name);
        if(amt<=0) return;
        int price = shop.sellPrice[name];
        coins += amt * price;
        storage.items[name] = 0;

        // coin particles on sidebar
        for(int i=0;i<12;i++){
            particles.push_back({ {800,500}, { (float)(rand()%20-10)/4.0f, (float)(rand()%20-10)/4.0f }, 40, GOLD });
        }
    }
};

// ----------------------------
// Main
// ----------------------------
int main(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BetterFarm++ Prototype (OOP)");
    SetTargetFPS(FPS);
    srand((unsigned)time(nullptr));

    // --- Load assets ---
    // Replace with your asset paths or placeholders
    Texture2D potatoTex = LoadTexture("assets/potato.png");
    Texture2D pepperTex = LoadTexture("assets/pepper.png");
    Texture2D pumpkinTex = LoadTexture("assets/pumpkin.png");

    Texture2D coinTex = LoadTexture("assets/coin.png"); // optional

    // --- Game objects ---
    Control control(GRID_SIZE, GRID_SIZE);
    control.loadAssets(potatoTex, pepperTex, pumpkinTex);

    // input seed selection mapping (keys)
    std::map<int,std::string> keyToSeed = {
        {KEY_ONE, "Potato"},
        {KEY_TWO, "Pepper"},
        {KEY_THREE, "Pumpkin"}
    };

    // main loop
    while(!WindowShouldClose()){
        Vector2 mouse = GetMousePosition();
        int gridX = (int)(mouse.x / TILE_SIZE);
        int gridY = (int)(mouse.y / TILE_SIZE);

        // Input
        if(IsKeyPressed(KEY_ESCAPE)) break;

        if(IsKeyPressed(KEY_R)){ control.tank.refill(); }

        for(auto &kv : keyToSeed){
            if(IsKeyPressed(kv.first)) control.selectedSeed = kv.second;
        }

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            // if clicked on sidebar area -> check buttons (simple: sell all)
            if(mouse.x > 650){
                // click within shop area — example: if click near product names sell them
                // simple region checks:
                if(mouse.y > 260 && mouse.y < 360){
                    // sell all potatoes (example)
                    control.sellAll("Potato");
                }
            } else {
                // planting / harvesting: if tile has a crop harvest, else plant
                if(control.farm.inBounds(gridX, gridY)){
                    if(control.farm.grid[gridY][gridX].hasCrop()){
                        // harvest
                        control.tryHarvestAt(gridX, gridY);
                    } else {
                        // plant
                        control.tryPlantAt(gridX, gridY);
                    }
                }
            }
        }

        if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
            // water
            if(control.farm.inBounds(gridX, gridY)) control.tryWaterAt(gridX, gridY);
        }

        // Update
        control.update();

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // draw farm grid background
        for(int y=0;y<GRID_SIZE;y++){
            for(int x=0;x<GRID_SIZE;x++){
                DrawRectangle(x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE, Fade(GRAY, 0.2f));
            }
        }

        // Draw farm (crops)
        control.farm.draw();

        // Draw particles & UI
        DrawParticles(control.particles);
        control.drawUI();

        // Draw a tooltip on tile
        if(control.farm.inBounds(gridX, gridY)){
            Tile &t = control.farm.grid[gridY][gridX];
            if(t.hasCrop()){
                DrawRectangle(10, SCREEN_HEIGHT-60, 400, 50, Fade(LIGHTGRAY, 0.9f));
                DrawText(t.crop->debugStr().c_str(), 16, SCREEN_HEIGHT-50, 14, BLACK);
            }
        }

        EndDrawing();
    }

    // unload textures
    UnloadTexture(potatoTex);
    UnloadTexture(pepperTex);
    UnloadTexture(pumpkinTex);
    UnloadTexture(coinTex);

    CloseWindow();
    return 0;
}
