//
// Created by Zhiyuan Wang on 18/06/2017.
//

#include "quadTree.h"

quadTree::quadTree(const Mat &img, Rect roi, double threshold, int min_granular_size) {
    this->mVector = colorHistVector(img, roi);
    this->similarity_threshold = threshold;
    int x = roi.x, y = roi.y;
    int w = roi.width, h = roi.height;
    if(w/2 >= min_granular_size && h/2 >= min_granular_size){
        this->upleft = new quadTree(img, Rect(x, y, w/2, h/2), threshold, min_granular_size);
        this->upright = new quadTree(img, Rect(x+w/2, 0, w-w/2, h/2), threshold, min_granular_size);
        this->downleft = new quadTree(img, Rect(x, y+h/2, w/2, h-h/2), threshold, min_granular_size);
        this->downright = new quadTree(img, Rect(x+w/2, y+h/2, w-w/2, h-h/2), min_granular_size);
    }
}

colorHistVector* quadTree::getVector() {
    return &(this->mVector);
}