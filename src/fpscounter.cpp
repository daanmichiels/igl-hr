#include "fpscounter.h"
#include <iostream>
#include <string>

FpsCounter::FpsCounter(bool verbose)
{
    _verbose = verbose;
    _fps = 0.0f;
    _last_time = 0.0f;
    _framecount = 0;
}

void FpsCounter::update(double time)
{
    if(time >= _last_time + 1.0)
    {
        _last_time += 1.0;
        _fps = _framecount;
        if(_verbose)
            std::cout << std::to_string(_framecount) << "\n";
        _framecount = 0;
    }
    _framecount++;
}

double FpsCounter::get_fps()
{
    return _fps;
}

