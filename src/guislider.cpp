#include "gui.h"
#include "guislider.h"
#include "graphics.h"

bool initGUISlider(float minValue, float maxValue, float *value, GUISlider *target)
{
    if (minValue > maxValue)
    {
        return false;
    }
    target->valueChanged = false;
    initClickable(&target->clickable);
    
    
    setClickablePosition(V2(0, 0), &target->clickable);
    setClickableColor(0xffbbbbbb, &target->clickable);
    target->modifiableValue = value;
    target->minValue = minValue;
    target->maxValue = maxValue;
    if (value)
    {
        setSliderVal(*value, target);
    }
    else
    {
        setSliderVal(jadel::lerp(minValue, maxValue, 0.5f), target);
    }
    return true;
}

void setSliderVal(float value, GUISlider *slider)
{
    float finalValue = JADEL_CLAMP(value, slider->minValue, slider->maxValue);
    slider->internalValue = finalValue;
    if (slider->modifiableValue)
    {
        *(slider->modifiableValue) = finalValue;
    }
}

void updateGUISlider(GUISlider *slider)
{
    slider->valueChanged = false;
    if (isClickableHovered(&slider->clickable) && jadel::inputIsMouseLeftHeld())
    {
        V2 sliderBottomLeft = getClickableWorldBottomLeft(&slider->clickable);
        float mouseRelY = (getMouseProjectedPos().y - sliderBottomLeft.y) / slider->clickable.dimensions.y;
        float value = jadel::lerp(slider->minValue, slider->maxValue, mouseRelY);
        setSliderVal(value, slider);
        slider->valueChanged = true;
        jadel::message("Slider value: %f\n", slider->internalValue);
    }
}

void renderSlider(const GUISlider *slider)
{
    float knobRadius = slider->clickable.dimensions.x * 0.25f;
    Clickable knob;
    initClickable(&knob);
    // V2 knobPos(slider->clickable.dimensions.x * 0.5f - knobRadius, knobRadius + (slider->internalValue / (slider->maxValue - slider->minValue)) * (slider->clickable.dimensions.y - 2.0f * knobRadius));
    V2 knobPos(slider->clickable.dimensions.x * 0.5f - knobRadius, (slider->internalValue / (slider->maxValue - slider->minValue)) * slider->clickable.dimensions.y - knobRadius);
    setClickablePosition(knobPos, &knob);
    setClickableDimensions(V2(knobRadius * 2.0f, knobRadius * 2.0f), &knob);
    setClickableParent(&slider->clickable, &knob, CLICKABLE_CORNER_BOTTOM_LEFT);
    setClickableColor(0xff555555, &knob);
    renderClickable(&slider->clickable);
    renderClickable(&knob);
}