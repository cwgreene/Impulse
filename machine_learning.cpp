#include <cv.h>
#include <stdio.h>
#include <highgui.h>
#include <iostream>

#include "filters.hpp"

using namespace cv;

int print_vec(Vec3b v){
    unsigned int a=v[0],b=v[0],c=v[0];
    std::cout << a << " " << b << " " << c << std::endl;
}

template<class T,class T2>
void copyEach(Mat_<T> &a, Mat_<T2> &b, int n){
    for(int i = 0; i < a.rows; i += n){
        for(int j = 0; j < a.cols; j += n){
            for(int index = 0; index < 3; index++){
                b(i/n,j/n)[index] = a(i,j)[index];
            }
        }
    }
}

void skipAhead(int n, VideoCapture &cap){
    Mat_<Vec3b> m;
    for(int i = 0; i < n; i+=100){
        cap >> m;
        if(i % 100 == 1){
            std::cout << "'Skipping' " << i << std::endl;
            imshow("video", m); if(waitKey(30) >= 0) break;
        }
    }
}

int main(int argc, char **argv){
    int reduce = 1;
    int skip = 0;
    if(argc < 2){
        printf("usage: machinelearning MOVIE_FILE\n");
        return -1;
    }
    if(argc >= 3)
        reduce = atoi(argv[2]);
    if(argc >= 4)
        skip = atoi(argv[3]);
    VideoCapture cap;
    cap.open(argv[1]);
    Mat_<Vec3b> frame; 
    Mat_<Vec3b> prev_frame; 
    namedWindow("video",1);
   
    //Init Circuit 
    //Get first frame
    int frame_count = 0;
    cap >> frame; if(! frame.data) return -1;

    //Initialize voltages to correct sizes
    LowPass circuit1(frame.rows,frame.cols,0.05,10.,1.0, reduce);
    HighPass circuit2(frame.rows,frame.cols,0.05,2.0,1.0, reduce);

    skipAhead(skip, cap);

    copyEach(frame,circuit2.qVout,reduce);

    //frame.copyTo(circuit.qVout);
    copyEach(frame,circuit2.Vin, 1);
    highPassTimeStep(circuit2, reduce);
    copyEach(circuit2.Vout,circuit1.Vin,reduce);
    copyEach(circuit1.Vin,circuit1.Vin_prev,reduce);

    int peekx= 100;
    int peeky= 100;

    std::cout << (int)circuit1.qVout(peekx,peeky)[1]
              << '/' <<circuit1.Vout(peekx,peeky)[1] 
              << std::endl;
    for(;;){
        copyEach(frame,circuit2.Vin_prev, 1);
        std::cout <<frame_count<<" ";
        frame_count++;
        cap >> frame;
        
        std::cout << (int)frame(peekx,peeky)[1] << " ";
        copyEach(frame,circuit2.Vin, 1);
        highPassTimeStep(circuit2,reduce);
        copyEach(circuit1.Vin,circuit1.Vin_prev, reduce);
        copyEach(circuit2.Vout,circuit1.Vin, reduce);
        lowPassTimeStep(circuit2,reduce);
        std::cout << (int)circuit1.qVout(peekx,peeky)[1]
                  << '/' <<circuit1.Vout(peekx,peeky)[1] 
                  << std::endl;
        if(frame_count %2)
        imshow("video", circuit2.qVout); if(waitKey(30) >= 0) break;
    }
}
