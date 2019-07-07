#include "simulator.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    int num_reg = 16;   //32 bit registers
    int mem_size = 256; // 1kb of memeory
    int clock_tick = 0;
    int ins_exec = 0;

    Simulator *proc_sim = new Simulator(num_reg, mem_size, argv[1]);

    while (1)
    {
        int ins_val = proc_sim->fetch();
        clock_tick++;
        if (ins_val == 0)
        {
            break;
        }
        else
        {
            Decoded_instruction *ins_dec = proc_sim->decode(ins_val);
            clock_tick++;
            int exec_val = proc_sim->execute(ins_dec);
            clock_tick++;
            ins_exec++;
            proc_sim->write_back(ins_dec, exec_val);
            clock_tick++;
        }       
    }

    proc_sim->print_memory();

    std::cout << "Program has run" << std::endl;
    std::cout << "Clock cycles: " << clock_tick << std::endl;
    std::cout << "Instructions executed: " << ins_exec << std::endl;
    float ipc = (float)ins_exec / (float)clock_tick;
    std::cout << "IPC: " << ipc << std::endl;

    return 0;
}
