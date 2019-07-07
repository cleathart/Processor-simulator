#pragma once

#include "alu_unit.hpp"
#include "branch_unit.hpp"
#include "decoded_instruction.hpp"
#include "ls_unit.hpp"
#include "reorder_buffer.hpp"
#include "register_allocation_table.hpp"
#include "reservation_station.hpp"
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

class OoO_Processor
{
    public:
    OoO_Processor(int num_regs, int memory_size, int num_alus);
    ~OoO_Processor();
    
    void load_program(std::string file_name);

    void fetch();

    void decode();

    void issue();

    void dispatch();

    void transmitCDB();

    void write_back();

    bool can_run();

    void increment_clock();
    int current_clock();
    int num_exec();

    void print_mem();
    void print_rf();
    void print_rob();
    void print_rat();
    void print_decode_out();
    void print_alu_rs();
    void print_branch_rs();
    void print_ls_rs();
    void print_alus();
    void print_CDB();

    private:

    int pc;
    int clock_tick;
    int ins_exec;
    int instructions_issued;

    std::vector<int> program_file;
    std::list<int> instruction_queue; //instruction queue
    std::vector<int> register_file; //register file
    Register_Allocation_Table *rat; //rat
    Reorder_Buffer *rob; //rob
    Decoded_instruction *decode_out; //decode unit
    std::vector<Reservation_Station *> alu_rs; //alus reservation stations
    Reservation_Station *branch_rs; //reservation station for rs only single one would not want to execute branch OoO
    std::vector<Reservation_Station *> ls_rs; //multiple reservation stations for load store unit
    std::vector<ALU_Unit *> alus; //alu units
    Branch_Unit *branch; //branch unit
    LS_Unit *load_store; // load store unit
    alu_output *cdb;//CDB
    //write back unit
};