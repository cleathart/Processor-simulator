#pragma once

#include "reservation_station.hpp"
#include <vector>

typedef struct
{
    int issue_num;
    int result;
} ls_unit_output;


class LS_Unit
{
public:

    LS_Unit(int mem_size);
    ~LS_Unit();

    //gets new instruction in
    void get_ins(rs_output *ins, int current_clock);
    //execute current instruction
    void exec();
    //checks if finsihed executing
    bool finished_exec(int current_clock);
    //gives the output for the cdb
    ls_unit_output *get_output(int current_clock);

    bool is_busy();
    void print_memory();

private:

    std::vector<int> memory;

    rs_output *current_ins;
    int start_clock;
    int done_clock;
    int result;
    bool busy;
};


