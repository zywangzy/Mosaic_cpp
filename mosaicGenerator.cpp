//
// Created by Zhiyuan Wang on 24/06/2017.
//

#include "mosaicGenerator.h"
#include "util.h"

mosaicGenerator::mosaicGenerator(imgSegmentation &segment_obj, string basic_path) {
    this->basic_path = basic_path;
    this->src_img = segment_obj.get_img();
    this->img_segments = segment_obj.getMap();
    library_reader();
}

Mat mosaicGenerator::generate() {
    cout << "Generating mosaic..." << endl;
    Mat result(src_img);
    string img_path = basic_path + "compressed/";
    cout << "img_path = " << img_path << endl;
    cout << "Doing for loop..." << endl;
    for(unordered_map<mRect, block*>::iterator it = img_segments.begin();
            it != img_segments.end(); it++){
        string key = find_target_in_lib(it->second->colorhist);
        Mat src = imread(img_path + key + ".jpg");
        resize(src, src, Size(it->first.width, it->first.height));
        src.copyTo(result(Rect(it->first.x, it->first.y, it->first.width, it->first.height)));
    }
    return result;
}

void mosaicGenerator::library_reader() {
    this->img_lib = unordered_map<string, colorHistVector>();
    ifstream list_file("../../CVML/Mosaic/list.txt");
    string line, src_path = basic_path + "colorHist/";
    while(getline(list_file, line)){
        string name = line.substr(0, line.length()-4);
        cout << name << endl;
        this->img_lib[name] = colorHistVector(src_path + name + ".json");
    }
}

string mosaicGenerator::find_target_in_lib(colorHistVector &histVector) {
    string result;
    double best_sim = 0;
    for(unordered_map<string, colorHistVector>::iterator it = img_lib.begin();
            it != img_lib.end(); it++){
        double sim = colorHistVector::colorSimilarity(histVector, it->second);
        if(sim > best_sim){
            result = it->first;
            best_sim = sim;
        }
    }
    cout << "bestsim = " << best_sim << endl;
    return result;
}

pcaMosaicGenerator::pcaMosaicGenerator(imgSegmentation &segment_obj, string basic_path, int r) {
    this->pca_dimension = r;
    this->basic_path = basic_path;
    this->src_img = segment_obj.get_img();
    this->img_segments = segment_obj.getMap();
    MatrixXd matrix_V = matrix_reader_from_csv("../svd_matrixV.csv");//n*n
    this->convert_matrix = matrix_V.block(0, 0, matrix_V.rows(), r);//n*r
    MatrixXd matrix_mean = matrix_reader_from_csv("../pca_mean.csv");
    this->vector_mean = matrix_mean.col(0);
    MatrixXd matrix_stddev = matrix_reader_from_csv("../pca_stddev.csv");
    this->vector_stddev = matrix_stddev.col(0);
    library_reader();
}

Mat pcaMosaicGenerator::generate() {
    cout << "Generating mosaic with pca..." << endl;
    Mat result(src_img);
    string img_path = basic_path + "compressed/";
    cout << "img_path = " << img_path << endl;
    cout << "Doing for loop..." << endl;
    int count = 0;
    for(unordered_map<mRect, block*>::iterator it = img_segments.begin();
        it != img_segments.end(); it++){
        string key = find_best_match_in_lib(it->second->colorhist);
        cout << key << "  ";
        Mat src = imread(img_path + key + ".jpg");
        cout << src.rows << ", " << src.cols << ", " << it->first.width << ", " << it->first.height << endl;
        resize(src, src, Size(it->first.width, it->first.height));
        src.copyTo(result(Rect(it->first.x, it->first.y, it->first.width, it->first.height)));
        cout << ".";
        if(++count % 50 == 0) cout << endl;
    }
    cout << endl;
    return result;
}

string pcaMosaicGenerator::find_best_match_in_lib(colorHistVector &histVector) {
    string result;
    VectorXd original = util::unfold_colorhist(histVector);
    cout << "before dimension reduction" << endl;
    cout << original << endl;
    VectorXd target = vector_dimension_reduction(original);
    //(original.transpose() * convert_matrix).transpose();
    cout << "after dimension reduction" << endl;
    cout << original << endl;
    double best_sim = -0.1;
    for(unordered_map<string, VectorXd>::iterator it = img_lib.begin();
        it != img_lib.end(); it++){
        double sim = util::vector_distance(target, it->second);
        if(sim > best_sim){
            result = it->first;
            best_sim = sim;
        }
    }
    cout << "bestsim = " << best_sim << endl;
    cout << result << endl;
    return result;
}

void pcaMosaicGenerator::library_reader() {
    cout << "Reading library..." << endl;
    this->img_lib = unordered_map<string, VectorXd>();
    ifstream list_file("../../CVML/Mosaic/list.txt");
    string line, src_path = basic_path + "colorHist/";
    while(getline(list_file, line)){
        string name = line.substr(0, line.length()-4);
        //cout << name << endl;
        colorHistVector chv(src_path + name + ".json");
        VectorXd original = util::unfold_colorhist(chv);
        VectorXd transformed = vector_dimension_reduction(original);
        this->img_lib[name] = transformed;
        assert(img_lib[name].rows() == pca_dimension && img_lib[name].cols() == 1);
    }
    cout << "Reading library done!" << endl;
}

MatrixXd pcaMosaicGenerator::matrix_reader_from_csv(string path) {
    ifstream src_file;
    src_file.open(path);
    string line;
    vector<vector<double>> tmp;
    while(getline(src_file, line)){
        stringstream linestream(line);
        string cell;
        vector<double> tmp_line;
        while(getline(linestream, cell, ',')){
            tmp_line.push_back(stod(cell));
        }
        tmp.push_back(tmp_line);
    }
    assert(tmp.size() > 0 && tmp[0].size() > 0);
    long height = tmp.size(), width = tmp[0].size();
    MatrixXd result(height, width);
    for(int row = 0; row < height; row++){
        for(int col = 0; col < width; col++){
            result(row, col) = tmp[row][col];
        }
        tmp[row].clear();
    }
    tmp.clear();
    return result;
}

VectorXd pcaMosaicGenerator::vector_dimension_reduction(VectorXd &vector){
    assert(vector.rows() == vector_mean.rows() &&
           vector.rows() == vector_stddev.rows());
    VectorXd diff = vector - vector_mean;
    for(int i = 0; i < vector.rows(); i++){
        diff[i] /= vector_stddev[i];
    }
    VectorXd result = (diff.transpose() * convert_matrix).transpose();
    return result;
}