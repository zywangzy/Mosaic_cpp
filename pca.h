//
// Created by Zhiyuan Wang on 26/06/2017.
//

#ifndef MOSAIC_CPP_PCA_H
#define MOSAIC_CPP_PCA_H

#include <Eigen3/Eigen/Dense>
#include <Eigen3/Eigen/Sparse>
#include <Eigen3/Eigen/SVD>
#include "mosaicGenerator.h"

using namespace std;
using namespace cv;
using namespace Eigen;

const static IOFormat CSV_Format(StreamPrecision, DontAlignCols, ", ", "\n");

class pca {
private:
    vector<VectorXd> hist_list;
    int color_level_count = 0;
    int vector_dimension = 0;
    int eigen_num;
public:
    /**
     * Constructor based on mosaicGenerator (needs the map of image lib from it).
     * @param generator the object of mosaicGenerator.
     * @param eigen_number the number of eigen vectors to use.
     */
    pca(mosaicGenerator& generator, int eigen_number = 20);
    /**
     * Constructor based on image library (containing color hist json files) path.
     */
    pca(string basic_path, int eigen_number = 20);
    /**
     * Compute the eigen vectors of color histograms.
     */
    void get_eigen_colorhist(bool read_cov_mat, bool use_svd);
    /**
     * Convert the 3d-matrix of color histogram to vector with size vector_dimension * 1.
     */
    VectorXd unfold_colorhist(colorHistVector& hist);
};


#endif //MOSAIC_CPP_PCA_H
