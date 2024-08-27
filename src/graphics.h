#pragma once
#include <jadel.h>
#include "clickable.h"
#include "defs.h"

jadel::Rectf getProjectedRect(const jadel::Rectf originalRect);
void renderSurface(jadel::Rectf area, const jadel::Surface *surface);
void renderRect(jadel::Rectf rect, uint32 color);
//void renderClickable(const Clickable *parent, ClickableCorner relativeCorner, const Clickable *clickable);
void renderClickable(const Clickable *clickable);
void renderBorder(V2 position, V2 dimensions, float thickness, uint32 color);
void pushClippingPlane(jadel::Rectf clippingPlane);
void popClippingPlane();