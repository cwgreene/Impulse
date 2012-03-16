#include "cv.h"
#include "highgui.h"
#include <iostream>

using namespace cv;

int print_vec(Vec3b v){
    unsigned int a=v[0],b=v[0],c=v[0];
    std::cout << a << " " << b << " " << c << std::endl;
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
    cap >> frame; if(! frame.data) return 0;
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
                            //Edit colors here.
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
