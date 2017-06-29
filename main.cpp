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
    //Next step: batch processing color histogram vector for library images.
    //util::batch_compute_colorHistVector();
    //util::imgSegmentationTester();
    //util::mosaicGeneratorTester();
    util::pcaTester();
    return 0;
}