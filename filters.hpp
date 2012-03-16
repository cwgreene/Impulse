#ifndef FILTERS_HPP
#define FILTERS_HPP
#include <cv.h>
#include <highgui.h>

using namespace cv;

struct Circuit{
    Mat_<Vec3b> Vin;
    Mat_<Vec3b> Vin_prev;
    
    Mat_<Vec3d> Vout;
    Mat_<Vec3d> dVout;
    Mat_<Vec3b> qVout;

    double iR; // Inverse Resistance
    double iC; // Inverse Capacitance


    double dt; 
};

void highPassTimeStep(Circuit &c, int reduction);
void lowPassTimeStep(Circuit &c, int reduction);
#endif
