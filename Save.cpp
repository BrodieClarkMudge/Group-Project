#include "Save.h"
#include <fstream>
#include "ui.h"

using namespace std;
void SaveGame(int coins, int water) {
    // output stream, create file.
    ofstream file("savegame.txt");
    // print coins and water to file
    file << coins << "\n" << water;
    file.close();
}

void LoadGame(int& coins, int& water) {
    // input stream open file
    ifstream file("savegame.txt");
    // read coins and water from file
    file >> coins >> water;
    file.close();
}