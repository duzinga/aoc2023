#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;

const int RED_COUNT = 12;
const int GREEN_COUNT =  13;
const int BLUE_COUNT =  14;

std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

bool is_possible(string game_text, int& min_power){
    string delimiter = ";";
    size_t pos = 0;
    std::string game_result;
    vector<string> games = split(game_text, "; ");
    int min_blues=1, min_greens=1, min_reds = 1;
    bool game_is_possible = true;
    for (auto game: games){
        vector<string> balls = split(game, ", ");
        for (auto ball_count: balls){
            size_t count_end = ball_count.find(" ");
            assert (count_end != string::npos);
            int count = stoi(ball_count.substr(0, count_end), nullptr);
            if (ball_count.find("blue") != string::npos){
                if (count > min_blues)
                    min_blues = count;
                if (count > BLUE_COUNT)
                    game_is_possible = false;
            } else if (ball_count.find("red") != string::npos){
                if (count > min_reds)
                    min_reds = count;
                if (count > RED_COUNT)
                    game_is_possible = false;
            } else {
                if (count > min_greens)
                    min_greens = count;
                if (count > GREEN_COUNT){
                        game_is_possible = false;
                }
            }
        }
    }
    min_power = min_blues * min_greens * min_reds;
    return game_is_possible;
}

void solve_puzzle(string infile_name){
    ifstream infile(infile_name);
    string line;
    int sum = 0, power_sum = 0;
    while(getline(infile, line)){
        size_t game_id_end = line.find(": ");
        size_t game_id_start = line.find(" ") + 1;
        int min_power = 1;
        int game_id = stoi(line.substr(game_id_start, game_id_end-game_id_start), nullptr);
        if (is_possible(line.substr(game_id_end+2), min_power)){
            sum += game_id;
        }
        power_sum += min_power;
    }
    cout << "possibility sum: " << sum << endl;
    cout << "power sum: " << power_sum << endl;

}

int main(){
    solve_puzzle("day2_example_data.txt");
    solve_puzzle("day2_test_data.txt");
}