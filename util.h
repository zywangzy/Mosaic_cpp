/**
 * Created by Zhiyuan Wang on 16/06/2017.
 * It's a class to provide some useful helping functions.
 */

#ifndef MOSAIC_UTIL_H
#define MOSAIC_UTIL_H

#include <iostream>
#include <string>
#include <fstream>
#include <opencv/cv.h>
#include <Eigen3/Eigen/Dense>
#include <Eigen3/Eigen/Sparse>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "colorHistVector.h"
#include "imgSegmentation.h"
#include "mosaicGenerator.h"
#include "pca.h"

using namespace std;
using namespace cv;
using namespace Eigen;

class util {
public:
    /**
     * Check the version of OpenCV.
     */
    static void check_cv_version();

    static VectorXd unfold_colorhist(colorHistVector& hist);

    static double vector_distance(VectorXd& vec1, VectorXd& vec2);

    static void save_vectorxd_to_json(string path, VectorXd& vector);

    static VectorXd read_vectorxd_from_json(string path);
    /**
     * Do batch processing of compressing library images.
     */
    static void batchCompressImages();
    /**
     * Compress the source image by calculating mean color in squares with granular size.
     * @param src source image.
     * @param dst destination image.
     * @param granular_size the size of squares in source image used to calculate mean color.
     */
    static void meanSquares(const Mat& src, Mat& dst, const int granular_size = 10);
    /**
     * Tester method of meanSquare.
     */
    static void meanSquaresTester();
    /**
     * Tester method of class colorHistVector.
     */
    static void colorHistVectorTester();
    /**
     * Do batch processing of computing color histogram of images.
     */
    static void batch_compute_colorHistVector();

    static void imgSegmentationTester();

    static void mosaicGeneratorTester();

    static void pcaTester();

    static void pcaMosaicGeneratorTester();
};


#endif //MOSAIC_UTIL_H
