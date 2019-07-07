#pragma once
#include "reservation_station.hpp"

typedef struct {
    int issue_num;
    bool branch_taken;
    int result;
} branch_unit_output;

class Branch_Unit
{
public:
    Branch_Unit();
    ~Branch_Unit();

    //get a new instruction into brnach unit
    void get_ins(rs_output *ins, int current_clock);
    //execute branch
    void exec();
    //check is exec has finsihed
    bool finished_exec(int current_clock);
    //return result of branch
    branch_unit_output *get_output(int current_clock);

    bool is_busy();
    void print();

private:
    rs_output *current_ins;
    bool busy;
    int start_clock;
    int done_clock;
    branch_unit_output *output;
};
