#include "colorHistVector.h"
#include "util.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    util::check_cv_version();
    // Step 1: compress library images to 100*100.
    // util::batchCompressImages();
    // Step 2: try to get the color histogram of each library image.
    //util::colorHistVectorTester();
    // Step 3: batch processing color histogram vector for library images.
    //util::batch_compute_colorHistVector();
    // Step 4: For each input image, do segmentation and merge for small chunks.
    //util::imgSegmentationTester();
    // Step 5: For each chunk in the input image, find the closest matching image in library.
    //util::mosaicGeneratorTester();
    //util::pcaTester();
    util::pcaMosaicGeneratorTester();
    //util::hsvHistTester();
    return 0;
}