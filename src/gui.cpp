#include "gui.h"
#include "guiwindow.h"
#include "globals.h"
#include "graphics.h"
#include "defs.h"
#include "guislider.h"
#include "util.h"

#define TOP_WINDOW guiWindows[guiWindowIndices[0]]

enum GUIState
{
    GUI_STATE_IDLE,
    GUI_STATE_MOVE_WINDOW,
    GUI_STATE_DRAG_RESIZE_WINDOW
};

GUIState currentState;

static GUIWindow guiWindows[10];
static int guiWindowIndices[10];
static int numGUIWindows = 0;
static bool topWindowHooked = false;
static V2 topWindowHookRelativePosition;

static V2 dragResizeHookPosition;
static float dragResizeHookWidth;
static float dragResizeHookHeight;
static uint32 dragResizeSideFlags;

V2 projectedMousePos;

const GUISlider *redSlider;

static float winR = 0.6f;
static float winG = 0.6f;
static float winB = 0.6f;
static float knobRadius = 0.02f;
static float minimumGUIWindowWidth = 0.02f;

static jadel::Surface testSurface;

static GUIWindow *getGUIWindow(int index)
{
    GUIWindow *result = &guiWindows[guiWindowIndices[index]];
    return result;
}

static void hookTopWindowToCursor()
{
    topWindowHooked = true;
    topWindowHookRelativePosition = getClickableWorldBottomLeft(&TOP_WINDOW.totalArea) - projectedMousePos;
}

static void switchToState(GUIState state)
{
    JADEL_DEBUGMSG("Current state: ");
    switch (state)
    {
    case GUI_STATE_IDLE:
    {
        currentState = GUI_STATE_IDLE;
        JADEL_DEBUGMSG("GUI_STATE_IDLE\n");
        break;
    }
    case GUI_STATE_DRAG_RESIZE_WINDOW:
    {
        currentState = GUI_STATE_DRAG_RESIZE_WINDOW;
        JADEL_DEBUGMSG("GUI_STATE_RESIZE_WINDOW\n");
        TOP_WINDOW.isHighlighted = false;
        dragResizeHookPosition = TOP_WINDOW.totalArea.position;
        V2 dim = TOP_WINDOW.totalArea.dimensions;
        dragResizeHookWidth = dim.x;
        dragResizeHookHeight = dim.y;
        break;
    }
    case GUI_STATE_MOVE_WINDOW:
    {
        currentState = GUI_STATE_MOVE_WINDOW;
        JADEL_DEBUGMSG("GUI_STATE_MOVE_WINDOW\n");
        hookTopWindowToCursor();
        break;
    }
    default:
    {
        break;
    }
    }
}

static void dragResize()
{
    // jadel::message("Resizing");
    V2 mousePos = getMouseProjectedPos();
    float newX = TOP_WINDOW.totalArea.position.x;
    float newY = TOP_WINDOW.totalArea.position.y;
    float newWidth = TOP_WINDOW.totalArea.dimensions.x;
    float newHeight = TOP_WINDOW.totalArea.dimensions.y;
    // Temporary
    float minimumGUIWindowHeight = TOP_WINDOW.header.dimensions.y + 0.01f;
    if (dragResizeSideFlags & CLICKABLE_SIDE_LEFT)
    {
        newWidth = dragResizeHookWidth - (mousePos.x - dragResizeHookPosition.x);
        if (newWidth < minimumGUIWindowWidth)
        {
            newX = dragResizeHookPosition.x + dragResizeHookWidth - minimumGUIWindowWidth;
            newWidth = minimumGUIWindowWidth;
        }
        else
        {
            newX = mousePos.x;
        }
        // jadel::message(" left");
    }
    else if (dragResizeSideFlags & CLICKABLE_SIDE_RIGHT)
    {
        newWidth = mousePos.x - dragResizeHookPosition.x;
        // jadel::message(" right");
    }
    if (dragResizeSideFlags & CLICKABLE_SIDE_BOTTOM)
    {
        newHeight = dragResizeHookHeight - (mousePos.y - dragResizeHookPosition.y);
        if (newHeight < minimumGUIWindowHeight)
        {
            newY = dragResizeHookPosition.y + dragResizeHookHeight - minimumGUIWindowHeight;
            newHeight = minimumGUIWindowHeight;
        }
        else
        {
            newY = mousePos.y;
        }
        // jadel::message(" bottom");
    }
    else if (dragResizeSideFlags & CLICKABLE_SIDE_TOP)
    {
        newHeight = mousePos.y - dragResizeHookPosition.y;
        // jadel::message(" top");
    }
    // jadel::message("\n");
    newWidth = JADEL_CLAMP_MIN(newWidth, minimumGUIWindowWidth);
    newHeight = JADEL_CLAMP_MIN(newHeight, minimumGUIWindowHeight);
    setGUIWindowPos(V2(newX, newY), &TOP_WINDOW);
    setGUIWindowDimensions(newWidth, newHeight, &TOP_WINDOW);
}

static void exitProgram()
{
    exit(0);
}

static GUIWindow *registerGUIWindow(float posX, float posY, uint32 color)
{
    if (numGUIWindows >= 10)
    {
        return NULL;
    }
    GUIWindow *result = &guiWindows[numGUIWindows];
    guiWindowIndices[numGUIWindows] = numGUIWindows;
    ++numGUIWindows;
    initGUIWindow(V2(posX, posY), color, result);
    return result;
}

static void bringGUIWindowToFront(int index)
{
    int guiWindowIndex = guiWindowIndices[index];
    memmove(&guiWindowIndices[1], &guiWindowIndices[0], index * sizeof(int));
    guiWindowIndices[0] = guiWindowIndex;
}

static void handleIdleState()
{
    bool isMouseLeftClicked = jadel::inputIsMouseLeftClicked();
    if (isMouseLeftClicked)
    {
        bool hookDragResize = false;
        V2 mousePos = getMouseProjectedPos();
        float left = getGUIWindowLeft(&TOP_WINDOW);
        float right = getGUIWindowRight(&TOP_WINDOW);
        float bottom = getGUIWindowBottom(&TOP_WINDOW);
        float top = getGUIWindowTop(&TOP_WINDOW);
        static float dragResizeRadius = 0.01f;
        if (jadel::pointfWithinRectf(mousePos, jadel::Rectf(left, bottom, right, top)))
        {
            dragResizeSideFlags = 0;
            if (JADEL_IS_VAL_BETWEEN(mousePos.x, left, left + dragResizeRadius))
            {
                dragResizeSideFlags |= CLICKABLE_SIDE_LEFT;
                hookDragResize = true;
            }
            else if (JADEL_IS_VAL_BETWEEN(mousePos.x, right - dragResizeRadius, right))
            {
                dragResizeSideFlags |= CLICKABLE_SIDE_RIGHT;
                hookDragResize = true;
            }
            if (JADEL_IS_VAL_BETWEEN(mousePos.y, bottom, bottom + dragResizeRadius))
            {
                dragResizeSideFlags |= CLICKABLE_SIDE_BOTTOM;
                hookDragResize = true;
            }
            else if (JADEL_IS_VAL_BETWEEN(mousePos.y, top - dragResizeRadius, top))
            {
                dragResizeSideFlags |= CLICKABLE_SIDE_TOP;
                hookDragResize = true;
            }
        }

        if (hookDragResize)
        {
            switchToState(GUI_STATE_DRAG_RESIZE_WINDOW);
            return;
        }
    }

    handleGUIWindowInput(&TOP_WINDOW);
    for (int i = 0; i < numGUIWindows; ++i)
    {
        GUIWindow *guiWindow = getGUIWindow(i);
        guiWindow->isHighlighted = false;
        updateGUIWindow(guiWindow);
    }

    for (int i = 0; i < numGUIWindows; ++i)
    {
        GUIWindow *guiWindow = getGUIWindow(i);
        bool windowHovered = isClickableHovered(&guiWindow->totalArea);
        bool headerHovered = isGUIWindowHeaderHovered(guiWindow);
        // bool clientHovered = isClickableHovered(&guiWindows[i].client);
        if (windowHovered || headerHovered)
        {
            if (headerHovered)
            {
                guiWindow->isHighlighted = true;
            }
            if (isMouseLeftClicked && i > 0)
            {
                bringGUIWindowToFront(i);
            }
            break;
        }
    }

    if (isGUIWindowHeaderHovered(&TOP_WINDOW))
    {
        if (isMouseLeftClicked)
        {
            switchToState(GUI_STATE_MOVE_WINDOW);
            return;
        }
    }
    int mWheel = jadel::inputGetMouseWheelScrolls();
    if (mWheel != 0)
    {
        modifyGUIWindowButtonGap(-(mWheel * 0.005f), &TOP_WINDOW);
    }
    if (redSlider->valueChanged)
    {
        for (int i = 0; i < numGUIWindows; ++i)
        {
            setGUIWindowColor(jadel::Color(1.0f, winR, 0, 0).toU32(), &guiWindows[i]);
        }
    }
    /*
    for (int i = 0; i < numSliders; ++i)
    {
        updateGUISlider(&sliders[i]);
    }*/
}

static void handleDragResizeState()
{
    if (jadel::inputIsMouseLeftReleased())
    {
        switchToState(GUI_STATE_IDLE);
        return;
    }
    dragResize();
}

static void handleMoveWindowState()
{
    TOP_WINDOW.isHighlighted = true;
    if (jadel::inputIsMouseLeftReleased())
    {
        switchToState(GUI_STATE_IDLE);
        return;
    }
    else
    {
        setGUIWindowPos(projectedMousePos + topWindowHookRelativePosition, &TOP_WINDOW);
    }
}

void updateGUI()
{
    projectedMousePos = g_screenToWorldProjMatrix * jadel::inputGetMouseRelative();
    if (numGUIWindows == 0)
    {
        return;
    }
    bool isMouseLeftClicked = jadel::inputIsMouseLeftClicked();

    switch (currentState)
    {
    case GUI_STATE_IDLE:
        handleIdleState();
        break;
    case GUI_STATE_MOVE_WINDOW:
        handleMoveWindowState();
        break;
    case GUI_STATE_DRAG_RESIZE_WINDOW:
        handleDragResizeState();
        break;
    }
}

void renderGUI()
{
    /*
    if (sliders[0].valueChanged || sliders[1].valueChanged || sliders[2].valueChanged)
    {
        for (int i = 0; i < numGUIWindows; ++i)
        {
            setGUIWindowColor(jadel::Color(1.0f, winR, winG, winB).toU32(), &guiWindows[i]);
        }
    }*/
    // Render in reverse order, as lower indices are on the foreground
    for (int i = numGUIWindows - 1; i >= 0; --i)
    {
        renderGUIWindow(getGUIWindow(i));
    }
    /*
        for (int i = 0; i < numSliders; ++i)
        {
            GUISlider *slider = &sliders[i];
            float knobRadius = slider->clickable.dimensions.x * 0.5f;

            renderClickable(&slider->clickable);
            renderBorder(slider->clickable.position, slider->clickable.dimensions, knobRadius, 0xffffffff);
            V2 sliderBottomLeft = getClickableBottomLeft(&slider->clickable);
            V2 knobPos(slider->clickable.position.x, (sliderBottomLeft.y + slider->internalValue * slider->clickable.dimensions.y) - knobRadius);
            renderRect(jadel::Rectf(knobPos.x, knobPos.y, knobPos.x + 2.0f * knobRadius, knobPos.y + 2.0f * knobRadius), 0xffff0000);
        }*/
}

void testFunc()
{
    jadel::message("Click!\n");
}

void initGUI()
{
    bool result = jadel::loadPNG("res/mediocre.png", &testSurface);
    if (!result)
    {
        exitProgram();
    }
    winR = 0.6f;
    winG = 0.6f;
    winB = 0.6f;
    /* GUISlider sliderR;
     GUISlider sliderG;
     GUISlider sliderB;
     initGUISlider(V2(0.0f, 0.5f), 0.2f, 0, 1.0f, &winR, &sliderR);
     initGUISlider(V2(0.3f, 0.5f), 0.2f, 0, 1.0f, &winG, &sliderG);
     initGUISlider(V2(0.6f, 0.5f), 0.2f, 0, 1.0f, &winB, &sliderB);
     registerGUISlider(sliderR);
     registerGUISlider(sliderG);
     registerGUISlider(sliderB);
 */
    for (int i = 0; i < 5; ++i)
    {
        GUIWindow *guiWindow = registerGUIWindow(-0.5f + (float)i * 0.1f, -0.5f + (float)i * 0.1f, jadel::Color(1, winR, winG, winB).toU32());
        if (i == 0 || i == 4)
            continue;
        addButtonToGUIWindow(testFunc, guiWindow);
        addButtonToGUIWindow(testFunc, guiWindow);
        addButtonToGUIWindow(exitProgram, guiWindow);
    }
    redSlider = addSliderToGUIWindow(0, 1.0f, &winR, &guiWindows[4]);
    setGUIWindowDimensions(0.4, 0.6f, &guiWindows[4]);
    setGUIWindowContent(&testSurface, &TOP_WINDOW);
    float surfaceAspect = (float)testSurface.width / (float)testSurface.height;
    setGUIWindowDimensions(0.9f * surfaceAspect, 0.9f, &TOP_WINDOW);
}

V2 getMouseProjectedPos()
{
    return projectedMousePos;
}