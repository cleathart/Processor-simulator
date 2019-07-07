#include "../OoO_Processor.hpp"

int main(int argc, char const *argv[])
{
    OoO_Processor *test_proc = new OoO_Processor(16, 256, 1);

    test_proc->load_program("assembler/assembled_progs/throughput_mul.prog");

    while(test_proc->can_run())
    {
        test_proc->write_back();
        test_proc->transmitCDB();
        test_proc->dispatch();
        test_proc->issue();
        test_proc->decode();
        test_proc->fetch();


        test_proc->increment_clock();
        // if (test_proc->current_clock() == 6)
        // {
        //     break;
        // }
    }
    
    test_proc->print_decode_out();
    test_proc->print_mem();
    test_proc->print_rf();
    test_proc->print_rat();
    test_proc->print_rob();
    test_proc->print_alu_rs();
    test_proc->print_branch_rs();
    test_proc->print_ls_rs();
    test_proc->print_alus();
    test_proc->print_CDB();

    std::cout << "Program has run" << std::endl;
    std::cout << "Clock cycles: " << test_proc->current_clock() << std::endl;
    std::cout << "Instructions executed: " << test_proc->num_exec() << std::endl;
    float ipc = (float)test_proc->num_exec() / (float)test_proc->current_clock();
    std::cout << "IPC: " << ipc << std::endl;
    
    return 0;
}
