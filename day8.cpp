#include <algorithm>
#include <numeric>
#include <cstring>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;


vector<string> split(string s, string delimiter=" ") {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        if (token != "")
            res.push_back (token);
    }
    res.push_back (s.substr (pos_start));
    return res;
}


void solve_puzzle_p1(string infile_name){
    ifstream infile(infile_name);
    string turns;
    getline(infile, turns);

    map<string,vector<string>> destinations;
    string line;
    getline(infile, line); // empty line
    assert (line == "");
    while(getline(infile, line)){
        size_t first_blank_idx = line.find(" ");
        vector<string> turn_destinations = split(line.substr(first_blank_idx + 4, line.size() - first_blank_idx - 5), ", ");
        destinations[line.substr(0,first_blank_idx)] = turn_destinations;
    }
    string current_loc = "AAA";
    unsigned no_steps = 0;
    while (current_loc != "ZZZ"){
        int current_turn = turns[no_steps % turns.size()] == 'R' ? 1 : 0;
        current_loc = destinations[current_loc][current_turn];
        no_steps++;
    }
    cout << "Part 1: " << no_steps << endl;
}

void solve_puzzle_p2(string infile_name){
    ifstream infile(infile_name);
    string turns;
    getline(infile, turns);

    map<string,vector<string>> destinations;
    vector<string> start_locations;
    string line;
    getline(infile, line); // empty line
    assert (line == "");
    while(getline(infile, line)){
        size_t first_blank_idx = line.find(" ");
        vector<string> turn_destinations = split(line.substr(first_blank_idx + 4, line.size() - first_blank_idx - 5), ", ");
        string start_loc = line.substr(0,first_blank_idx);
        if (start_loc[2] == 'A'){
            start_locations.push_back(start_loc);
        }
        destinations[line.substr(0,first_blank_idx)] = turn_destinations;
    }
    vector<long unsigned> done_steps(start_locations.size(), 0);
    bool all_done = false;
    long unsigned no_steps = 0;
    while(!all_done){
        int current_turn = turns[no_steps % turns.size()] == 'R' ? 1 : 0;
        int index = 0;
        for (auto &current_loc: start_locations){
            current_loc = destinations[current_loc][current_turn];
            if (current_loc[2] == 'Z'){
                done_steps[index] = no_steps + 1;
            }
            index++;
        }
        no_steps++;
        long unsigned min_done_steps = *min_element(done_steps.begin(), done_steps.end());
        if (min_done_steps){
            long unsigned least_common_multiple = 1;
            for (auto &steps: done_steps){
                least_common_multiple = lcm(least_common_multiple, steps);
            }
            no_steps = least_common_multiple;
            all_done = true;
        }
    }
    cout << "Part 2: " << no_steps << endl;
}

int main(){
    solve_puzzle_p1("day8_example_data.txt");
    solve_puzzle_p1("day8_example_data2.txt");
    solve_puzzle_p1("day8_test_data.txt");
    solve_puzzle_p2("day8_example_data3.txt");
    solve_puzzle_p2("day8_test_data.txt");
}