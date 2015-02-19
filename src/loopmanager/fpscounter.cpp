#include "fpscounter.h"
#include <iostream>
#include <string>

/** \brief Setup the fps counter
 * \param void
 * \return void
 */
FpsCounter::FpsCounter()
{
    fps = 0.0f;
    time = 0.0f;
    framecount = 0;
}

/** \brief Update the current fps
 * \param deltatime as a double
 * \return void
 */
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


