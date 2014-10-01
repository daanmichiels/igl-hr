#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

class FpsCounter
{
    private:
        bool _verbose;
        double _last_time;
        double _fps;
        int _framecount;
    public:
        FpsCounter(bool verbose);
        void update(double time);
        double get_fps();
};

#endif

