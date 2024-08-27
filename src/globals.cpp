#include <jadel.h>
jadel::Graphics g_graphics;
float g_aspect;
jadel::Mat3 g_worldToScreenProjMatrix;
jadel::Mat3 g_screenToWorldProjMatrix;

bool initGlobals()
{
    bool result = g_graphics.init();
    if (!result)
    {
        return false;
    }
    g_graphics.setFlagEnabled(jadel::JADEL_GRAPHICS_BLIT_FILTER_BILINEAR);
    return result;
}