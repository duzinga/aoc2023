#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;

struct PartNumber{
    int number;
    int x_start;
    int y;
    int x_end;
    PartNumber(){
        number = x_start = y = x_end = -1;
    }
};

struct Location{
    int x;
    int y;
    char c;
    Location(int _x, int _y, char spec_char) {
        x = _x;
        y = _y;
        c = spec_char;
    }
};
void solve_puzzle(string infile_name){
    ifstream infile(infile_name);
    vector<PartNumber> parts;
    vector<Location> char_locs;
    string line;
    int line_index=0;
    int line_size=0;
    parts.push_back(PartNumber());
    while(getline(infile, line)){
        line_size = line.size();
        for(unsigned i = 0; i < line_size; ++i) {
            if (isdigit(line[i])){
                if (parts.back().x_start == -1){
                    parts.back().x_start = i;
                    parts.back().y = line_index;
                }
                if (i == (line.size() - 1)){
                    parts.back().x_end = i;
                    parts.back().number = stoi(line.substr(parts.back().x_start, i-parts.back().x_start + 1), nullptr);
                    parts.push_back(PartNumber());
                }
            } else if (parts.back().x_start != -1) {
                parts.back().x_end = i - 1;
                parts.back().number = stoi(line.substr(parts.back().x_start, i-parts.back().x_start), nullptr);
                parts.push_back(PartNumber());
            }

            if (line[i] != '.' && !isdigit(line[i])){
                char_locs.push_back(Location(i, line_index, line[i]));
            }
        }
        line_index++;
    }
    parts.pop_back();
    vector<PartNumber> validated_parts;
    int sum_of_gear_ratios = 0;
    for (auto spec_char: char_locs){
        int gear_ratio = 1;
        int gears = 0;
        for (auto it = parts.begin(); it != parts.end(); ){
            if (abs((*it).y - spec_char.y) < 2){
                if (((*it).x_start - 1) <= spec_char.x && spec_char.x <= ((*it).x_end + 1)){
                    validated_parts.push_back(*it);
                    if (spec_char.c == '*'){
                        gears++;
                        gear_ratio *= (*it).number;
                    }

                    it = parts.erase(it);
                    continue;
                }
            }
            ++it;
        }
        if (gears == 2){
            sum_of_gear_ratios += gear_ratio;
        }

    }

    int sum_of_validated_parts = 0;
    for (auto part: validated_parts){
        sum_of_validated_parts += part.number;
    }
    cout << "sum_of_validated_parts: " << sum_of_validated_parts <<", sum_of_gear_ratios: " << sum_of_gear_ratios << endl;
}

int main(){
    solve_puzzle("day3_example_data.txt");
    solve_puzzle("day3_test_data.txt");
}