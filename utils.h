#include <vector>
#include <string>

std::vector<std::string> split(std::string s, std::string delimiter=" ") {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        if (token != "")
            res.push_back (token);
    }
    res.push_back (s.substr (pos_start));
    return res;
}

std::vector<int long> str_to_int(std::vector<std::string> input){
    std::vector<int long> res;
    for (auto val : input){
        res.push_back(std::stoul(val, nullptr));
    }
    return res;
}
