#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

class FpsCounter
{
    private:
        double time;
        int framecount;
    public:
        double fps;
        FpsCounter();
        void update(double time);
};

#endif

