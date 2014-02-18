#ifndef OPTIONS_H
#define OPTIONS_H
#include <string>
#include <opencv2/opencv.hpp>

void handle_options(int argc, char **argv,
                    int &frame_offset,
                    int &reduce,
                    long long &max_frames,
                    std::string &input_file,
                    cv::VideoCapture &cap,
                    cv::VideoWriter &writer);
#endif
