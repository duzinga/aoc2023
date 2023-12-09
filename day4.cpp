#include "utils.h"
#include <deque>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;

std::vector<std::string> intersection(std::vector<std::string> v1,
                                      std::vector<std::string> v2){
    std::vector<std::string> v3;

    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    std::set_intersection(v1.begin(),v1.end(),
                          v2.begin(),v2.end(),
                          back_inserter(v3));
    return v3;
}

void solve_puzzle(string infile_name){
    ifstream infile(infile_name);
    string line;
    int sum_of_winning_points = 0;
    deque<int> copy_cards;
    copy_cards.push_back(0);
    int total_cards_with_copies = 0;

    while(getline(infile, line)){
        string num_str = line.substr(line.find(":")+2);
        vector<string> winning_numbers = split(num_str.substr(0, num_str.find(" |")), " ");
        // Remove empty entries (happens when multiple blanks are in the data)
        for (auto it = winning_numbers.begin(); it != winning_numbers.end(); ){
            if ((*it) == "")
                it = winning_numbers.erase(it);
            else
            ++it;
        }

        vector<string> drawn_numbers = split(num_str.substr(num_str.find("|") + 2), " ");
        vector<string> nums_overlap = intersection(winning_numbers, drawn_numbers);

        if (nums_overlap.size()){
          sum_of_winning_points += 1 << (nums_overlap.size() - 1);
        }

        if (!copy_cards.size()){
            copy_cards.push_back(0);
        }
        
        for (size_t i = 0; i < nums_overlap.size(); i++)
        {
            if (copy_cards.size() > (i + 1)){
                copy_cards[i+1] += copy_cards.front() + 1;
            } else{
                copy_cards.push_back(copy_cards.front() + 1);
            }
        }
        total_cards_with_copies += copy_cards.front() + 1;
        copy_cards.pop_front();
        
    }
    cout << "Part 1: " <<  sum_of_winning_points << endl;
    cout << "Part 2: " <<  total_cards_with_copies << endl;
}

int main(){
    solve_puzzle("day4_example_data.txt");
    solve_puzzle("day4_test_data.txt");
}