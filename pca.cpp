//
// Created by Zhiyuan Wang on 26/06/2017.
//

#include "pca.h"

pca::pca(mosaicGenerator &generator, int eigen_number) {
    this->eigen_num = eigen_number;
    this->hist_list = vector<VectorXd>();
    unordered_map<string, colorHistVector>* map_ptr = generator.get_img_lib();
    this->color_level_count = (*map_ptr).begin()->second.get_color_level_count();
    this->vector_dimension = color_level_count * color_level_count * color_level_count;
    for(unordered_map<string, colorHistVector>::iterator it = (*map_ptr).begin(); it != (*map_ptr).end(); it++){
        this->hist_list.push_back(unfold_colorhist(it->second));
    }
}

pca::pca(string basic_path, int eigen_number) {
    this->eigen_num = eigen_number;
    this->hist_list = vector<VectorXd>();
    string hist_path = basic_path + "aflw 2/data/flickr/colorHist/";
    ifstream list_file(basic_path + "list.txt");
    string line;
    while(getline(list_file, line)){
        string name = line.substr(0, line.length()-4);
        cout << name << endl;
        colorHistVector chv(hist_path + name + ".json");
        if(this->vector_dimension == 0){
            this->color_level_count = chv.get_color_level_count();
            this->vector_dimension = color_level_count * color_level_count * color_level_count;
        }
        this->hist_list.push_back(unfold_colorhist(chv));
    }
}

//Placeholder
void pca::get_eigen_colorhist() {

}

//Placeholder
VectorXd pca::unfold_colorhist(colorHistVector &hist) {
    VectorXd result(vector_dimension);
    vector<vector<vector<double>>>* ptr = hist.get_ptr_to_hist();
    int count = 0;
    for(int r = 0; r < color_level_count; r++){
        for(int g = 0; g < color_level_count; g++){
            for(int b = 0; b < color_level_count; b++){
                result[count] = (*ptr)[r][g][b];
                count++;
            }
        }
    }
    return result;
}