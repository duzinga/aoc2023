#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;


struct MapType {
    unsigned long src_start;
    unsigned long dest_start;
    unsigned long range;
    MapType(unsigned long dest, unsigned long src, unsigned long len){
        src_start = src;
        dest_start = dest;
        range = len;
    }
};

std::vector<std::string> split(std::string s, std::string delimiter=" ") {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        if(token != "")
            res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

vector<unsigned long> str_to_int(vector<string> input){
    vector<unsigned long> res;
    for (auto val : input){
        res.push_back(stoul(val, nullptr));
    }
    return res;
}

unsigned long win_count(unsigned long time, unsigned long distance){
    unsigned long wins=0;
    for (unsigned long i = 1; i < time; i++)
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
    unsigned long one_big_time = stoul(one_num, nullptr);
    vector<unsigned long> time_list = str_to_int(number_strings);

    getline(infile, line);
    number_strings = split(line.substr(line.find(":") + 1));
    one_num = "";
    for (auto &num : number_strings){
        one_num += num;
    }
    unsigned long one_big_distance = stoul(one_num, nullptr);
    vector<unsigned long> distance_list = str_to_int(split(line.substr(line.find(":") + 1)));

    unsigned long win_product = 1;
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