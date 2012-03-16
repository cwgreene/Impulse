#include "cv.h"
#include "highgui.h"
#include <iostream>

using namespace cv;

int print_vec(Vec3b v){
    unsigned int a=v[0],b=v[0],c=v[0];
    std::cout << a << " " << b << " " << c << std::endl;
}

struct Circuit{
    Mat_<Vec3b> Vin;
    Mat_<Vec3b> Vin_prev;
    Mat_<Vec3d> Vout;
    Mat_<Vec3b> qVout;

    double iR; // Inverse Resistance
    double iC; // Inverse Capacitance

    Mat_<Vec3d> dVout;

    double dt; 
};

void timeStep(Circuit &c){
    double dt = c.dt;
    double iR = c.iR;
    double iC = c.iC;
    for(int i = 0; i < c.Vin.rows; i++){
        for(int j = 0; j < c.Vin.cols; j++){
            for(int index = 0; index < 3;index++){
                double _Vout = c.Vout(i,j)[index];
                double _dVout = c.dVout(i,j)[index];
                double _ddVout = ((c.Vin(i,j)[index]
                                    -c.Vin_prev(i,j)[index])
                                    -_dVout)*iR*iC;
    
                c.Vout(i,j)[index] = _Vout+_dVout*dt;
                c.dVout(i,j)[index] = _dVout+_ddVout*dt;
                c.qVout(i,j)[index] = _Vout;
            }
        }
    }
}

int main(int argc, char **argv){
    if(argc < 2){
        printf("usage: machinelearning MOVIE_FILE\n");
        return -1;
    }
    VideoCapture cap;
    cap.open(argv[1]);
    Mat_<Vec3b> frame; 
    Mat_<Vec3b> prev_frame; 
    namedWindow("video",1);
   
    //Init Circuit 
    Circuit circuit;
    cap >> frame; if(! frame.data) return -1;
    circuit.Vout =Mat::zeros(frame.rows,frame.cols,CV_32F); 
    circuit.qVout = frame;
    circuit.dVout = Mat::zeros(frame.rows,frame.cols,CV_32F);
    circuit.iR = 1/1000.;
    circuit.iC = 1000;
    circuit.dt = 1/5.;
    int frame_count = 0;
    circuit.Vin=frame;
    circuit.Vin_prev = Mat::zeros(frame.rows,frame.cols,CV_32F);
    timeStep(circuit);
    for(;;){
        circuit.Vin_prev = frame;
//        std::cout <<frame_count<<std::endl;
        frame_count++;
        cap >> frame;
        
        circuit.Vin = frame;
        std::cout <<frame(1,1)[0];
        timeStep(circuit);
        std::cout <<frame(1,1)[0]<<std::endl;

        imshow("video", frame); if(waitKey(30) >= 0) break;
    }
}
