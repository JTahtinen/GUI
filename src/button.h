#pragma once
#include "clickable.h"
#include "defs.h"

struct Button
{
    Clickable clickable;
    void (*callback)();    
};

void initButton(void (*callback)(), Button *target);