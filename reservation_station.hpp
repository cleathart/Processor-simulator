#pragma once

#include "decoded_instruction.hpp"
#include <iostream>
#include <map>
#include <vector>

typedef struct {
    int dispatch_num;
    int opcode;
    std::vector<int> *values;
} rs_output;

class Reservation_Station
{
    public:

    Reservation_Station();
    ~Reservation_Station();

    //instruction placed in reservation station
    void insert(int rob_val, Decoded_instruction *ins);
    //ask for registers values that are stored in rat
    std::vector<int> ask_reg_nums();
    //gets given updated register names from rat
    void get_reg_nums(std::vector<int> reg_nums);
    //ask for data
    std::map<int,int> ask_data();
    //get data from reg or rob 
    void get_data(int operand_num, int value);
    //check in RS is ready to dispatch
    bool ready_dispatch();
    //dispatch the result of reservation station
    rs_output *dispatch_rs();

    void bad_branch(int rob_val);

    bool is_busy();
    int get_wb_reg();
    int get_id();
    void print();

    private:

    int rob_val;
    int opcode;
    int write_reg;
    std::vector<int> operand_regs;
    std::vector<int> values;
    std::vector<bool> values_collected;

    int rs_id;
    bool busy;
    static int counter;
};