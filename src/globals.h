#pragma once
#include <jadel.h>

extern jadel::Graphics g_graphics;
extern float g_aspect;
extern jadel::Mat3 g_worldToScreenProjMatrix;
extern jadel::Mat3 g_screenToWorldProjMatrix;

bool initGlobals();