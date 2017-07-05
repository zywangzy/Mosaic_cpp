//
// Created by Zhiyuan Wang on 19/06/2017.
//

#ifndef MOSAIC_IMGSEGMENTATION_H
#define MOSAIC_IMGSEGMENTATION_H

#include <unordered_set>
#include <unordered_map>
#include <opencv/cv.hpp>
#include "colorHistVector.h"

using namespace std;
using namespace cv;

/**
 * Customized class inherited from Rect class. Added hash and equal functions.
 */
class mRect: public Rect{
public:
    mRect(){};
    mRect(int x, int y, int width, int height);
    mRect(Rect rect);
    bool operator==(const mRect& rect);
};

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

/**
 * The class representing the color histogram of a chunk.
 */
class block {
public:
    block(Mat const& img, mRect const& roi, int colorRes);
    //bool operator==(const block& b);
public:
    int size;
    //mRect roi;
    colorHistVector colorhist;
};

/**
 * The major class for image segmentation.
 */
class imgSegmentation {
private:
    Mat mImg;
    unordered_map<mRect, block*> map;
    double similarity_threshold;
    int min_size;
    int max_size;
    int color_resolution;
public:
    /**
     * Constructor. This segmentation class uses bottom-top method (merge).
     * @param img source image.
     * @param color_resolution color resolution of 3d-vector for color histogram.
     * @param similarity_threshold the threshold of color similarity for different chunks to get merged.
     * @param min_size the starting size of each chunk during merging.
     * @param max_size maximum size limit of chunks in segmentation result.
     */
    imgSegmentation(Mat &img, int color_resolution = 10, double similarity_threshold = 0.5,
                    int min_size = 20, int max_size = 160);
    /**
     * The method doing actual segmentation.
     */
    void segment();
    /**
     * Print some info to help debug.
     */
    void print();
    /**
     * Save merge result to disk.
     * @param path the destination path.
     */
    void saveMergeResult(string path);
    /**
     * Helper method for mosaicGenerator to get map.
     * @return the map in this class.
     */
    unordered_map<mRect, block*> getMap(){ return this->map; };
    /**
     * Helper method for mosaicGenerator to get source image Mat object.
     */
    Mat get_img(){ return mImg; };
private:
    /**
     * Helper method to crop source image to fit for merge. The remainder of image size
     * divided by minimal chunk size (starting size) would be cropped.
     * @param img source image.
     * @param min_size starting size of chunks.
     */
    void imgCropper(Mat &img, int min_size);
    /**
     * Calculate all colorHistVector and save for later merge.
     */
    void initialize_segments();
    /**
     * If there're 4 neighbor segments with same size and color histogram similarities
     * between each pair are above threshold, then merge them. Do this step until there
     * are no segments satisfying the condition.
     * */
    void merge_segments();
    /**
     * For a given rectangle area, generate all 4 possible combinations of rectangles.
     */
    vector<vector<mRect>> get_candidate_rois(const mRect& rect);
    /**
     * Decide if given 4 roi is valid for merge. All 4 rois must be in map (the chunks exist)
     * and not merged yet, and the similarities between each pair of chunks must be above threshold.
     */
    bool valid_for_merge(const vector<mRect>& rois, const unordered_set<mRect>& merged);
    /**
     * Get result rectangle of the chunk merged by 4 smaller chunks.
     */
    mRect merge_rois(const vector<mRect> rois);
};


#endif //MOSAIC_IMGSEGMENTATION_H
