#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;


size_t get_first_text_digit(string line, int &digit){
    vector<string> digits = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    size_t first_digit_index = string::npos;
    for(auto it = digits.begin(); it != digits.end(); ++it){
        size_t loc = line.find(*it);
        if (loc != string::npos){
            if (loc < first_digit_index){
                first_digit_index = loc;
                digit = it - digits.begin() + 1;
            }
        }
    }
    return first_digit_index;
}


size_t get_last_text_digit(string line, int &digit){
    vector<string> digits = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    size_t last_digit_index = string::npos;
    for(auto it = digits.begin(); it != digits.end(); ++it){
        string digit_str = string(*it);
        size_t loc = string::npos;

        auto found_it = find_end(line.begin(), line.end(), digit_str.begin(), digit_str.end());
        if (found_it != line.end()){
            loc = found_it - line.begin();
        }
        if (loc != string::npos){
            if (loc > last_digit_index || last_digit_index == string::npos){
                last_digit_index = loc;
                digit = it - digits.begin() + 1;
            }
        }
    }
    return last_digit_index;
}


void solve_puzzle(string infile_name){
    ifstream infile(infile_name);
    string line;
    int sum_of_all = 0;
    while(getline(infile, line)){
        const int UNINIT = -100;
        int first_digit = UNINIT;
        int last_digit = UNINIT;

        size_t first_digit_loc = get_first_text_digit(line, first_digit);
        size_t last_digit_loc = get_last_text_digit(line, last_digit);
        
        char prev_char = 0;
        for (int i = 0; i < line.size(); i++){
            char c = line[i];
            if (c < '0' || c >'9'){
                continue;
            }
            int val = static_cast<int>(c) - static_cast<int>('0');
            if (prev_char == '-'){
                val *= -1;
            }

            if (first_digit_loc > i){
                first_digit = val;
                first_digit_loc =  i;
            } else if (last_digit_loc == string::npos || last_digit_loc < i) {
                last_digit = val;
                last_digit_loc = i;
            }
            prev_char = c;
        }
        assert (first_digit != UNINIT);
        if(last_digit == UNINIT){
            last_digit = first_digit;
        }
        //cout << first_digit << " " << last_digit << endl;
        sum_of_all += first_digit * 10 + last_digit;
    }
    cout << sum_of_all << endl;
}

int main(){
    solve_puzzle("day1_example_data.txt");
    solve_puzzle("day1_example2_data.txt");
    solve_puzzle("day1_test_data.txt");
}