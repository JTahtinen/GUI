#pragma once
#include "clickable.h"
#include "defs.h"

struct GUISlider
{
    Clickable clickable;
    float internalValue;
    float *modifiableValue;
    float minValue;
    float maxValue;
    bool valueChanged;
};

bool initGUISlider(float minValue, float maxValue, float *value, GUISlider *target);
void setSliderVal(float value, GUISlider *slider);
void updateGUISlider(GUISlider *slider);
void renderSlider(const GUISlider *slider);