#include "utils.h"

#include <algorithm>
#include <numeric>
#include <map>
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

struct Part{
    map<char, unsigned long> properties;
    Part(const Part & rhs){
         for (auto const& [key, val] : rhs.properties){
            properties[key] = val;
        }
    }
    Part(){
        properties['x'] = 0;
        properties['m'] = 0;
        properties['a'] = 0;
        properties['s'] = 0;
    }
    Part(unsigned long coolness, unsigned long musicality, unsigned long aerodynamic_factor, unsigned long shinyness){
        properties['x'] = coolness;
        properties['m'] = musicality;
        properties['a'] = aerodynamic_factor;
        properties['s'] = shinyness;
    }
    Part& operator=(const Part& rhs) 
    {
        for (auto const& [key, val] : rhs.properties){
            properties[key] = val;
        }
        return * this;
    }
    unsigned long sum(){
        unsigned long summed_properties = 0;
        
        for (auto const& [key, val] : properties){
            summed_properties += val;
        }
        return summed_properties;
    }

    unsigned long product(){
        unsigned long multiplied_properties = 1;
        
        for (auto const& [key, val] : properties){
            multiplied_properties *= val;
        }
        return multiplied_properties;
    }

    friend Part operator-(const Part& lhs, const Part& rhs) {
        Part new_part;
        for (auto const& [key, val] : lhs.properties){
            new_part.properties[key] = val;
        }

        for (auto const& [key, val] : rhs.properties){
            new_part.properties[key] -= val;
        }

        return new_part;
    }

    void print(){
        cout << "x:" << properties['x'] << ", m: "<< properties['m'] << ", a: " << properties['a'] << ", s " << properties['s'] << endl;
    }
};

void build_rules(map<string, vector<string>> &rules, ifstream &infile){
    string line;
    while(getline(infile, line) && line != ""){
        size_t open_brace_idx = line.find("{");
        string name = line.substr(0,open_brace_idx);
        vector<string> rule_set =  split(line.substr(open_brace_idx+1, line.find("}") - open_brace_idx-1), ",");
        rules[name] = rule_set;
    }

}

bool check_rules(map<string, vector<string>> &rules, Part &part){
    string rule_name = "in";
    while (true){
        for (auto & rule: rules[rule_name]){
            if (rule[1] == '<' || rule[1] == '>'){
                unsigned long val = stoul(rule.substr(2, rule.find(":") - 2), nullptr);
                string destination = rule.substr(rule.find(":")+1, rule.length() - rule.find(":") - 1);
                if ( (rule[1] == '<' && val > part.properties[rule[0]]) ||
                    (rule[1] == '>' && val < part.properties[rule[0]]) ){
                    if (destination == "A") {
                        return true;
                    } else if (destination == "R"){
                        return false;
                    }
                    rule_name = destination;
                    break;
                }
            } else {
                if (rule == "A"){
                    return true;
                } else if(rule == "R"){
                    return false;
                }
                rule_name = rule;
                break;
            }
        }
    }
}


unsigned long get_ruleset_acceptance_combo(map<string, vector<string>> &rules, string rule_name, Part &part_min, Part &part_max){
    unsigned long acceptance_combo = 0;
    Part part_max_local(part_max);
    Part part_min_local(part_min);
    for (auto & rule: rules[rule_name]){
        Part part_max_apply(part_max_local);
        Part part_min_apply(part_min_local);
        if (rule[1] == '<' || rule[1] == '>'){
            unsigned long val = stoul(rule.substr(2, rule.find(":") - 2), nullptr);
            string destination = rule.substr(rule.find(":")+1, rule.length() - rule.find(":") - 1);
            if (rule[1] == '<'){
                part_max_apply.properties[rule[0]] = val;
                part_min_local.properties[rule[0]] = val;
            } else {
                part_min_apply.properties[rule[0]] = val + 1;
                part_max_local.properties[rule[0]] = val + 1;
            }
            if (destination == "A"){
                acceptance_combo += ((part_max_apply - part_min_apply).product());
                continue;
            }
            if (destination == "R"){
                continue;
            }

            acceptance_combo += get_ruleset_acceptance_combo(rules, destination, part_min_apply, part_max_apply);
        } else {
            if (rule == "A"){
                acceptance_combo += ((part_max_apply - part_min_apply).product());
                continue;
            }
            if (rule == "R"){
                continue;
                return 0;
            }
            acceptance_combo += get_ruleset_acceptance_combo(rules, rule, part_min_local, part_max_local);
        }
    }
    return acceptance_combo;
}

void solve_puzzle(string infile_name){
    ifstream infile(infile_name);
    map<string, vector<string>> rules;
    vector<Part> accepted_parts;
    build_rules(rules, infile);
    string line;
    unsigned long sum_of_all = 0;
    while(getline(infile, line)){
        vector<string> part_info = split(line.substr(1, line.length() - 2), ",");
        unsigned long x = stoul(part_info[0].substr(2,part_info[0].length() - 2), nullptr);
        unsigned long m = stoul(part_info[1].substr(2,part_info[1].length() - 2), nullptr);
        unsigned long a = stoul(part_info[2].substr(2,part_info[2].length() - 2), nullptr);
        unsigned long s = stoul(part_info[3].substr(2,part_info[3].length() - 2), nullptr);
        Part new_part(x,m,a,s);
        if (check_rules(rules, new_part)){
            accepted_parts.push_back(new_part);
            sum_of_all += new_part.sum();
        }
    }
    cout << "Part 1: " << sum_of_all << endl;
    Part min_part(1,1,1,1);
    Part max_part(4001,4001,4001,4001);
    cout << "Part 2: " <<( get_ruleset_acceptance_combo(rules, "in", min_part, max_part)) << endl;

   
}

int main(){
    solve_puzzle("day19_example_data.txt");
    solve_puzzle("day19_test_data.txt");
}
