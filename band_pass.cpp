#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "filters.hpp"

using namespace cv;

namespace po = boost::program_options;

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

void print_help(po::options_description &description,
                po::positional_options_description &pd) {
    std::string usage;
    for(unsigned int i = 0; i < pd.max_total_count(); i++) {
        usage += " ";
        usage += pd.name_for_position(i);
    }
    std::cout << "Usage: band_pass" + usage << std::endl;
    std::cout << description;
    exit(-1);
}

int main(int argc, char **argv){
    int skip;
    int reduce;
    po::options_description description;
    description.add_options()
                            ("help", "Display this help message")
                            ("reduce", po::value<int>(&reduce)->default_value(1), "Reduction factor")
                            ("skip", po::value<int>(&skip)->default_value(0), "Frame skip")
                            ("webcam", "Use Webcam")
                            ("input-file", "Location of movie");

    po::positional_options_description pd;
    pd.add("input-file", 1);

    // Parse
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
                options(description).positional(pd).run(), vm);
    po::notify(vm);


    if(vm.count("help")) {
        print_help(description, pd);
    }

    if(!vm.count("input-file") && !vm.count("webcam")) {
        print_help(description, pd);
    }

    std::string input_file;
    if (!vm.count("webcam") ) {
        input_file = vm["input-file"].as<std::string>();
    }

    VideoCapture cap;
    if (vm.count("webcam")) {
        cap.open(0);
    } else {
        cap.open(input_file);
    }
    if (!cap.isOpened()) {
        std::cout << "Failed to open " << input_file << std::endl;
        if (!boost::filesystem::exists(input_file)) {
            std::cout << "File does not exist." << std::endl;;
        } else {
            std::cout << "Filetype not supported, make sure that OpenCV has"
                         " ffmpeg support." << std::endl;
        }
        return -1;
    }
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

    copyEach(frame,circuit2.qVout, reduce);

    //frame.copyTo(circuit.qVout);
    copyEach(frame,circuit2.Vin, 1);
    highPassTimeStep(circuit2, reduce);
    copyEach(circuit2.Vout,circuit1.Vin, reduce);
    copyEach(circuit1.Vin,circuit1.Vin_prev, reduce);

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
        highPassTimeStep(circuit2, reduce);
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
