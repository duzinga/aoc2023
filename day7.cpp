#include <algorithm>
#include <cstring>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;
#define ENABLE_PART2 1
struct Hand{
#if ENABLE_PART2
    const char cards_ordered[13] = {'J', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'Q', 'K', 'A'};
#else
    const char cards_ordered[13] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
#endif
    string card_info;
    unsigned card_type;
    Hand(string cards, unsigned type){
        card_type = type;
        card_info = cards;
        assert(5==card_info.length());
    }
    bool operator <(const Hand& rhs) const{
        if (card_type != rhs.card_type){
            return card_type < rhs.card_type;
        }

        for (size_t i = 0; i < card_info.length(); i++){
            if (card_info[i] != rhs.card_info[i]){
                int mycard_strength = std::distance(cards_ordered, std::find(cards_ordered, cards_ordered + sizeof(cards_ordered), card_info[i]));
                int rhscard_strength = std::distance(cards_ordered, std::find(cards_ordered, cards_ordered + sizeof(cards_ordered), rhs.card_info[i]));
                return mycard_strength < rhscard_strength;  
            }
        }
        return false;
        
    }
};

struct Bid {
    unsigned long bid;
    Bid(unsigned long b){
        bid = b;
    }
};

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


unsigned get_type(string hand){
    sort(hand.begin(),hand.end());
    unsigned type = 0;
    char last_char = ' ';
    unsigned char_repetition_count = 0;
    unsigned most_common_char_repetition_count = 0;
    char most_common_char = ' ';
    unsigned J_count = 0;
    for (auto c : hand){
        if (c == 'J'){
            J_count++;
            continue;
        }
        if (c == last_char){
            char_repetition_count++;
            if (char_repetition_count > most_common_char_repetition_count){
                most_common_char = c;
                most_common_char_repetition_count = char_repetition_count;
            }
        } else{
            if (char_repetition_count > 0){
                type += 2* char_repetition_count - 1;
            }
            char_repetition_count = 0;
            last_char = c;
        }
    }
    if (char_repetition_count > 0){
        type += 2 * char_repetition_count - 1;
        if (char_repetition_count > most_common_char_repetition_count){
            most_common_char = last_char;
            most_common_char_repetition_count = char_repetition_count;
        }
    }

#if ENABLE_PART2 
    if(J_count){
        if (most_common_char == ' '){
            if (last_char == ' '){
                most_common_char = 'A';
            } else {
                most_common_char = last_char;
            }
        }
        replace(hand.begin(), hand.end(), 'J', most_common_char);
        type = get_type(hand);
    }
#endif
    return type;
}

void solve_puzzle(string infile_name){
    ifstream infile(infile_name);
    string line;
    map<Hand, unsigned long> hands;
    while(getline(infile, line)){
        vector<string> hand_info  = split(line);
        hands.insert(make_pair(Hand(hand_info[0], get_type(hand_info[0])), stoul(hand_info[1], nullptr)));
    }

    unsigned long total_winnings = 0;
    unsigned rank = 1;
    for (auto const& [key, val] : hands) {
        total_winnings += rank++ * val; 
    }

#if ENABLE_PART2
    cout << "Part2: ";
#else
    cout << "Part1: ";
#endif
    cout << total_winnings << endl;
}

int main(){
    solve_puzzle("day7_example_data.txt");
    solve_puzzle("day7_test_data.txt");
}