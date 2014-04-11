#include "CsvCapture.h"

CsvCapture::CsvCapture(const cv::String &filename) {
    // TODO: throw exception if this fails
    open(filename);
}

bool CsvCapture::open(const cv::String &filename) {
    // TODO: Do better error handling
    inputFile.open(filename.c_str());
    return inputFile.is_open();
}

bool CsvCapture::isOpened() const {
    return inputFile.is_open();
}

void CsvCapture::release() {
    inputFile.close();
}

bool CsvCapture::grab() {
    //TODO: Check spec
    return false;
}

bool CsvCapture::retrieve(cv::OutputArray image, int flag) {
    //TODO: Check spec
    return false;
}

cv::VideoCapture& CsvCapture::operator>> (cv::Mat& image) {
    //TODO: Setup Image
    return *this;
}

cv::VideoCapture& CsvCapture::operator>> (cv::UMat& image) {
    //TODO: Setup Image
    return *this;
}

bool CsvCapture::read(cv::OutputArray image) {
    return false;
}

bool CsvCapture::set(int propId, double value) {
    return false;
}

double CsvCapture::get(int propid) {
    return 0;
}

CsvCapture::~CsvCapture() {
    inputFile.close();
}
