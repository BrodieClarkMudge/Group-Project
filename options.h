#ifndef OPTIONS_H
#define OPTIONS_H

class Options {
 private:
  bool soundFX = true;
  float timeScale = 1.0f;
  int selectedOption = 0;
  bool optionsOpen = false;
  bool draggingSlider = false;
  float knobX;

 public:
  void ShowOptions(Options& options);

  void setSoundFXTrue();

  void setSoundFXFalse();

  bool getSoundFX();

  void setTimeScale(float time);

  float getTimeScale();

  void setSelectedOption(int opt);

  int getSelectedOption();

  void setOptionsOpenTrue();

  void setOptionsOpenFalse();

  bool getOptionsOpen();

  void setDraggingSliderTrue();

  void setDraggingSliderFalse();

  bool getDraggingSlider();

  void setKnobX(float knobValue);

  float getKnobX();
};

#endif
