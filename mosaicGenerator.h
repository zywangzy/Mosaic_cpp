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
    /**
     * Constructor of mosaicGenerator.
     * @param segment_obj the object of image segmentation result.
     * @param basic_path the path of image library.
     */
    mosaicGenerator(imgSegmentation& segment_obj, string basic_path);
    /**
     * Generate the final mosaic result. Iterate all chunks in segmentation result,
     * get the optimal replacement image from library and return the result image.
     * @return
     */
    Mat generate();
    unordered_map<string, colorHistVector>* get_img_lib() { return &img_lib; };
private:
    /**
     * Helper method to get all the colorHistVector objects from library.
     */
    void library_reader();
    /**
     * Helper method to find the best matching hist in library for input hist.
     * @param histVector given color histogram vector.
     * @return the key of best matching hist in map.
     */
    string find_target_in_lib(colorHistVector& histVector);
};


#endif //MOSAIC_CPP_MOSAICGENERATOR_H
