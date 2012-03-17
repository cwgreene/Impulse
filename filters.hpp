#ifndef FILTERS_HPP
#define FILTERS_HPP
#include <cv.h>
#include <highgui.h>

using namespace cv;

struct Circuit{
    Mat_<Vec3d> Vin;
    Mat_<Vec3d> Vin_prev;
    
    Mat_<Vec3d> Vout;
    Mat_<Vec3d> dVout;
    Mat_<Vec3b> qVout;

    double iR; // Inverse Resistance
    double iC; // Inverse Capacitance


    double dt; 
};

struct LowPass:Circuit{
    LowPass(int rows,int cols, double iR, double iC, double dt, int reduce);
};

struct HighPass:Circuit{
    HighPass(int rows,int cols, double iR, double iC, double dt, int reduce);
};

void highPassTimeStep(Circuit &c, int reduction);
void lowPassTimeStep(Circuit &c, int reduction);
#endif
