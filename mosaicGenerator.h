//
// Created by Zhiyuan Wang on 24/06/2017.
//

#ifndef MOSAIC_CPP_MOSAICGENERATOR_H
#define MOSAIC_CPP_MOSAICGENERATOR_H

#include <fstream>
#include <Eigen3/Eigen/Dense>
#include <Eigen3/Eigen/Sparse>
#include "imgSegmentation.h"
#include "pca.h"

using namespace Eigen;

class mosaicGenerator {
private:
    string basic_path;
    unordered_map<mRect, block*> img_segments;
    unordered_map<string, colorHistVector> img_lib;
    Mat src_img;
public:
    /**
     *
     * @param segment_obj
     * @param basic_path
     */
    mosaicGenerator(imgSegmentation& segment_obj, string basic_path);
    mosaicGenerator(){};
    /**
     *
     * @return
     */
    Mat generate();
    /**
     *
     * @return
     */
    unordered_map<string, colorHistVector>* get_img_lib() { return &img_lib; };
private:
    void library_reader();
    string find_target_in_lib(colorHistVector& histVector);
};

class pcaMosaicGenerator{//: public mosaicGenerator {
private:
    int pca_dimension;
    string basic_path;
    unordered_map<mRect, block*> img_segments;
    unordered_map<string, VectorXd> img_lib;
    Mat src_img;
    MatrixXd convert_matrix;
public:
    pcaMosaicGenerator(imgSegmentation& segment_obj, string basic_path, int r = 50);
    Mat generate();
    void library_reader();
    MatrixXd matrix_reader_from_csv(string path);
};


#endif //MOSAIC_CPP_MOSAICGENERATOR_H
