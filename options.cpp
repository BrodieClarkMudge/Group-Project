#include "options.h"
#include "menu.h"
#include "raylib.h"

void Options::ShowOptions(Options &options) {

    const int optionsWidth = 400;
    const int optionsHeight = 300;

    int optionsX = (GetScreenWidth()  - optionsWidth)  / 2;
    int optionsY = (GetScreenHeight() - optionsHeight) / 2;

    Rectangle optionsRect = { (float)optionsX, (float)optionsY, (float)optionsWidth, (float)optionsHeight };
    Rectangle soundRect = { optionsRect.x + 50, optionsRect.y + 50, 300, 50 };
    Rectangle sliderRect = { optionsRect.x + 50, optionsRect.y + 160, 300, 20 };

    while (!WindowShouldClose()) {
        int winWidth = GetScreenWidth();
        int winHeight = GetScreenHeight();

        if (winWidth < 800 || winHeight < 500) {
            SetWindowSize((winWidth < 800) ? 800 : winWidth,
                          (winHeight < 500) ? 500 : winHeight);
        }
        
        Vector2 mousePos = GetMousePosition();

        if (options.getSoundFX()) SetMasterVolume(1.0f);
        else       SetMasterVolume(0.0f);

        // Start dragging when mouse pressed on slider
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, sliderRect)) {
            options.setDraggingSliderTrue();
        }
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            options.setDraggingSliderFalse();
        }

        // Update timeScale if dragging
        if (options.getDraggingSlider()) {
            float newX = mousePos.x;
            if (newX < sliderRect.x) newX = sliderRect.x;
            if (newX > sliderRect.x + sliderRect.width) newX = sliderRect.x + sliderRect.width;

            float tNorm = (newX - sliderRect.x) / sliderRect.width;
            options.setTimeScale( 0.5f + tNorm * (2.0f - 0.5f));
        }

        BeginDrawing();

        DrawRectangleRec(optionsRect, LIGHTGRAY);

        // Draw sound toggle
        DrawRectangleRec(soundRect, DARKGRAY);
        DrawText(options.getSoundFX() ? "Sound FX: ON" : "Sound FX: OFF", soundRect.x + 10, soundRect.y + 10, 20, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, soundRect)) {
            if (options.getSoundFX()) {
                options.setSoundFXFalse();
            } else {
                options.setSoundFXTrue();
            }
        }

        // Draw timeScale slider
        DrawRectangleRec(sliderRect, DARKGRAY);

        // Knob
        options.setKnobX(sliderRect.x + ((options.getTimeScale() - 0.5f) / (2.0f - 0.5f)) * sliderRect.width);
        Rectangle knobRect = { options.getKnobX() - 10, sliderRect.y - 5, 20, 30 };
        DrawRectangleRec(knobRect, RED);

        DrawText(TextFormat("Timescale: %.2fx", options.getTimeScale()), sliderRect.x, sliderRect.y - 30, 20, BLACK);

        DrawText("Press ENTER to continue", optionsRect.x + 65, optionsRect.y + 220, 20, BLACK);

        if (IsKeyPressed(KEY_ENTER)) {
            break;
        }


        EndDrawing();
    }
}

    void Options::setSoundFXTrue() {
        soundFX = true;
    }

    void Options::setSoundFXFalse() {
        soundFX = false;
    }

    bool Options::getSoundFX() {
        return soundFX;
    }

    void Options::setTimeScale(float time) {
        timeScale = time;
    }

    float Options::getTimeScale() {
        return timeScale;
    }

    void Options::setSelectedOption(int opt) {
        selectedOption = opt;
    }

    int Options::getSelectedOption() {
        return selectedOption;
    }

    void Options::setOptionsOpenTrue() {
        optionsOpen = true;
    }

    void Options::setOptionsOpenFalse() {
        optionsOpen = false;
    }

    bool Options::getOptionsOpen() {
        return optionsOpen;
    }

    void Options::setDraggingSliderTrue() {
        draggingSlider = true;
    }

    void Options::setDraggingSliderFalse() {
        draggingSlider = false;
    }

    bool Options::getDraggingSlider() {
        return draggingSlider;
    }

    void Options::setKnobX(float knobValue) {
        knobX = knobValue;
    }

    float Options::getKnobX() {
        return knobX;
    }
