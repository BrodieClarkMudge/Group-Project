#ifndef MENU_H
#define MENU_H

class Menu {
    private:
    bool soundFX = true;
    float timeScale = 1.0f;
    int selectedOption = 0;

    public:
    void ShowMenu(Menu &menu);

    void setSoundFXTrue();

    void setSoundFXFalse();

    bool getSoundFX();

    void setTimeScale(float time);

    float getTimeScale();

    void setSelectedOption(int opt);

    int getSelectedOption();

};

#endif
