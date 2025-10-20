#ifndef MENU_H
#define MENU_H

struct MainMenu {
    bool soundFX = true;
    float timeScale = 1.0f;
    int selectedOption = 0;
};

void ShowMainMenu(MainMenu &menu);

#endif
