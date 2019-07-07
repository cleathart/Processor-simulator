#pragma once

#include <vector>
#include <cstdint>
#include <iostream>

class Decoded_instruction
{   
    public: 

    Decoded_instruction(int instruction);
    ~Decoded_instruction();
    
    int get_opcode();
    //sort out a bug do not really want to have to use
    void set_opcode(int opcode);
    int number_operands();
    int get_operand(int n);

    void print_instruction();

    private:
    int opcode;
    std::vector<int> operands;
};