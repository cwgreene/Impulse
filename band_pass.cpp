#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "filters.hpp"
#include "options.h"

using namespace cv;

void print_vec(Vec3b v){
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
    int frame_offset;
    int reduce;
    long long max_frames;
    std::string output_file;
    VideoCapture cap;

    handle_options(argc, argv, frame_offset, reduce, max_frames, output_file, cap);

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

    skipAhead(frame_offset, cap);

    copyEach(frame,circuit2.qVout, reduce);
    copyEach(frame,circuit2.Vin, 1);

    highPassTimeStep(circuit2, reduce);
    copyEach(circuit2.Vout,circuit1.Vin, reduce);
    copyEach(circuit1.Vin,circuit1.Vin_prev, reduce);

    for(long long i = 0; i != max_frames; i++){
        copyEach(frame,circuit2.Vin_prev, 1);
        cap >> frame;
        frame_count++;
        
        std::cout << frame_count << std::endl;

        copyEach(frame,circuit2.Vin, 1);
        highPassTimeStep(circuit2, reduce);
        copyEach(circuit1.Vin,circuit1.Vin_prev, reduce);
        copyEach(circuit2.Vout,circuit1.Vin, reduce);
        lowPassTimeStep(circuit2,reduce);

        imshow("video", circuit2.qVout);
        int key = waitKey(30);
        if (key >= 0 && key <= 255) {
            printf("%d\n",key);
            break;
        }
    }
 }
