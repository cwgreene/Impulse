#include "cv.h"
#include "highgui.h"
#include <iostream>

using namespace cv;

int print_vec(Vec3b v){
    unsigned int a=v[0],b=v[0],c=v[0];
    std::cout << a << " " << b << " " << c << std::endl;
}

struct Circuit{
    Mat_ *vin;
    Mat_ Vout;

    double iR = 1 ; // Inverse Resistance
    double iC = 1000*1000; // Inverse Capacitance

    Mat_ dVout;

    double dt = 1./30;
};

void timeStep(Circuit &circuit){
    double dt = circuit.dt;
    double iR = circuit.iR;
    double iC = circuit.iC;
    for(int i = 0; i < circuit.Vin.rows; i++){
        for(int j = 0; j < circuit.Vout.cols; j++){
            for(int index = 0; i < 3;index++){
                double _Vout = Vout(i,j)[index];
                double _dVout = dVout(i,j)[index];
                double _ddVout = (*Vin)(i,j)[index]-Vout(i,j)[index]*iC*iR;
    
                Vout(i,j)[index] = _Vout+_dVout*dt;
                dVout(i,j)[index] = _dVout+_ddVout*dt;
            }
        }
    }
}

double colors

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
    cap >> frame; if(! frame.data) break;
    circuit.Vout = frame;
    circuit.dVout = zeros(frame.rows,frame.cols);
    for(;;){
        cap >> frame; if(!frame.data) break;
        if(prev_frame.data){
            int count = 0;
            for(int i = 0; i < frame.rows; i++){
                for (int j = 0 ; j < frame.cols;j++){
                    // What? The format seems to be BGR. :(
                    // Endianess issue maybe?
                    if(frame(i,j) == prev_frame(i,j))
                        count +=1;
                    for(int index = 0; i < 3;i++){
                        if(frame(i,j)[0] != prev_frame(i,j)[0])
                        {
                            frame(i,j)[0]=0;
                            frame(i,j)[1]=0;
                            frame(i,j)[2]=0;
                        }
                   }
                }
            }
            std::cout << count << std::endl;
        }
        imshow("video", frame); if(waitKey(30) >= 0) break;
        prev_frame = frame;
    }
}
