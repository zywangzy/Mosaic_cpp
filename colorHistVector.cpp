/**
 * Created by Zhiyuan Wang on 15/06/2017.
 * It's a class used to describe the color histogram of an image by a vector.
**/

#include "colorHistVector.h"

int h_bins = 32;
int s_bins = 30;
int v_bins = 16;
int histSize[] = {h_bins, s_bins, v_bins};

float h_ranges[] = { 0, 256 };
float s_ranges[] = { 0, 180 };
float v_ranges[] = { 0, 256 };

const float* ranges[] = { h_ranges, s_ranges, v_ranges };
int channels[] = { 0, 1, 2};

colorHistVector::colorHistVector(const Mat &img, int colorRes) {
    this->colorSpaceResolution = colorRes;
    this->colorLevelCount = (int)ceil(256.0 / colorRes);
    int pixel_count = img.cols * img.rows;
    double increment_unit = 1.0 / pixel_count;
    init_3dvector();
    for(int j = 0; j < img.rows; j++){
        for(int i = 0; i < img.cols; i++){
            int r = img.at<Vec3b>(j, i)[2] / colorRes;
            int g = img.at<Vec3b>(j, i)[1] / colorRes;
            int b = img.at<Vec3b>(j, i)[0] / colorRes;
            mVector[r][g][b] += increment_unit;
        }
    }
}

colorHistVector::colorHistVector(const Mat &img, Rect roi, int colorRes) {
    this->colorSpaceResolution = colorRes;
    this->colorLevelCount = (int)ceil(256.0 / colorRes);
    assert(checkInBound(img, roi));
    int pixel_count = roi.height * roi.width;
    double increment_unit = 1.0 / pixel_count;
    init_3dvector();
    for(int j = roi.y; j < roi.y + roi.height; j++){
        for(int i = roi.x; i < roi.x + roi.width; i++){
            int r = img.at<Vec3b>(j, i)[2] / colorRes;
            int g = img.at<Vec3b>(j, i)[1] / colorRes;
            int b = img.at<Vec3b>(j, i)[0] / colorRes;
            mVector[r][g][b] += increment_unit;
        }
    }
}

colorHistVector::colorHistVector(string filename) {
    FileStorage fs(filename, FileStorage::READ);
    int filename_len = filename.length();
    assert(filename.substr(filename_len - 5).compare(".json") == 0);
    fs["colorSpaceResolution"] >> this->colorSpaceResolution;
    fs["colorLevelCount"] >> this->colorLevelCount;
    //Read the vector entries
    vector<vector<vector<double>>> source_vector;//maybe vector<string>
    fs["vector"] >> source_vector;
    init_3dvector();
    for(int r = 0; r < colorLevelCount; r++){
        for(int g = 0; g < colorLevelCount; g++){
            for(int b = 0; b < colorLevelCount; b++){
                mVector[r][g][b] = source_vector[r][g][b];
            }
        }
    }
}

double colorHistVector::colorSimilarity(const colorHistVector &vector1,
                                        const colorHistVector &vector2) {
    assert(vector1.colorLevelCount == vector2.colorLevelCount);
    assert(vector1.mVector.size() && vector2.mVector.size());
    int colorLevelCount = vector1.colorLevelCount;
    double distance_sum = 0;
    vector<vector<vector<double>>> array1 = vector1.mVector,
                                   array2 = vector2.mVector;
    for(int r = 0; r < colorLevelCount; r++){
        for(int g = 0; g < colorLevelCount; g++){
            for(int b = 0; b < colorLevelCount; b++){
                /*
                 * Here we use the 'density distance' defined in this paper:
                 * https://engineering.ucsb.edu/~cse/Files/distributiondistance042.pdf
                 * within which formula (25) and (26).
                 */
                distance_sum += abs(array1[r][g][b] - array2[r][g][b]);
            }
        }
    }
    return 1 - sqrt(distance_sum) / 2;
}

double colorHistVector::colorDistance(const colorHistVector &vector1,
                                      const colorHistVector &vector2) {
    assert(vector1.colorSpaceResolution == vector2.colorSpaceResolution);
    int colorLevelCount = round(256.0 / vector1.colorSpaceResolution);
    int colorRes = vector1.colorSpaceResolution;
    vector<vector<vector<double>>> array1 = vector1.mVector,
            array2 = vector2.mVector;
    double weighted_vec1[] = {0, 0, 0}, weighted_vec2[] = {0, 0, 0};
    for(int r = 0; r < colorLevelCount; r++){
        for(int g = 0; g < colorLevelCount; g++){
            for(int b = 0; b < colorLevelCount; b++){
                weighted_vec1[0] += array1[r][g][b] * r * colorRes;//r
                weighted_vec1[1] += array1[r][g][b] * g * colorRes;//g
                weighted_vec1[2] += array1[r][g][b] * b * colorRes;//b
                weighted_vec2[0] += array2[r][g][b] * r * colorRes;//r
                weighted_vec2[1] += array2[r][g][b] * g * colorRes;//g
                weighted_vec2[2] += array2[r][g][b] * b * colorRes;//b
            }
        }
    }
    return sqrt(pow(weighted_vec1[0] - weighted_vec2[0],2) +
                pow(weighted_vec1[1] - weighted_vec2[1],2) +
                pow(weighted_vec1[2] - weighted_vec2[2],2));
}

int colorHistVector::get_color_level_count() {
    return this->colorLevelCount;
}

vector<vector<vector<double>>>* colorHistVector::get_ptr_to_hist() {
    return &(this->mVector);
}

void colorHistVector::exportToFile(const string path) {
    cout << "Saving colorHistVector to " << path << endl;
    FileStorage fs(path, FileStorage::WRITE);
    fs << "colorSpaceResolution" << colorSpaceResolution;
    fs << "colorLevelCount" << colorLevelCount;
    fs << "vector" << "[";
    for(int r = 0; r < colorLevelCount; r++){
        fs << "[";
        for(int g = 0; g < colorLevelCount; g++){
            fs << "[";
            for(int b = 0; b < colorLevelCount; b++){
                fs << this->mVector[r][g][b];
            }
            fs << "]";
        }
        fs << "]";
    }
    fs << "]";
    fs.release();
}

void colorHistVector::print() {
    cout << "colorSpaceResolution = " << this->colorSpaceResolution
         << ", colorLevelCount = " << this->colorLevelCount << endl;
}

void colorHistVector::init_3dvector() {
    this->mVector = vector<vector<vector<double>>>();
    for(int i = 0; i < colorLevelCount; i++){
        vector<vector<double>> row_g;
        for(int j = 0; j < colorLevelCount; j++){
            vector<double> row_b(colorLevelCount);
            row_g.push_back(row_b);
        }
        mVector.push_back(row_g);
    }
}

bool colorHistVector::checkInBound(const Mat &img, const Rect &roi) {
    return (roi.x >= 0) && (roi.x < img.cols)
        && (roi.y >= 0) && (roi.y < img.rows)
        && (roi.width >= 0) && (roi.x + roi.width <= img.cols)
        && (roi.height >= 0) && (roi.y + roi.height <= img.rows);
}

hsvHistVector::hsvHistVector(Mat &img) {
    int pixel_count = img.cols * img.rows;
    double increment_unit = 1.0 / pixel_count;
    //convert from RGB to HSV color space
    cvtColor(img, img, COLOR_BGR2HSV);
    //calculate histogram
    calcHist(&img, 1, channels, Mat(), mVector, 3, histSize, ranges, true, false);
}

void hsvHistVector::print_info() {
    cout << "dims = " << mVector.dims << endl;
    double sum = 0;
    for(int x = 0; x < h_bins; x++){
        for(int y = 0; y < s_bins; y++){
            for(int z = 0; z < v_bins; z++){
                sum += mVector.at<double>(x, y, z);
                cout << mVector.at<float>(x, y, z) << ", ";
            }
            cout << endl;
        }
    }
    cout << "sum = " << sum << endl;
}