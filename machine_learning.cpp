#include <cv.h>
#include <highgui.h>
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
    Mat_<Vec3d> dVout;
    Mat_<Vec3b> qVout;

    double iR; // Inverse Resistance
    double iC; // Inverse Capacitance


    double dt; 
};

void timeStep(Circuit &c, int reduction){
    double dt = c.dt;
    double iR = c.iR;
    double iC = c.iC;
    int offset = reduction;
    for(int ip = 0; ip < c.Vin.rows; ip += offset){
        for(int jp = 0; jp < c.Vin.cols; jp += offset){
            int i = ip/offset;
            int j = jp/offset;
            for(int index = 0; index < 3;index++){
                double _Vout = c.Vout(i,j)[index];
                double _dVin = c.Vin(ip,jp)[index]-c.Vin_prev(ip,jp)[index];
                /* Low Pass */
                 
                double _dVout = c.dVout(i,j)[index];
                double _ddVout = ((c.Vin(ip,jp)[index]
                                    -c.Vin_prev(ip,jp)[index])
                                    -_dVout)*iR*iC/(dt*dt);
                /* High Pass */
//                double _dVout = _dVin-(c.iR*c.iC)*_dVout;
    
                c.Vout(i,j)[index] = _Vout+_dVout*dt;
                c.dVout(i,j)[index] = _dVout + _ddVout*dt;
                _Vout = c.Vin(ip,jp)[index]-c.qVout(i,j)[index];
                if(_Vout > 255)
                {
                    c.qVout(i,j)[index] = 255;
                }
                else if(_Vout < 0){
                    c.qVout(i,j)[index] = 0;
                }
                else
                    c.qVout(i,j)[index] = _Vout;
            }
        }
    }
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
    Circuit circuit;
    circuit.iR = 0.05;
    circuit.iC = 1;
    circuit.dt = 1;
        //Get first frame
    int frame_count = 0;
    cap >> frame; if(! frame.data) return -1;

    //Initialize voltages to correct sizes

    circuit.Vout = Mat::zeros(frame.rows/reduce,frame.cols/reduce,CV_32F); 
    circuit.dVout = Mat::zeros(frame.rows/reduce,frame.cols/reduce,CV_32F);
    circuit.qVout = Mat::zeros(frame.rows/reduce,frame.cols/reduce,CV_32F);
    circuit.Vin = Mat::zeros(frame.rows,frame.cols,CV_32F);
    circuit.Vin_prev = Mat::zeros(frame.rows,frame.cols,CV_32F);

    skipAhead(skip, cap);

    copyEach(frame,circuit.qVout,reduce);

    //frame.copyTo(circuit.qVout);
    //frame.copyTo(circuit.Vout);
    frame.copyTo(circuit.Vin);
    timeStep(circuit, reduce);

    int peekx= 100;
    int peeky= 100;

    std::cout << (int)circuit.qVout(peekx,peeky)[1]
              << '/' <<circuit.Vout(peekx,peeky)[1] 
              << std::endl;
    for(;;){
        frame.copyTo(circuit.Vin_prev);
        std::cout <<frame_count<<" ";
        frame_count++;
        cap >> frame;
        
        frame.copyTo(circuit.Vin);
        std::cout << (int)frame(peekx,peeky)[1] << " ";
        timeStep(circuit,reduce);
        std::cout << (int)circuit.qVout(peekx,peeky)[1]
                  << '/' <<circuit.Vout(peekx,peeky)[1] 
                  << std::endl;
        if(frame_count %2)
        imshow("video", circuit.qVout); if(waitKey(30) >= 0) break;
    }
}
