//
// Created by Zhiyuan Wang on 24/06/2017.
//

#ifndef MOSAIC_CPP_MOSAICGENERATOR_H
#define MOSAIC_CPP_MOSAICGENERATOR_H

#include <fstream>
#include "imgSegmentation.h"

class mosaicGenerator {
private:
    string basic_path;
    unordered_map<mRect, block*> img_segments;
    unordered_map<string, colorHistVector> img_lib;
    Mat src_img;
public:
    mosaicGenerator(imgSegmentation& segment_obj, string basic_path);
    Mat generate();
    unordered_map<string, colorHistVector>* get_img_lib() { return &img_lib; };
private:
    void library_reader();
    string find_target_in_lib(colorHistVector& histVector);
};


#endif //MOSAIC_CPP_MOSAICGENERATOR_H
