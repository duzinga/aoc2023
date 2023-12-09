#include "utils.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;


struct MapType {
    int long src_start;
    int long dest_start;
    int long range;
    MapType(int long dest, int long src, int long len){
        src_start = src;
        dest_start = dest;
        range = len;
    }
};

int long win_count(int long time, int long distance){
    int long wins=0;
    for (int long i = 1; i < time; i++)
    {
        if (((time - i) * i) > distance){
            wins++;
        }
    }
    return wins;
}

void solve_puzzle(string infile_name){
    ifstream infile(infile_name);
    string line;
    getline(infile, line);
    vector<string> number_strings = split(line.substr(line.find(":") + 1));
    string one_num;
    for (auto &num : number_strings){
        one_num += num;
    }
    int long one_big_time = stoul(one_num, nullptr);
    vector<int long> time_list = str_to_int(number_strings);

    getline(infile, line);
    number_strings = split(line.substr(line.find(":") + 1));
    one_num = "";
    for (auto &num : number_strings){
        one_num += num;
    }
    int long one_big_distance = stoul(one_num, nullptr);
    vector<int long> distance_list = str_to_int(split(line.substr(line.find(":") + 1)));

    int long win_product = 1;
    for (size_t i = 0; i < time_list.size(); i++)
    {
        win_product *= win_count(time_list[i], distance_list[i]);
    }
    cout << "Part 1: " << win_product << endl;
    cout << "Part 2: " << win_count(one_big_time, one_big_distance) << endl;

}

int main(){
    solve_puzzle("day6_example_data.txt");
    solve_puzzle("day6_test_data.txt");
}