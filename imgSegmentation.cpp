//
// Created by Zhiyuan Wang on 19/06/2017.
//

#include "imgSegmentation.h"

mRect::mRect(int in_x, int in_y, int in_width, int in_height){
    this->x = in_x;
    this->y = in_y;
    this->width = in_width;
    this->height = in_height;
}

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

block::block(Mat const& img, mRect const& input_roi, const int colorRes):roi(input_roi) {
    assert(colorHistVector::checkInBound(img, input_roi));
    assert(roi.height == roi.width);
    this->size = roi.height;
    //this->roi = input_roi;
    this->colorhist = colorHistVector(img, roi, colorRes);
}

bool block::operator==(const block &b) {
    return (this->size == b.size) && (this->roi.x == b.roi.x) &&
           (this->roi.y == b.roi.y) && (this->roi.width == b.roi.width);
}

imgSegmentation::imgSegmentation(Mat &img, int color_resolution, double sim_threshold, int min_size, int max_size):
    color_resolution(color_resolution), similarity_threshold(sim_threshold), min_size(min_size), max_size(max_size)
{
    this->map = unordered_map<mRect, block*>();
    imgCropper(img, min_size);
}

void imgSegmentation::imgCropper(Mat &img, int min_size) {
    int width = img.cols, height = img.rows;
    int width_remainder = width % min_size, height_remainder = height % min_size;
    int final_width = width - width_remainder, final_height = height - height_remainder;
    this->mImg = img(Rect(width_remainder / 2, height_remainder / 2, final_width, final_height));
}

void imgSegmentation::segment() {
    initialize_segments();
    print();
    merge_segments();
}

void imgSegmentation::initialize_segments() {
    int x_num = this->mImg.cols / this->min_size,
        y_num = this->mImg.rows / this->min_size;
    for(int i = 0; i < x_num; i++){
        for(int j = 0; j < y_num; j++){
            const mRect rect(i * min_size, j * min_size, min_size, min_size);
            block* mblock = new block(this->mImg, rect, this->color_resolution);
            this->map[rect] = mblock;
        }
    }
}

void imgSegmentation::merge_segments(){

}

void imgSegmentation::print() {
    cout << "********imgSegmentation Info*********" << endl;
    cout << "similarity_threshold = " << this->similarity_threshold << endl;
    cout << "min_size = " << this->min_size << ", max_size = " << this->max_size << endl;
    cout << "color_resolution = " << this->color_resolution << endl;
    cout << "size(map<mRect, block*>) = " << this->map.size() << endl;
    cout << "*************************************" << endl;
}