#include "graphics.h"
#include "globals.h"
#include "gui.h"

jadel::Rectf getProjectedRect(const jadel::Rectf originalRect)
{
    V2 projectedStart = g_worldToScreenProjMatrix * V2(originalRect.x0, originalRect.y0);
    V2 projectedEnd = g_worldToScreenProjMatrix * V2(originalRect.x1, originalRect.y1);
    jadel::Rectf result(projectedStart, projectedEnd);
    return result;
}

void renderSurface(jadel::Rectf area, const jadel::Surface *surface)
{
    jadel::Rectf projectedArea = getProjectedRect(area);
    g_graphics.blitRelative(surface, projectedArea);
}

void renderRect(jadel::Rectf rect, uint32 color)
{
    jadel::Rectf projectedRect = getProjectedRect(rect);
    g_graphics.drawRectRelative(projectedRect, color);
}

void renderRect(V2 start, V2 end, uint32 color)
{
    renderRect(jadel::Rectf(start, end), color);
}

/*void renderClickable(const Clickable *parent, ClickableCorner relativeCorner, const Clickable *clickable)
{
    jadel::Rectf area = getClickableAreaParentRelative(parent, relativeCorner, clickable);
    if (clickable->content)
    {
        renderSurface(area, clickable->content);
    }
    else
    {
        renderRect(area, clickable->color);
    }
}*/

void renderClickable(const Clickable *clickable)
{
    jadel::Rectf area = getClickableWorldArea(clickable);
    if (clickable->content)
    {
        renderSurface(area, clickable->content);
    }
    else
    {
        renderRect(area, clickable->color);
    }
}

void renderBorder(V2 position, V2 dimensions, float thickness, uint32 color)
{
    V2 leftStart(position.x - thickness, position.y - thickness);
    V2 leftEnd(position.x, position.y + dimensions.y + thickness);

    V2 topStart(position.x, position.y + dimensions.y);
    V2 topEnd(topStart.x + dimensions.x, topStart.y + thickness);

    V2 rightStart(position.x + dimensions.x, position.y - thickness);
    V2 rightEnd(rightStart.x + thickness, position.y + dimensions.y + thickness);

    V2 bottomStart(position.x, position.y - thickness);
    V2 bottomEnd(bottomStart.x + dimensions.x, bottomStart.y + thickness);

    renderRect(leftStart, leftEnd, color);
    renderRect(topStart, topEnd, color);
    renderRect(rightStart, rightEnd, color);
    renderRect(bottomStart, bottomEnd, color);
}

void pushClippingPlane(jadel::Rectf clippingPlane)
{
    g_graphics.pushClippingPlane(g_worldToScreenProjMatrix * clippingPlane);
}

void popClippingPlane()
{
    g_graphics.popClippingPlane();
}