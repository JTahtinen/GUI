#pragma once
#include <jadel.h>
#include "defs.h"

enum ClickableSide
{
    CLICKABLE_SIDE_NONE = 0,
    CLICKABLE_SIDE_LEFT = 1 << 0,
    CLICKABLE_SIDE_TOP = 1 << 1,
    CLICKABLE_SIDE_RIGHT = 1 << 2,
    CLICKABLE_SIDE_BOTTOM = 1 << 3
};

enum ClickableCorner
{
    CLICKABLE_CORNER_NONE = 0,
    CLICKABLE_CORNER_BOTTOM_LEFT,
    CLICKABLE_CORNER_TOP_LEFT,
    CLICKABLE_CORNER_TOP_RIGHT,
    CLICKABLE_CORNER_BOTTOM_RIGHT
};

struct Clickable
{
    const Clickable *parent;
    ClickableCorner parentRelativeCorner;
    V2 position;
    V2 dimensions;
    uint32 color;
    const jadel::Surface *content;
};

V2 getClickableWorldPosition(const Clickable *clickable);
jadel::Rectf getClickableWorldArea(const Clickable *clickable);
bool isClickableHovered(const Clickable *clickable);
void setClickableContent(const jadel::Surface *content, Clickable *clickable);
//void renderClickable(const Clickable *clickable);
void initClickable(Clickable *target);
void setClickablePositionX(float x, Clickable *clickable);
void setClickablePositionY(float y, Clickable *clickable);
void setClickablePosition(V2 position, Clickable *clickable);
void setClickableDimensions(V2 dimensions, Clickable *clickable);
void setClickableWidth(float width, Clickable *clickable);
void setClickableHeight(float height, Clickable *clickable);
void setClickableColor(uint32 color, Clickable *clickable);
void setClickableParent(const Clickable *parent, Clickable *clickable, ClickableCorner relativeCorner);
float getClickableWorldLeft(const Clickable *clickable);
float getClickableWorldRight(const Clickable *clickable);
float getClickableWorldBottom(const Clickable *clickable);
float getClickableWorldTop(const Clickable *clickable);
V2 getClickableWorldBottomLeft(const Clickable *clickable);
V2 getClickableWorldTopLeft(const Clickable *clickable);
V2 getClickableWorldTopRight(const Clickable *clickable);
V2 getClickableWorldBottomRight(const Clickable *clickable);
float getClickableLocalLeft(const Clickable *clickable);
float getClickableLocalRight(const Clickable *clickable);
float getClickableLocalBottom(const Clickable *clickable);
float getClickableLocalTop(const Clickable *clickable);
V2 getClickableLocalBottomLeft(const Clickable *clickable);
V2 getClickableLocalTopLeft(const Clickable *clickable);
V2 getClickableLocalTopRight(const Clickable *clickable);
V2 getClickableLocalBottomRight(const Clickable *clickable);