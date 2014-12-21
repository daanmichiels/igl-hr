#include "fpscounter.h"
#include <iostream>
#include <string>

FpsCounter::FpsCounter()
{
    fps = 0.0f;
    time = 0.0f;
    framecount = 0;
}

void FpsCounter::update(double dt)
{
    time += dt;
    if(time >= 1.0)
    {
        time -= 1.0;
        fps = framecount;
        framecount = 0;
    }
    framecount++;
}


