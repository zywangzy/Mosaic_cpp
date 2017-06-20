//
// Created by Zhiyuan Wang on 18/06/2017.
//

#ifndef MOSAIC_QUADTREE_H
#define MOSAIC_QUADTREE_H

#include "colorHistVector.h"

class quadTree {
private:
    colorHistVector mVector = colorHistVector();
    double similarity_threshold;
    quadTree* upleft = NULL;
    quadTree* upright = NULL;
    quadTree* downleft = NULL;
    quadTree* downright = NULL;
public:
    quadTree(const Mat &img, Rect roi, double threshold = 0.5, int min_granular_size = 10);
    colorHistVector* getVector();
};


#endif //MOSAIC_QUADTREE_H
