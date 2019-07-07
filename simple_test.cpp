#include "simulator.hpp"

void test_constructor()
{
    int num_errors = 0;
    int num_reg = 16;
    int mem_size = 256;
    
    Simulator *test_sim = new Simulator(num_reg, mem_size, "fib.prog");

    if (test_sim->num_registers() != num_reg)
    {
        std::cerr << "Number of registers not being allocated correctly" << std::endl;
        num_errors++;
    }

    if (test_sim->memory_size() != mem_size)
    {
        std::cerr << "Size of memory for simulator is incorrect" << std::endl;
        num_errors++;
    }    

    if (test_sim->pc_val() != 0)
    {
        std::cerr << "Pc not intialised at 0" << std::endl;
        num_errors++;
    }

    if (num_errors > 0)
    {
        abort();
    }
    else
    {
        std::cout << "Constructor test passed" << std::endl;
    }
    free(test_sim);
}

void test_fettch()
{
    Simulator *test_sim = new Simulator(16, 256, "fib.prog");
    
    int first_line = test_sim->fetch();

    if (first_line != 0x8031400)
    {
        std::cerr << "first line not being read correctly" << std::endl;
        abort();
    }
    else
    {
        std::cout << "Fetch test passed" << std::endl;
    }

    free(test_sim);
}

void test_decode()
{
    Simulator *test_sim = new Simulator(16, 256, "fib.prog");

    int first_ins = test_sim->fetch();

    Decoded_instruction *test_dec = test_sim->decode(first_ins);

    if (test_dec->get_opcode() == 8 && test_dec->get_operand(0) == 3 && test_dec->get_operand(1) == 20)
    {
        std::cout << "Decode test passed" << std::endl;
    }
    else
    {
        std::cerr << "Decode test failed" << std::endl;
        abort();
    }

    free(test_sim);
}

void test_execute()
{
    Simulator *test_sim = new Simulator(16, 256, "fib.prog");
    
    int first_ins = test_sim->fetch();

    Decoded_instruction *first_ins_dec = test_sim->decode(first_ins);

    int return_val = test_sim->execute(first_ins_dec);

    if (return_val == 20)
    {
        std::cout << "Execute test passed" << std::endl;
    }
    else
    {
        std::cerr << "Execute test failed" << std::endl;
    }

    free(test_sim);
}

void test_write_back()
{
    Simulator *test_sim = new Simulator(16, 256, "fib.prog");

    int first_ins = test_sim->fetch();

    Decoded_instruction *first_ins_dec = test_sim->decode(first_ins);

    int return_val = test_sim->execute(first_ins_dec);

    
}

int main(int argc, char const *argv[])
{
    test_constructor();
    test_fettch();
    test_decode();
    test_execute();
    return 0;
}
