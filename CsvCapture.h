#ifndef CsvCapture_H
#define CsvCapture_H
#include <opencv2/highgui.hpp>

#include <iostream>
#include <fstream>

class CsvCapture : public cv::VideoCapture {
public:
    CsvCapture() {};
    CsvCapture(const cv::String& filename);

    virtual bool open(const cv::String& filename);
    virtual bool isOpened() const;
    virtual void release();
    
    virtual bool grab();
    virtual bool retrieve(cv::OutputArray image, int flag = 0);
    virtual cv::VideoCapture& operator >> (cv::Mat& image);
    virtual cv::VideoCapture& operator >> (cv::UMat& image);
    virtual bool read(cv::OutputArray image);

    virtual bool set(int propId, double value);
    virtual double get(int propId);
    
    virtual ~CsvCapture();
private:
    std::ifstream inputFile;
};
#endif
