#include "decoded_instruction.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>

class Simulator 
{
    public:
    Simulator(int num_registers, int memory_size, std::string program_file);
    ~Simulator();

    int num_registers();
    int memory_size();
    int pc_val();
    
    //runs simulator 
    void run();

    //fetches instruction
    int fetch();
    //decodes instruction
    Decoded_instruction *decode(int instruction);
    //executes instruction
    int execute(Decoded_instruction *instruction);
    //writes back result to 
    void write_back(Decoded_instruction *instruction, int exec_result);

    int get_register_val(int reg_num);

    void print_register_file();

    void print_memory();
    
    private:

    std::vector<int> register_file;
    std::vector<int> memory;
    std::vector<int> program;
    int pc;
};