#include "util.h"
#include "globals.h"

V2 getScreenProjectedDiff(V2 start, V2 end)
{
    V2 projectedStart = g_screenToWorldProjMatrix * V2(start.x, start.y);
    V2 projectedEnd = g_screenToWorldProjMatrix * V2(end.x, -end.y);
    V2 result = projectedEnd - projectedStart;
    return result;
}

jadel::Rectf getAreaAroundPoint(V2 point, float radius)
{
    jadel::Rectf result(point.x - radius, point.y - radius, point.x + radius, point.y + radius);
    return result;
}