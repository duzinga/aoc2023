#include <execution>
#include <algorithm>
#include <cstring>
#include <vector>
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

vector<unsigned long> str_to_int(vector<string> input){
    vector<unsigned long> res;
    for (auto val : input){
        res.push_back(stoul(val, nullptr));
    }
    return res;
}

vector<MapType> build_maps(ifstream& infile){
    string line;
    vector<MapType> map_res;
    while(getline(infile, line)){
        if (line == ""){
            break;
        }
        vector<string> split_line = split(line.substr(line.find(":") + 1), " ");
        vector<unsigned long> numbers = str_to_int(split_line);
        assert(numbers.size() == 3);
        map_res.push_back(MapType(numbers[0], numbers[1], numbers[2]));
    }
    return map_res;
}

unsigned long get_dest_from_src(unsigned long src, vector<MapType> src_dest_map){
    for (auto entry : src_dest_map){
        if (src >= entry.src_start && (entry.src_start + entry.range) >= src){
            return entry.dest_start + src - entry.src_start;
        }
    }
    return src;
}

void solve_puzzle(string infile_name){
    ifstream infile(infile_name);
    string line;
    getline(infile, line);
    assert(line.find("seeds:") != string::npos);
    vector<string> split_line = split(line.substr(line.find(":") +2), " ");
    vector<unsigned long> seeds = str_to_int(split_line);
    vector<MapType> seed_ranges;
    for (size_t i = 0; i < seeds.size(); i+=2)
    {
        seed_ranges.push_back(MapType(0, seeds[i], seeds[i+1]));
        
    }
    getline(infile, line); // blank
    getline(infile, line);
    assert(line.find("seed-to-soil map") != string::npos);
    vector<MapType> seed_to_soil_map = build_maps(infile);

    getline(infile, line);
    assert(line.find("soil-to-fertilizer map:") != string::npos);
    vector<MapType> soil_to_fertilizer_map = build_maps(infile);

    getline(infile, line);
    assert(line.find("fertilizer-to-water map:") != string::npos);
    vector<MapType> fertilizer_to_water_map = build_maps(infile);

    getline(infile, line);
    assert(line.find("water-to-light map:") != string::npos);
    vector<MapType> water_to_light_map = build_maps(infile);

    getline(infile, line);
    assert(line.find("light-to-temperature map:") != string::npos);
    vector<MapType> light_to_temperature_map = build_maps(infile);

    getline(infile, line);
    assert(line.find("temperature-to-humidity map:") != string::npos);
    vector<MapType> temperature_to_humidity_map = build_maps(infile);

    getline(infile, line);
    assert(line.find("humidity-to-location map:") != string::npos);
    vector<MapType> humidity_to_location_map = build_maps(infile);

    vector<unsigned long> locations;
    for (auto seed : seeds){
        unsigned long soil = get_dest_from_src(seed, seed_to_soil_map);
        unsigned long fertilizer = get_dest_from_src(soil, soil_to_fertilizer_map);
        unsigned long water = get_dest_from_src(fertilizer, fertilizer_to_water_map);
        unsigned long light = get_dest_from_src(water, water_to_light_map);
        unsigned long temperature = get_dest_from_src(light, light_to_temperature_map);
        unsigned long humidity = get_dest_from_src(temperature, temperature_to_humidity_map);
        unsigned long location = get_dest_from_src(humidity, humidity_to_location_map);
        locations.push_back(location);
    }
    cout << "Part 1: "<< (*min_element(locations.begin(), locations.end())) << endl;

    unsigned long min_loc = UINT64_MAX;
    for_each(
    std::execution::par,
    seed_ranges.begin(),
    seed_ranges.end(),
    [&min_loc, seed_to_soil_map, soil_to_fertilizer_map, fertilizer_to_water_map, water_to_light_map, light_to_temperature_map, temperature_to_humidity_map, humidity_to_location_map](auto& seed_range)
    {
        for (unsigned long seed = seed_range.src_start; seed < (seed_range.src_start + seed_range.range); seed++)
        {
            unsigned long soil = get_dest_from_src(seed, seed_to_soil_map);
            unsigned long fertilizer = get_dest_from_src(soil, soil_to_fertilizer_map);
            unsigned long water = get_dest_from_src(fertilizer, fertilizer_to_water_map);
            unsigned long light = get_dest_from_src(water, water_to_light_map);
            unsigned long temperature = get_dest_from_src(light, light_to_temperature_map);
            unsigned long humidity = get_dest_from_src(temperature, temperature_to_humidity_map);
            unsigned long location = get_dest_from_src(humidity, humidity_to_location_map);
            if (location < min_loc){
                min_loc = location;
            }
        }
    });

    cout << "Part 2: "<< min_loc << endl;

}

int main(){
    // Build with tbb: g++-12  day5.cpp -O3 --std=c++20 -ltbb
    solve_puzzle("day5_example_data.txt");
    solve_puzzle("day5_test_data.txt");
}