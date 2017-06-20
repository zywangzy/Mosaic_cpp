//
// Created by Zhiyuan Wang on 19/06/2017.
//

#ifndef MOSAIC_IMGSEGMENTATION_H
#define MOSAIC_IMGSEGMENTATION_H

#include <unordered_map>
#include <opencv/cv.hpp>
#include "colorHistVector.h"

using namespace std;
using namespace cv;

class mRect: public Rect{
public:
    mRect(Rect rect);
    bool operator==(const mRect& rect);
};

template <>
struct std::hash<mRect>{
    size_t operator()(const mRect& rect) const{
        int result = rect.x;
        result = 31*result + rect.width;
        result = 31*result + rect.y;
        result = 31*result + rect.height;
        return result;
    }
};

class block {
public:
    block(Mat &img, Rect &roi, int colorRes);
    bool operator==(const block& b);
public:
    int size;
    mRect roi;
    colorHistVector colorhist;
};

template <>
struct std::hash<block>{
    size_t operator()(const block& b) const {
        int result = b.size;
        result = 31*result + b.roi.x;
        result = 31*result + b.roi.width;
        result = 31*result + b.roi.y;
        return result;
    }
};

class imgSegmentation {
private:
    Mat mImg;
    unordered_map<mRect, block> rois;
    double similarity_threshold;
    int min_size;
    int max_size;
public:
    imgSegmentation(Mat &img, double similarity_threshold = 0.5, int min_size = 20, int max_size = 160);

private:
    void imgCropper(Mat &img, int min_size);
};


#endif //MOSAIC_IMGSEGMENTATION_H
