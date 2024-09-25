#include "guiwindow.h"
#include "globals.h"
#include "gui.h"
#include "graphics.h"

static float headerHeight = 0.04f;
static float buttonHeight = 0.05f;
static float buttonSideMargin = 0.0f;
static float borderWidth = 0.01f;

static uint32 currentWindowID = 1;

static uint32 buttonIdleColor = 0xff777777;
static uint32 buttonHoverColor = 0xffaaaaaa;

static void setNextGUIButtonPos(Button *button, GUIWindow *guiWindow)
{
    V2 pos(buttonSideMargin,
           guiWindow->lastAddedButtonY - button->clickable.dimensions.y - guiWindow->buttonGap);
    setClickablePosition(pos, &button->clickable);
    guiWindow->lastAddedButtonY = pos.y;
}

static void setRequiredDimensionsFor(GUIWindow *guiWindow)
{
    V2 result(0, 0);

    for (int i = 0; i < guiWindow->numButtons; ++i)
    {
        V2 currentDim = guiWindow->buttons[i].clickable.dimensions;
        if (result.x < currentDim.x)
        {
            result.x = currentDim.x;
        }
        result.y += (currentDim.y + guiWindow->buttonGap);
    }
    setGUIWindowDimensions(result.x + borderWidth * 2.0f, -guiWindow->lastAddedButtonY + headerHeight + borderWidth * 2.0f, guiWindow);
}

static jadel::Rectf getGUIWindowScreenArea(GUIWindow *guiWindow)
{
    V2 start = guiWindow->totalArea.position;
    V2 end = start + guiWindow->totalArea.dimensions;
    jadel::Rectf result(start, end);
    return result;
}

void initGUIWindow(V2 position, uint32 color, GUIWindow *target)
{
    target->id = currentWindowID++;
    float clientHeight = 0.02f;
    target->isHighlighted = false;
    target->numButtons = 0;
    target->buttonGap = 0.0f;
    target->lastAddedButtonY = 0;
    initClickable(&target->totalArea);
    initClickable(&target->client);
    initClickable(&target->header);
    setGUIWindowPos(position, target);
    setClickableParent(&target->totalArea, &target->client, CLICKABLE_CORNER_BOTTOM_LEFT);
    setClickableParent(&target->totalArea, &target->header, CLICKABLE_CORNER_TOP_LEFT);
    setClickableColor(0xffaaaaaa, &target->totalArea);
    setClickablePosition(V2(borderWidth, borderWidth), &target->client);
    setClickablePosition(V2(borderWidth, -borderWidth - headerHeight), &target->header);
    setGUIWindowDimensions(DEFAULT_GUI_WINDOW_WIDTH, clientHeight, target);
    setRequiredDimensionsFor(target);
}

void setGUIWindowContent(const jadel::Surface *content, GUIWindow *guiWindow)
{
    setClickableContent(content, &guiWindow->client);
}

void handleGUIWindowInput(GUIWindow *guiWindow)
{
    bool isMouseLeftClicked = jadel::inputIsMouseLeftClicked();
    uint32 buttonCurrentColor;
    for (int i = 0; i < guiWindow->numButtons; ++i)
    {
        Button *button = &guiWindow->buttons[i];
        if (isClickableHovered(&button->clickable))
        {
            buttonCurrentColor = buttonHoverColor;
            if (isMouseLeftClicked)
            {
                JADEL_DEBUGMSG("Clicked GUIWindow id: %d, button: %d\n", guiWindow->id, i);
                if (button->callback)
                {
                    button->callback();
                }
            }
        }
        else
        {
            buttonCurrentColor = buttonIdleColor;
        }
        setClickableColor(buttonCurrentColor, &guiWindow->buttons[i].clickable);
    }
    for (int i = 0; i < guiWindow->numSliders; ++i)
    {
        GUISlider *slider = &guiWindow->sliders[i];
        updateGUISlider(slider);
    }
}

void updateGUIWindow(GUIWindow *guiWindow)
{
}

void renderGUIWindow(GUIWindow *guiWindow)
{
    pushClippingPlane(getGUIWindowScreenArea(guiWindow));
    uint32 headerColor = (guiWindow->isHighlighted) ? 0xffaa0000 : 0xffaaaa00;

    setClickableColor(headerColor, &guiWindow->header);
    renderClickable(&guiWindow->totalArea);
    renderClickable(&guiWindow->header);
    pushClippingPlane(getClickableWorldArea(&guiWindow->client));
    renderClickable(&guiWindow->client);

    for (int i = 0; i < guiWindow->numButtons; ++i)
    {
        renderClickable(&guiWindow->buttons[i].clickable);
    }
    for (int i = 0; i < guiWindow->numSliders; ++i)
    {
        pushClippingPlane(getClickableWorldArea(&guiWindow->sliders[i].clickable));
        renderSlider(&guiWindow->sliders[i]);
        popClippingPlane();
    }
    popClippingPlane();
    popClippingPlane();
}

void setGUIWindowPos(V2 pos, GUIWindow *guiWindow)
{
    setClickablePosition(pos, &guiWindow->totalArea);
}

void setGUIWindowDimensions(float width, float height, GUIWindow *guiWindow)
{
    V2 clientDim(width - borderWidth * 2.0f, height - headerHeight - borderWidth * 2);
    V2 headerDim(clientDim.x, headerHeight);
    setClickableDimensions(V2(width, height), &guiWindow->totalArea);
    setClickableDimensions(clientDim, &guiWindow->client);
    setClickableDimensions(headerDim, &guiWindow->header);
}

void setGUIWindowColor(uint32 color, GUIWindow *guiWindow)
{
    guiWindow->client.color = color;
}

const GUISlider *addSliderToGUIWindow(float minValue, float maxValue, float *modifiableValue, GUIWindow *guiWindow)
{
    if (guiWindow->numSliders >= MAX_GUI_WINDOW_ELEMENTS_OF_TYPE)
    {
        return NULL;
    }
    GUISlider *slider = &guiWindow->sliders[guiWindow->numSliders++];
    initGUISlider(minValue, maxValue, modifiableValue, slider);
    setClickableDimensions(V2(0.15f, 0.3f), &slider->clickable);
    setClickableParent(&guiWindow->client, &slider->clickable, CLICKABLE_CORNER_TOP_LEFT);
    setClickablePosition(V2(0, -slider->clickable.dimensions.y), &slider->clickable);
    return slider;
}

void addButtonToGUIWindow(void (*callback)(), GUIWindow *guiWindow)
{
    if (guiWindow->numButtons >= MAX_GUI_WINDOW_ELEMENTS_OF_TYPE)
    {
        return;
    }

    int buttonIndex = guiWindow->numButtons++;
    Button *button = &guiWindow->buttons[buttonIndex];

    initButton(callback, button);
    setClickableDimensions(V2(0.5f, buttonHeight), &button->clickable);
    setClickableParent(&guiWindow->client, &button->clickable, CLICKABLE_CORNER_TOP_LEFT);
    setNextGUIButtonPos(button, guiWindow);
    setClickableColor(buttonIdleColor, &button->clickable);
    setRequiredDimensionsFor(guiWindow);
}

void modifyGUIWindowButtonGap(float amt, GUIWindow *guiWindow)
{
    guiWindow->buttonGap += amt;
    if (guiWindow->buttonGap < 0)
    {
        guiWindow->buttonGap = 0;
    }
    else if (guiWindow->buttonGap > 0.3f)
    {
        guiWindow->buttonGap = 0.3f;
    }

    guiWindow->lastAddedButtonY = 0;
    for (int i = 0; i < guiWindow->numButtons; ++i)
    {
        setNextGUIButtonPos(&guiWindow->buttons[i], guiWindow);
    }
    setRequiredDimensionsFor(guiWindow);
}

float getGUIWindowLeft(const GUIWindow *guiWindow)
{
    float result = getClickableWorldLeft(&guiWindow->totalArea);
    return result;
}

float getGUIWindowRight(const GUIWindow *guiWindow)
{
    float result = getClickableWorldRight(&guiWindow->totalArea);
    return result;
}

float getGUIWindowBottom(const GUIWindow *guiWindow)
{
    float result = getClickableWorldBottom(&guiWindow->totalArea);
    return result;
}

float getGUIWindowTop(const GUIWindow *guiWindow)
{
    float result = getClickableWorldTop(&guiWindow->totalArea);
    return result;
}

jadel::Rectf getGUIWindowRect(const GUIWindow *guiWindow)
{
    jadel::Rectf result(getGUIWindowLeft(guiWindow), getGUIWindowBottom(guiWindow),
                        getGUIWindowRight(guiWindow), getGUIWindowTop(guiWindow));
    return result;
}

bool isGUIWindowHeaderHovered(const GUIWindow *guiWindow)
{
    bool result = isClickableHovered(&guiWindow->header);
    return result;
}