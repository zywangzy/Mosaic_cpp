//
// Created by Zhiyuan Wang on 16/06/2017.
//

#include "util.h"

using namespace std;
using namespace cv;

const int compress_size = 100;

void util::check_cv_version() {
    cout << "********** OpenCV Version **********" << endl;
    cout << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;
    cout << "************************************" << endl;
}

void util::batchCompressImages(void){
    string path = "../aflw 2/data/flickr/";
    string src_path = path + "0/";
    string dst_path = path + "compressed/";

    ifstream list_file("../list.txt");

    string line;
    while(getline(list_file, line)){
        cout << line << endl;
        Mat src = imread(src_path + line);
        int dst_size = min(src.cols, src.rows);
        Mat dst = src(Range((src.rows - dst_size)/2, (src.rows + dst_size)/2),
                      Range((src.cols - dst_size)/2, (src.cols + dst_size)/2));
        resize(dst, dst, Size(compress_size, compress_size));
        imwrite(dst_path + line, dst);
    }
}

void util::meanSquares(const Mat& src, Mat& dst, const int granular_size){
    dst = Mat(src.rows, src.cols, src.type());
    for(int y = 0; y < ceil(src.rows * 1.0 / granular_size); y++){
        for(int x = 0; x < ceil(src.cols * 1.0 / granular_size); x++){
            int sum_r = 0, sum_g = 0, sum_b = 0;
            int bias_y = y * granular_size, bias_x = x * granular_size;
            int range_y = min(granular_size, src.rows - bias_y);
            int range_x = min(granular_size, src.cols - bias_x);
            for(int j = 0; j < range_y; j++){
                for(int i = 0; i < range_x; i++){
                    sum_r += src.at<Vec3b>(bias_y + j, bias_x + i)[2];
                    sum_g += src.at<Vec3b>(bias_y + j, bias_x + i)[1];
                    sum_b += src.at<Vec3b>(bias_y + j, bias_x + i)[0];
                }
            }
            sum_r /= (granular_size * granular_size);
            sum_g /= (granular_size * granular_size);
            sum_b /= (granular_size * granular_size);
            for(int j = 0; j < range_y; j++){
                for(int i = 0; i < range_x; i++){
                    dst.at<Vec3b>(bias_y + j, bias_x + i)[2] = sum_r;
                    dst.at<Vec3b>(bias_y + j, bias_x + i)[1] = sum_g;
                    dst.at<Vec3b>(bias_y + j, bias_x + i)[0] = sum_b;
                }
            }
        }
    }
}

void util::meanSquaresTester(void){
    Mat src = imread("../lena.bmp");
    Mat dst;
    imshow("src", src);
    waitKey();
    meanSquares(src, dst, 3);
    imshow("dst", dst);
    waitKey();
}

void util::colorHistVectorTester() {
    Mat src1 = imread("../lena.bmp");
    Mat src2 = imread("../defend_the_land.png");
    colorHistVector vector1(src1, Rect(0,0,256,512), 20), vector2(src1, Rect(256, 0, 256, 512), 20);
    double dist = colorHistVector::colorDistance(vector1, vector2);
    cout << "The distance between vector1 and 2 are " << dist << endl;
    Scalar diff = mean(src1) - mean(src2);
    double square_sum = 0;
    for(int i = 0; i < 3; i++) square_sum += pow(diff[i], 2);
    cout << "The distance between average color of two images are " << sqrt(square_sum) << endl;
    cout << "The similarity between vector 1 and 2 are "
         << colorHistVector::colorSimilarity(vector1, vector2) << endl;
    vector1.print();
    vector1.exportToFile("../vector_lena.json");
    colorHistVector vector3("../vector_lena.json");
    vector3.print();
    cout << "The similarity between vector 1 and 3 are "
         << colorHistVector::colorSimilarity(vector1, vector3) << endl;
}

void util::batch_compute_colorHistVector() {
    string path = "../aflw 2/data/flickr/";
    string src_path = path + "compressed/";
    string dst_path = path + "colorHist/";
    ifstream list_file("../list.txt");
    time_t raw_start_time; time(&raw_start_time);
    string start_time = asctime(localtime(&raw_start_time));

    int count = 0;
    string line;
    while(getline(list_file, line)){
        count++;
        string name = line.substr(0, line.length()-4);
        cout << name << endl;
        Mat src = imread(src_path + line);
        colorHistVector hist(src, 20);
        hist.exportToFile(dst_path + name + ".json");
    }

    time_t raw_end_time; time(&raw_end_time);
    string end_time = asctime(localtime(&raw_end_time));
    cout << "From: " << start_time;
    cout << "To: " << end_time;
    cout << "Batch processed " << count << " images for computation of color histogram." << endl;
    /**
     * From: Sun Jun 18 01:28:46 2017
       To: Sun Jun 18 01:28:59 2017
       Batch processed 7202 images for computation of color histogram.
     */
}

void util::imgSegmentationTester() {
    Mat src = imread("../lena.bmp");
    imgSegmentation segment(src);
    segment.segment();
    segment.showMergeResult();
}