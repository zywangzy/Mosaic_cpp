//
// Created by Zhiyuan Wang on 19/06/2017.
//

#include "imgSegmentation.h"

mRect::mRect(Rect rect) {
    this->x = rect.x;
    this->y = rect.y;
    this->width = rect.width;
    this->height = rect.height;
}

bool mRect::operator==(const mRect &rect) {
    return (this->x == rect.x) && (this->width == rect.width) &&
           (this->y == rect.y) && (this->height == rect.height);
}

/*
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
 */

block::block(Mat &img, Rect &input_roi, int colorRes): roi(input_roi) {
    assert(colorHistVector::checkInBound(img, input_roi));
    assert(roi.height == roi.width);
    this->size = roi.height;
    this->colorhist = colorHistVector(img, roi, colorRes);
}

bool block::operator==(const block &b) {
    return (this->size == b.size) && (this->roi.x == b.roi.x) &&
           (this->roi.y == b.roi.y) && (this->roi.width == b.roi.width);
}

imgSegmentation::imgSegmentation(Mat &img, double sim_threshold, int min_size, int max_size) {
    this->similarity_threshold = sim_threshold;
    this->min_size = min_size;
    this->max_size = max_size;
    this->rois = unordered_map<mRect, block>();
    imgCropper(img, min_size);
    imshow("original", img);
    cvWaitKey();
    imshow("cropped image", this->mImg);
    cvWaitKey();
}

void imgSegmentation::imgCropper(Mat &img, int min_size) {
    int width = img.cols, height = img.rows;
    int width_remainder = width % min_size, height_remainder = height % min_size;
    int final_width = width - width_remainder, final_height = height - height_remainder;
    this->mImg = img(Rect(width_remainder / 2, height_remainder / 2, final_width, final_height));
}