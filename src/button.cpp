#include "button.h"

void initButton(void (*callback)(), Button *target)
{
    initClickable(&target->clickable);
    target->callback = callback;
}