#include <jadel.h>
#include <thread>
#include "globals.h"
#include "gui.h"

static float frameTime;
static jadel::Window window;
static jadel::Surface windowSurface;

static int screenWidth = 1280;
static int screenHeight = 720;
// static int screenWidth = 640;
// static int screenHeight = 480;


void render()
{
    g_graphics.clearTargetSurface();
    renderGUI();

    // renderGUIWindow(&guiWindow);
    // renderGUIWindow(&guiWindow1);
    jadel::windowUpdate(&window, &windowSurface);
}

void tick()
{
    if (jadel::inputIsKeyTyped(jadel::KEY_ESCAPE))
    {
        exit(0);
    }
    updateGUI();
}

void init()
{
    if (!initGlobals())
    {
        exit(0);
    }
    g_aspect = (float)screenWidth / (float)screenHeight;
    
    g_worldToScreenProjMatrix = jadel::Mat3(1.0f / g_aspect, 0, 0,
                                 0, 1.0f, 0,
                                 0, 0, 1.0f);

    g_screenToWorldProjMatrix = jadel::Mat3(g_aspect, 0, 0, 0, 1.0f, 0, 0, 0, 1.0f);

    jadel::windowCreate(&window, "GUI", screenWidth, screenHeight);
    jadel::graphicsCreateSurface(screenWidth, screenHeight, &windowSurface);
    g_graphics.pushTargetSurface(&windowSurface);
    g_graphics.setClearColor(0);
    initGUI();

}

int JadelMain(int argc, char **argv)
{
    JadelInit(MB(10));
    if (!JadelIsInitialized())
    {
        return -1;
    }

    jadel::allocateConsole();
    jadel::message("Hello, Jadel!\n\n");

    jadel::Timer frameTimer;
    frameTimer.start();
    uint32 elapsedInMillis = 0;
    uint32 minFrameTime = 1000 / 165;
    uint32 accumulator = 0;
    int frames = 0;
    frameTime = 0;
    init();

    render();
    while (true)
    {
        JadelUpdate();
        render();
        tick();

        ++frames;
        elapsedInMillis = frameTimer.getMillisSinceLastCheck();
        // jadel::message("%d\n", elapsedInMillis);
        if (elapsedInMillis > 0)
        {
            frameTime = (float)elapsedInMillis * 0.001f;
        }
        accumulator += elapsedInMillis;

#ifdef DEBUG
        if (accumulator >= 1000)
        {
            accumulator %= 1000;
            // jadel::message("FPS: %d\n", framesPerSecond);
            frames = 0;
        }
#endif
        if (elapsedInMillis < minFrameTime)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(minFrameTime - elapsedInMillis));
        }

        // uint32 debugTime = frameTimer.getMillisSinceLastUpdate();
        // jadel::message("%f\n", frameTime);

        // frameTimer.update();
    }
    return 0;
}