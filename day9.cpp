#include "utils.h"

#include <algorithm>
#include <numeric>
#include <map>
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

int long predict_next_val(vector<int long> values){
    vector<int long> diffs;
    for (size_t i=0; i < values.size() - 1; i++){
        diffs.push_back(values[i+1] - values[i]);
    }
    int long next_val  = values[values.size() - 1];
    if (diffs[diffs.size() - 1]){
        next_val += predict_next_val(diffs);
    }
    return next_val;
}

int long predict_prev_val(vector<int long> values){
    vector<int long> diffs;
    for (size_t i=0; i < values.size() - 1; i++){
        diffs.push_back(values[i+1] - values[i]);
    }
    int long prev_val  = values[0];
    if (!all_of(diffs.begin(), diffs.end(), [](int long i) { return i==0; })){
        
        prev_val -= predict_prev_val(diffs);
    }
    return prev_val;
}

void solve_puzzle(string infile_name){
    ifstream infile(infile_name);
    string line;
    int long sum_of_next_predictions = 0;
    int long sum_of_prev_predictions = 0;
    while(getline(infile, line)){
        vector<string> data_str = split(line);
        vector<int long> data = str_to_int(data_str);
        sum_of_next_predictions += predict_next_val(data);
        sum_of_prev_predictions += predict_prev_val(data);
    }

    cout << "Part 1: " << sum_of_next_predictions << endl;
    cout << "Part 2: " << sum_of_prev_predictions << endl;
}


int main(){
    solve_puzzle("day9_example_data.txt");
    solve_puzzle("day9_test_data.txt");
}