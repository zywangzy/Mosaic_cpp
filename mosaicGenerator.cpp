//
// Created by Zhiyuan Wang on 24/06/2017.
//

#include "mosaicGenerator.h"

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