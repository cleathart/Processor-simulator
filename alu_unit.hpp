#pragma once

#include "reservation_station.hpp"

typedef struct {
    int issue_num;
    int result;
} alu_output;

class ALU_Unit
{
    public:
    ALU_Unit();
    ~ALU_Unit();

    //puts instruction into ALU
    void get_ins(rs_output *ins, int current_clock);
    //execute instruction in ALU
    void exec();
    //check execution unit is finished
    bool finished_exec(int current_clock);
    //gives output to so can be transmitted over the CDB
    alu_output *get_output(int current_clock);

    bool is_busy();
    void print();
    
    private:

    rs_output *current_ins;
    int start_clock;
    int done_clock;
    int result;
    bool busy;
    int id;
    static int counter;
};