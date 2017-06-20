/**
 * Created by Zhiyuan Wang on 15/06/2017.
 * It's a class used to describe the color histogram of an image by a vector.
**/

#ifndef MOSAIC_COLORHISTVECTOR_H
#define MOSAIC_COLORHISTVECTOR_H

#include <vector>
#include <cassert>
#include <string>
#include <iostream>
#include <opencv/cv.h>

using namespace std;
using namespace cv;

class colorHistVector {
private:
    int colorSpaceResolution;
    int colorLevelCount;
    vector<vector<vector<double>>> mVector;
public:
    colorHistVector(){};
    /**
     * Constructor of colorHistVector. Will store the vector values in
     * the 3-dimensional array. Each entry of vector is the frequency (0-1)
     * of pixels in that color range.
     * @param img Assume the input Mat img is in rgb256 format.
     * @param colorRes color space resolution, to reduce the size of 3d vector.
     */
    colorHistVector(const Mat& img, int colorRes = 10);
    /**
     * Constructor of colorHistVector based on images with the ROI.
     * @param img source image.
     * @param ROI region of interest. Should be inside the source image.
     * @param colorRes color space resolution, to reduce the size of 3d vector.
     */
    colorHistVector(const Mat& img, Rect roi, int colorRes = 10);
    /**
     * Constructs a colorHistVector from a json/xml file
     * storing the parameters.
     * @param filename
     */
    colorHistVector(string filename);
    /**
     * Calculate the distance in rgb space between 2 color histogram vectors.
     * @param vector1 an object of class colorHistVector
     * @param vector2 another object of class colorHistVector
     * @return
     */
    static double colorDistance(const colorHistVector& vector1, const colorHistVector& vector2);
    /**
     * Calculate the color distribution similarity between 2 color histogram vectors.
     * @param vector1 an object of class colorHistVector
     * @param vector2 another object of class colorHistVector
     * @return
     */
    static double colorSimilarity(const colorHistVector &vector1, const colorHistVector &vector2);
    /**
     * Export the vector to json file on harddisk. (Make database further)
     * @param path
     */
    void exportToFile(string path);
    /**
     * Print basic info of the vector. (It's more like for debugging use)
     */
    void print();
    /**
     * Helper private method. Check if the roi is in the bound of image.
     */
    static bool checkInBound(const Mat &img, const Rect &roi);

private:
    /**
     * Helper private method. Initializes the 3-d vector.
     */
    void init_3dvector();
};


#endif //MOSAIC_COLORHISTVECTOR_H
