//
// Created by Zhiyuan Wang on 26/06/2017.
//

#ifndef MOSAIC_CPP_PCA_H
#define MOSAIC_CPP_PCA_H

//include Eigen library here, temporarily use Mat for Matrix
//#include "Eigen.h"
#include "mosaicGenerator.h"

class pca {
private:
    vector<Mat> hist_list;
public:
    pca(mosaicGenerator& generator);
    pca(string basic_path);
    void get_eigen_colorhist();
    Mat unfold_colorhist(colorHistVector& hist);
};


#endif //MOSAIC_CPP_PCA_H
