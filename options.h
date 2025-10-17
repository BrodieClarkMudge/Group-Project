#ifndef OPTIONS_H
#define OPTIONS_H

struct Options {
    bool soundFX = true;
    float timeScale = 1.0f;
    int selectedOption = 0;
};

void ShowOptions(Options &options);

#endif
