//
// Created by Zhiyuan Wang on 19/06/2017.
//

#include "imgSegmentation.h"

mRect::mRect(int in_x, int in_y, int in_width, int in_height){
    this->x = in_x;
    this->y = in_y;
    this->width = in_width;
    this->height = in_height;
}

mRect::mRect(Rect rect) {
    this->x = rect.x;
    this->y = rect.y;
    this->width = rect.width;
    this->height = rect.height;
}

bool mRect::operator==(const mRect &rect) {
    return (this->x == rect.x) && (this->width == rect.width) &&
           (this->y == rect.y) && (this->height == rect.height);
}

block::block(Mat const& img, mRect const& input_roi, const int colorRes) {
    assert(colorHistVector::checkInBound(img, input_roi));
    assert(input_roi.height == input_roi.width);
    this->size = input_roi.height;
    //this->roi = input_roi;
    this->colorhist = new colorHistVector(img, input_roi, colorRes);
}

block::~block() {
    delete colorhist;
    colorhist = nullptr;
}

imgSegmentation::imgSegmentation(Mat &img, int color_resolution, double sim_threshold, int min_size, int max_size):
    color_resolution(color_resolution), similarity_threshold(sim_threshold), min_size(min_size), max_size(max_size)
{
    //this->map = unordered_map<mRect, block*>();
    imgCropper(img, min_size);
}

imgSegmentation::~imgSegmentation() {
    for(auto it: this->map){
        delete it.second;
        it.second = nullptr;
    }
    this->map.clear();
}

void imgSegmentation::imgCropper(Mat &img, int min_size) {
    int width = img.cols, height = img.rows;
    int width_remainder = width % min_size, height_remainder = height % min_size;
    int final_width = width - width_remainder, final_height = height - height_remainder;
    this->mImg = img(Rect(width_remainder / 2, height_remainder / 2, final_width, final_height));
}

void imgSegmentation::segment() {
    initialize_segments();
    print();
    merge_segments();
    print();
}

void imgSegmentation::initialize_segments() {
    int x_num = this->mImg.cols / this->min_size,
        y_num = this->mImg.rows / this->min_size;
    for(int i = 0; i < x_num; i++){
        for(int j = 0; j < y_num; j++){
            const mRect rect(i * min_size, j * min_size, min_size, min_size);
            map[rect] = new block(mImg, rect, color_resolution);
        }
    }
}

void imgSegmentation::merge_segments(){
    cout << "Segment merging..." << endl;
    int line_width = 50, line_count = 0;
    bool stop = false;
    while(!stop){
        //cout << ".";
        //if((++line_count) % line_width == 0) cout << endl;
        cout << ++line_count << "   " << map.size() << endl;
        bool merges_exist = false;
        unordered_set<mRect> merged;
        for(auto it: map){
            if (merged.find(it.first) != merged.end()) continue;
            vector<vector<mRect>> candidates = get_candidate_rois(it.first);
            for(int i = 0; i < candidates.size(); i++){
                if(valid_for_merge(candidates[i], merged)){
                    mRect newroi = merge_rois(candidates[i]);
                    block* newblock = new block(this->mImg, newroi, this->color_resolution);
                    for(mRect& item: candidates[i]){
                        merged.insert(item);
                    }
                    map[newroi] = newblock;
                    merges_exist = true;
                    break;
                }
            }
        }
        for(auto it: merged){
            //delete entries (call destructor of block)
            delete map[it];
            //make entry in map clean
            map.erase(it);
        }
        stop = !merges_exist;
    }
    cout << endl;
}

vector<vector<mRect>> imgSegmentation::get_candidate_rois(const mRect &rect) {
    assert(rect.width == rect.height);
    vector<vector<mRect>> result;
    int x = rect.x, y = rect.y, w = rect.width, h = rect.height;
    mRect top1(x - w, y - h, w, h), top2(x, y - h, w, h), top3(x+w, y - h, w, h);
    mRect mid1(x - w, y, w, h), mid2(rect), mid3(x + w, y, w, h);
    mRect bot1(x - w, y + h, w, h), bot2(x, y + h, w, h), bot3(x + w, y + h, w, h);
    vector<mRect> topleft, topright, bottomleft, bottomright;
    topleft.push_back(top1); topleft.push_back(top2);
    topleft.push_back(mid1); topleft.push_back(mid2);
    topright.push_back(top2); topright.push_back(top3);
    topright.push_back(mid2); topright.push_back(mid3);
    bottomleft.push_back(mid1); bottomleft.push_back(mid2);
    bottomleft.push_back(bot1); bottomleft.push_back(bot2);
    bottomright.push_back(mid2); bottomright.push_back(mid3);
    bottomright.push_back(bot2); bottomright.push_back(bot3);
    result.push_back(topleft); result.push_back(topright);
    result.push_back(bottomleft); result.push_back(bottomright);
    return result;
}

bool imgSegmentation::valid_for_merge(const vector<mRect>& rois, const unordered_set<mRect>& merged) {
    assert(rois.size() == 4);
    //assume the rois have same size
    /*int width = rois[0].width, height = rois[0].height;
    for(int i = 1; i < 4; i++){
        if(rois[i].width != width || rois[i].height != height) return false;
    }*/
    for(int i = 0; i < 4; i++){
        if(map.find(rois[i]) == map.end()) return false;//verify the key exists in map
        if(merged.find(rois[i]) != merged.end()) return false;
    }
    for(int i = 0; i < 3; i++){
        for(int j = i + 1; j < 4; j++){
            if(colorHistVector::colorSimilarity(*(map[rois[i]]->colorhist),
                                                *(map[rois[j]]->colorhist)) < similarity_threshold)
                return false;
        }
    }
    return true;
}

mRect imgSegmentation::merge_rois(const vector<mRect> rois) {
    //assume the rois have same sizes
    int x0 = rois[0].x, y0 = rois[0].y;
    int x3 = rois[3].x, y3 = rois[3].y;
    int w = rois[0].width, h = rois[0].height;
    assert((x0 + w == x3) && (y0 + h == y3));
    return mRect(x0, y0, 2 * w, 2 * h);
}

void imgSegmentation::print() {
    cout << "********imgSegmentation Info*********" << endl;
    cout << "similarity_threshold = " << this->similarity_threshold << endl;
    cout << "min_size = " << this->min_size << ", max_size = " << this->max_size << endl;
    cout << "color_resolution = " << this->color_resolution << endl;
    cout << "size(map<mRect, block*>) = " << this->map.size() << endl;
    cout << "number of buckets = " << this->map.bucket_count() << endl;
    cout << "*************************************" << endl;
}

void imgSegmentation::saveMergeResult(string path) {
    Mat segment_result(mImg);
    Scalar color(255, 0, 0);
    for(unordered_map<mRect, block*>::iterator it = map.begin(); it != map.end(); it++){
        rectangle(segment_result, it->first, color);
    }
    imwrite(path, segment_result);
    imshow("Segmentation result", segment_result);
    cvWaitKey();
}