#include "utils.h"

#include <algorithm>
#include <numeric>
#include <map>
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

struct PipeInfo{
    unsigned x;
    unsigned y;
    char pipe;
    PipeInfo * prev;
    PipeInfo * next;
    PipeInfo(const PipeInfo& a) : PipeInfo(&a) {}
    PipeInfo(const PipeInfo* p_a) : x(p_a->x), y(p_a->y), pipe(p_a->pipe), prev(p_a->prev), next(p_a->next) {}
    PipeInfo(){}
    PipeInfo(char pipe_char, unsigned x_coord, unsigned y_coord){
        pipe = pipe_char;
        x = x_coord;
        y = y_coord;
    }

    PipeInfo& operator=(const PipeInfo& rhs) 
    {
        x = rhs.x;
        y = rhs.y;
        pipe = rhs.pipe;
        next = rhs.next;
        prev = rhs.prev;
        return * this;
    }

    bool operator==(const PipeInfo& a) {
        return (x == a.x) && (y == a.y) && (a.pipe == pipe);
    }

    bool operator!=(const PipeInfo& a) {
        return !(*this == a);
    }
};

PipeInfo get_prev_pipe_from_s(PipeInfo current_pipe, vector<string> &pipemap){
    assert (current_pipe.pipe == 'S');
    if (current_pipe.x > 0){
        switch (pipemap[current_pipe.y][current_pipe.x-1]){
            case '-':
            return PipeInfo('-', current_pipe.x-1, current_pipe.y);
            case 'F':
            return PipeInfo('F', current_pipe.x-1, current_pipe.y);
            case 'L':
            return PipeInfo('L', current_pipe.x-1, current_pipe.y);
            default:
                assert (current_pipe.y+1 < pipemap.size());
        }
    }
    switch (pipemap[current_pipe.y+1][current_pipe.x])
    {
        case '|':
            return PipeInfo('|', current_pipe.x, current_pipe.y+1);
        case 'J':
            return PipeInfo('J', current_pipe.x, current_pipe.y+1);
        case 'L':
            return PipeInfo('L', current_pipe.x, current_pipe.y+1);
        default:
            assert(false);
    }
}

PipeInfo get_next_pipe_from_s(PipeInfo current_pipe, vector<string> &pipemap){
    assert (current_pipe.pipe == 'S');
    if (current_pipe.x+1 < pipemap[0].length()){
        switch (pipemap[current_pipe.y][current_pipe.x+1]){
            case '-':
            return PipeInfo('-', current_pipe.x+1, current_pipe.y);
            case 'J':
            return PipeInfo('J', current_pipe.x+1, current_pipe.y);
            case '7':
            return PipeInfo('7', current_pipe.x+1, current_pipe.y);
            default:
                assert (current_pipe.y != 0);
        }
    }
    switch (pipemap[current_pipe.y-1][current_pipe.x])
    {
        case '|':
            return PipeInfo('|', current_pipe.x, current_pipe.y-1);
        case 'F':
            return PipeInfo('F', current_pipe.x, current_pipe.y-1);
        case '7':
            return PipeInfo('7', current_pipe.x, current_pipe.y-1);
        default:
            assert(false);
    }
}

PipeInfo* get_next_pipe(PipeInfo prev_pipe, PipeInfo current_pipe, vector<string> &pipemap){
    switch (current_pipe.pipe){
        case '|':
            if(current_pipe.y > prev_pipe.y)
                return new PipeInfo(pipemap[current_pipe.y+1][current_pipe.x], current_pipe.x, current_pipe.y + 1);
            else
                return new PipeInfo(pipemap[current_pipe.y-1][current_pipe.x], current_pipe.x, current_pipe.y - 1);
        case '-':
            if(current_pipe.x < prev_pipe.x)
                return new PipeInfo(pipemap[current_pipe.y][current_pipe.x - 1], current_pipe.x-1, current_pipe.y);
            else
                return new PipeInfo(pipemap[current_pipe.y][current_pipe.x + 1], current_pipe.x+1, current_pipe.y);
        case 'L':
            if(current_pipe.y > prev_pipe.y)
                return new PipeInfo(pipemap[current_pipe.y][current_pipe.x + 1], current_pipe.x+1, current_pipe.y);
            else
                return new PipeInfo(pipemap[current_pipe.y-1][current_pipe.x], current_pipe.x, current_pipe.y-1);
        case 'J':
            if(current_pipe.y > prev_pipe.y)
                return new PipeInfo(pipemap[current_pipe.y][current_pipe.x - 1], current_pipe.x-1, current_pipe.y);
            else
                return new PipeInfo(pipemap[current_pipe.y-1][current_pipe.x], current_pipe.x, current_pipe.y-1);
        case '7':
            if(current_pipe.y < prev_pipe.y)
                return new PipeInfo(pipemap[current_pipe.y][current_pipe.x-1], current_pipe.x-1, current_pipe.y);
            else
                return new PipeInfo(pipemap[current_pipe.y+1][current_pipe.x], current_pipe.x, current_pipe.y+1);
        case 'F':
            if(current_pipe.y < prev_pipe.y)
                return new PipeInfo(pipemap[current_pipe.y][current_pipe.x+1], current_pipe.x+1, current_pipe.y);
            else
                return new PipeInfo(pipemap[current_pipe.y+1][current_pipe.x], current_pipe.x, current_pipe.y+1);
    }
    assert(false);
    return new PipeInfo();
}


int area(vector<PipeInfo> &points) {
    int areaVal = 0;
    vector<PipeInfo>::iterator current, next;
    current = points.begin();
    next = points.begin();
    int firstX = current->x;
    int firstY = current->y;
    for (next++; next != points.end(); current++, next++) {
        int x1 = current->x;
        int y2 = next->y;
        int x2 = next->x;
        int y1 = current->y;
        int currEval = (x1 * y2) - (x2 * y1);
        areaVal += currEval;
    }

    int x2 = current->x;
    int y2 = current->y;

    int currEval = (x2 * firstY) - (firstX * y2);

    areaVal += currEval;

    return areaVal/2;
}

int interiorPoints(int area, int boundary_points) {
    int inter_points = area + 1 - (boundary_points/2);
    return inter_points;
}


void solve_puzzle(string infile_name){
    ifstream infile(infile_name);
    string line;
    vector<string> pipemap;
    PipeInfo s_pipe = PipeInfo();
    while(getline(infile, line)){
        size_t s_xpos = line.find("S");
        if (s_xpos != string::npos){
            s_pipe.pipe = 'S';
            s_pipe.x = s_xpos;
            s_pipe.y = pipemap.size();
        }
        pipemap.push_back(line);
    }

    PipeInfo next_pipe_cw = get_next_pipe_from_s(s_pipe, pipemap);
    PipeInfo *p_next_pipe_cw = &next_pipe_cw;
    next_pipe_cw.prev = &s_pipe;
    s_pipe.next = p_next_pipe_cw;
    //cout << next_pipe_cw.pipe << " " << next_pipe_cw.x << " " << next_pipe_cw.y <<endl;
    PipeInfo next_pipe_acw = get_prev_pipe_from_s(s_pipe, pipemap);
    PipeInfo *p_next_pipe_acw = &next_pipe_acw;
    next_pipe_acw.next = &s_pipe;
    s_pipe.prev = p_next_pipe_acw;
    vector<PipeInfo> pipes;
    pipes.push_back(s_pipe);
    //cout << "awc: " << next_pipe_acw.pipe << " " << next_pipe_acw.x << " " << next_pipe_acw.y <<endl;
    PipeInfo prev_pipe_cw(s_pipe);
    PipeInfo prev_pipe_acw(s_pipe);

    
    unsigned no_pipes = 1;
    do
    {
        PipeInfo *p_current_pipe_cw = p_next_pipe_cw;
        p_next_pipe_cw = get_next_pipe(prev_pipe_cw, *p_current_pipe_cw, pipemap);
        p_current_pipe_cw->next = p_next_pipe_cw;
        p_next_pipe_cw->prev = p_current_pipe_cw;
        pipes.push_back(*p_current_pipe_cw);
        prev_pipe_cw = *p_current_pipe_cw;

        //cout << next_pipe_cw.pipe << " " << next_pipe_cw.x << " " << next_pipe_cw.y <<endl;
        PipeInfo *p_current_pipe_acw = p_next_pipe_acw;
        p_next_pipe_acw = get_next_pipe(prev_pipe_acw, *p_current_pipe_acw, pipemap);
        p_current_pipe_acw->prev = p_next_pipe_acw;
        p_next_pipe_acw->next = p_current_pipe_acw;
        pipes.push_back(*p_current_pipe_acw);
        prev_pipe_acw = *p_current_pipe_acw;
        //cout << "awc: " << next_pipe_acw.pipe << " " << next_pipe_acw.x << " " << next_pipe_acw.y <<endl;
        no_pipes++;
    } while (*p_next_pipe_acw != *p_next_pipe_cw);

    p_next_pipe_acw->prev = p_next_pipe_cw->prev;
    p_next_pipe_cw->next = p_next_pipe_acw->next;
    pipes.push_back(*p_next_pipe_acw);

    cout << "Part 1: " << no_pipes << endl;

    // https://en.wikipedia.org/wiki/Shoelace_formula
    vector<PipeInfo> edge_points;
    assert(pipes[0].pipe=='S');
    edge_points.push_back(pipes[0]);
    PipeInfo *next = pipes[0].next;
    do{
        //cout << next->pipe << endl;
        if (next->pipe != '-' && next->pipe!='|'){
            edge_points.push_back(*next);
            }
        next = next->next;
    }while(next->pipe != 'S');
    

    int shape_area = area(edge_points);
    if (shape_area < 0){
        shape_area = -1 * shape_area;
    }

    cout << "Part 2: " << interiorPoints(shape_area, no_pipes*2) << endl;
}


int main(){
    solve_puzzle("day10_example_data1.txt");
    solve_puzzle("day10_example_data.txt");
    solve_puzzle("day10_test_data.txt");
}