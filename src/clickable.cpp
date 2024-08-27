#include "clickable.h"
#include "gui.h"
#include "graphics.h"

#define DEFAULT_CLICKABLE_COLOR (0xff222222);

static V2 getClickableLocalCorner(const Clickable *clickable, ClickableCorner corner)
{
    V2 result;
    switch (corner)
    {
    case CLICKABLE_CORNER_NONE:
    case CLICKABLE_CORNER_BOTTOM_LEFT:
        result = getClickableLocalBottomLeft(clickable);
        break;
    case CLICKABLE_CORNER_TOP_LEFT:
        result = getClickableLocalTopLeft(clickable);
        break;
    case CLICKABLE_CORNER_TOP_RIGHT:
        result = getClickableLocalTopRight(clickable);
        break;
    case CLICKABLE_CORNER_BOTTOM_RIGHT:
        result = getClickableLocalBottomRight(clickable);
        break;
    }
    return result;
}

V2 getClickableWorldPosition(const Clickable *clickable)
{
    V2 result = clickable->position;
    const Clickable *currentClickable = clickable;
    const Clickable *currentParent = clickable->parent;
    while (currentParent)
    {
        result += getClickableLocalCorner(currentParent, currentClickable->parentRelativeCorner);
        currentClickable = currentParent;
        currentParent = currentClickable->parent;
    }
    return result;
}

jadel::Rectf getClickableWorldArea(const Clickable *clickable)
{
    V2 start = getClickableWorldPosition(clickable);
    V2 end = start + clickable->dimensions;
    jadel::Rectf result(start, end);
    return result;
}

bool isClickableHovered(const Clickable *clickable)
{
    jadel::Rectf clickableArea = getClickableWorldArea(clickable);
    bool result = jadel::pointfWithinRectf(getMouseProjectedPos(), clickableArea);
    return result;
}

void setClickableContent(const jadel::Surface *content, Clickable *clickable)
{
    clickable->content = content;
}

void initClickable(Clickable *target)
{
    target->parent = NULL;
    target->parentRelativeCorner = CLICKABLE_CORNER_NONE;
    target->position = V2(0, 0);
    target->dimensions = V2(0, 0);
    target->color = DEFAULT_CLICKABLE_COLOR;
    target->content = NULL;
}

void setClickablePositionX(float x, Clickable *clickable)
{
    clickable->position.x = x;
}

void setClickablePositionY(float y, Clickable *clickable)
{
    clickable->position.y = y;
}

void setClickablePosition(V2 position, Clickable *clickable)
{
    setClickablePositionX(position.x, clickable);
    setClickablePositionY(position.y, clickable);
}

void setClickableDimensions(V2 dimensions, Clickable *clickable)
{
    clickable->dimensions = dimensions;
}

void setClickableWidth(float width, Clickable *clickable)
{
    clickable->dimensions.x = width;
}

void setClickableHeight(float height, Clickable *clickable)
{
    clickable->dimensions.y = height;
}

void setClickableColor(uint32 color, Clickable *clickable)
{
    clickable->color = color;
}

void setClickableParent(const Clickable *parent, Clickable *clickable, ClickableCorner relativeCorner)
{
    clickable->parent = parent;
    clickable->parentRelativeCorner = relativeCorner;
}

float getClickableWorldLeft(const Clickable *clickable)
{
    float result = getClickableWorldArea(clickable).x0;
    // float result = clickable->position.x;
    return result;
}

float getClickableWorldRight(const Clickable *clickable)
{
    float result = getClickableWorldArea(clickable).x1;
    // float result = clickable->position.x + clickable->dimensions.x;
    return result;
}

float getClickableWorldBottom(const Clickable *clickable)
{
    float result = getClickableWorldArea(clickable).y0;
    return result;
}

float getClickableWorldTop(const Clickable *clickable)
{
    float result = getClickableWorldArea(clickable).y1;
    return result;
}

V2 getClickableWorldBottomLeft(const Clickable *clickable)
{
    jadel::Rectf finalArea = getClickableWorldArea(clickable);
    V2 result(finalArea.x0, finalArea.y0);
    return result;
}

V2 getClickableWorldTopLeft(const Clickable *clickable)
{
    jadel::Rectf finalArea = getClickableWorldArea(clickable);
    V2 result(finalArea.x0, finalArea.y1);
    return result;
}

V2 getClickableWorldTopRight(const Clickable *clickable)
{
    jadel::Rectf finalArea = getClickableWorldArea(clickable);
    V2 result(finalArea.x1, finalArea.y1);
    return result;
}

V2 getClickableWorldBottomRight(const Clickable *clickable)
{
    jadel::Rectf finalArea = getClickableWorldArea(clickable);
    V2 result(finalArea.x1, finalArea.y0);
    return result;
}

float getClickableLocalLeft(const Clickable *clickable)
{
    float result = clickable->position.x;
    return result;
}

float getClickableLocalRight(const Clickable *clickable)
{
    float result = clickable->position.x + clickable->dimensions.x;
    return result;
}

float getClickableLocalBottom(const Clickable *clickable)
{
    float result = clickable->position.y;
    return result;
}

float getClickableLocalTop(const Clickable *clickable)
{
    float result = clickable->position.y + clickable->dimensions.y;
    return result;
}

V2 getClickableLocalBottomLeft(const Clickable *clickable)
{
    V2 result(getClickableLocalLeft(clickable), getClickableLocalBottom(clickable));
    return result;
}

V2 getClickableLocalTopLeft(const Clickable *clickable)
{
    V2 result(getClickableLocalLeft(clickable), getClickableLocalTop(clickable));
    return result;
}

V2 getClickableLocalTopRight(const Clickable *clickable)
{
    V2 result(getClickableLocalRight(clickable), getClickableLocalTop(clickable));
    return result;
}

V2 getClickableLocalBottomRight(const Clickable *clickable)
{
    V2 result(getClickableLocalRight(clickable), getClickableLocalBottom(clickable));
    return result;
}