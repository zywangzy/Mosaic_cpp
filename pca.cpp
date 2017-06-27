//
// Created by Zhiyuan Wang on 26/06/2017.
//

#include "pca.h"

pca::pca(mosaicGenerator &generator) {
    this->hist_list = vector<Mat>();
    unordered_map<string, colorHistVector>* map_ptr = generator.get_img_lib();
    for(unordered_map<string, colorHistVector>::iterator it = (*map_ptr).begin(); it != (*map_ptr).end(); it++){
        this->hist_list.push_back(unfold_colorhist(it->second));
    }
}

pca::pca(string basic_path) {
    this->hist_list = vector<Mat>();
    string hist_path = basic_path + "aflw 2/data/flickr/colorHist/";
    ifstream list_file(basic_path + "list.txt");
    string line;
    while(getline(list_file, line)){
        string name = line.substr(0, line.length()-4);
        cout << name << endl;
        colorHistVector chv(hist_path + name + ".json");
        this->hist_list.push_back(unfold_colorhist(chv));
    }
}

//Placeholder
void pca::get_eigen_colorhist() {

}

//Placeholder
Mat pca::unfold_colorhist(colorHistVector &hist) {
    Mat result();
    return result;
}