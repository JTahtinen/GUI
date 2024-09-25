#pragma once
#include <jadel.h>
#include "clickable.h"
#include "button.h"
#include "defs.h"
#include "guislider.h"

#define MAX_GUI_WINDOW_ELEMENTS_OF_TYPE (10)
#define DEFAULT_GUI_WINDOW_WIDTH (0.2f)

struct GUIWindow
{
    uint32 id;
    Clickable totalArea;
    Clickable client;
    Clickable header;
    bool isHighlighted;
    GUISlider sliders[MAX_GUI_WINDOW_ELEMENTS_OF_TYPE];
    int numSliders;
    Button buttons[MAX_GUI_WINDOW_ELEMENTS_OF_TYPE];
    int numButtons;
    float buttonGap;
    float lastAddedButtonY;
};

// void initGUIWindow(V2 position, V2 dimensions, uint32 color, GUIWindow *target);
void initGUIWindow(V2 position, uint32 color, GUIWindow *target);
void setGUIWindowContent(const jadel::Surface *content, GUIWindow *guiWindow);
void handleGUIWindowInput(GUIWindow *guiWindow);
void updateGUIWindow(GUIWindow *guiWindow);
void renderGUIWindow(GUIWindow *guiWindow);
void setGUIWindowPos(V2 pos, GUIWindow *guiWindow);
void setGUIWindowDimensions(float width, float height, GUIWindow *guiWindow);
void setGUIWindowColor(uint32 color, GUIWindow *guiWindow);
const GUISlider *addSliderToGUIWindow(float minValue, float maxValue, float *modifiableValue, GUIWindow *guiWindow);
void addButtonToGUIWindow(void (*callback)(), GUIWindow *guiWindow);
void modifyGUIWindowButtonGap(float amt, GUIWindow *guiWindow);
float getGUIWindowLeft(const GUIWindow *guiWindow);
float getGUIWindowRight(const GUIWindow *guiWindow);
float getGUIWindowBottom(const GUIWindow *guiWindow);
float getGUIWindowTop(const GUIWindow *guiWindow);
jadel::Rectf getGUIWindowRect(const GUIWindow *guiWindow);
bool isGUIWindowHeaderHovered(const GUIWindow *guiWindow);