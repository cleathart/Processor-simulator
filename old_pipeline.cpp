#include "simulator.hpp"
#include <list>
#include <utility>

bool check_can_exec(Decoded_instruction *ins_check, std::list< std::pair<Decoded_instruction *, int>> exec_output)
{
    //checks alu operation does not have an operand that has value that has not been 
    //writen back yet
    if (ins_check->get_opcode() < 7)
    {
        int operand_1 = ins_check->get_operand(1);
        int operand_2 = ins_check->get_operand(2);
        for (auto itr = exec_output.begin(); itr != exec_output.end(); ++itr)
        {
            if (itr->first->get_opcode() <= 8)
            {
                int write_reg = itr->first->get_operand(0);
                if (write_reg == operand_1 || write_reg == operand_2)
                {
                    return false;
                }
            } 
        }
    }
    else if (ins_check->get_opcode() == 10)
    {
        int operand_0 = ins_check->get_operand(0);
        for (auto itr = exec_output.begin(); itr != exec_output.end(); itr++)
        {
            if ((itr->first->get_opcode()) <= 8)
            {
                int write_reg = itr->first->get_operand(0);
                if (write_reg == operand_0)
                {
                    return false;
                }
            } 
        }
    }

    return true;
}

int main(int argc, char const *argv[])
{
    int num_reg = 16;   //32 bit registers
    int mem_size = 256; // 1kb of memeory
    int clock_tick = 0;
    int ins_executed = 0;

    Simulator *proc_sim = new Simulator(num_reg, mem_size, argv[1]);

    std::list<int> fetch_output;
    std::list<Decoded_instruction *> decode_output;
    std::list< std::pair<Decoded_instruction *, int> > exec_output;

    bool ins_fetch = true;
    bool ins_decode = false;
    bool ins_exec = false;
    bool ins_wb = false;
    
    while((ins_fetch || ins_decode || ins_exec || ins_wb) && clock_tick < 65)
    {
        if (ins_fetch)
        {
            if (fetch_output.size() > 0)
            {
                ins_decode = true;
            }

            std::cout << "pc: " << proc_sim->pc_val() << std::endl;
            int fetched_ins = proc_sim->fetch();
            if (fetched_ins == 0)
            {
                ins_fetch = false;
                ins_decode = true;
            }
            else
            {
                fetch_output.push_back(fetched_ins);
            }
        }

        if (ins_decode)
        {
            if (decode_output.size() > 0)
            {
                ins_exec = true;
            } 

            int ins_to_decode = fetch_output.front();
            fetch_output.pop_front();
            Decoded_instruction *decoded_ins = proc_sim->decode(ins_to_decode);
            decode_output.push_back(decoded_ins);

            if (fetch_output.size() == 0)
            {
                ins_decode = false;
            }
            
        }

        if (ins_exec)
        {
            if (exec_output.size() > 0)
            {
                ins_wb = true;
            }
            else
            {
                ins_wb = false;
            }

            Decoded_instruction *exec_ins = decode_output.front();
            exec_ins->print_instruction();
            
            if (check_can_exec(exec_ins, exec_output))
            {
                std::cout << "instruction above is to be executed: " << std::endl;
                decode_output.pop_front();
                int before_pc = proc_sim->pc_val();
                int exec_res = proc_sim->execute(exec_ins);
                int after_pc = proc_sim->pc_val();
                ins_executed++;
                
                
                //no branch prediction will just drop instructions that are already in the pipeline
                if (before_pc != after_pc)
                {
                    fetch_output.clear();
                    ins_fetch = true;
                    decode_output.clear();
                    ins_decode = false;
                }

                std::pair<Decoded_instruction *, int> exec_pair = std::make_pair(exec_ins, exec_res);

                exec_output.push_back(exec_pair);

                if (decode_output.size() == 0)
                {
                    ins_exec = false;
                }
            }
            else 
            {
                std::cout << "The above instruction will not be executed" << std::endl;
            }     
        }
        

        if (ins_wb && exec_output.size() > 0)
        {
            std::pair<Decoded_instruction *, int> wb_ins = exec_output.front();
            exec_output.pop_front();

            proc_sim->write_back(wb_ins.first, wb_ins.second);
        }
        else
        {
            std::cout << "prior value of wb " << ins_wb << std::endl;
            std::cout << "Gets into here to set wb to false" << std::endl;
            ins_wb = false;
        }

        clock_tick++;

        std::cout << "On clock tick: " << clock_tick << std::endl;
        proc_sim->print_register_file();

        std::cout << "State of f/d/e/w: " << ins_fetch << "/" << ins_decode << "/" << ins_exec << '/' << ins_wb << std::endl;
        std::cout << "Fetch output size: " << fetch_output.size() << std::endl;
        std::cout << "Decode output size: " << decode_output.size() << std::endl;
        std::cout << "Exec output size: " << exec_output.size() << std::endl;

        std::cout << std::endl;
    }

    std::cout << "Opcode of stuck instruction " << (int)exec_output.front().first->get_opcode() << std::endl;

    // proc_sim->print_register_file();

    // std::cout << "Fetch output size: " << fetch_output.size() << std::endl;
    // std::cout << "Decode output size: " << decode_output.size() << std::endl;
    // std::cout << "Exec output size: " << exec_output.size() << std::endl;

    std::cout << "Program has run" << std::endl;
    std::cout << "Clock cycles: " << clock_tick << std::endl;
    std::cout << "Instructions executed: " << ins_executed << std::endl;
    float ipc = (float)ins_executed / (float)clock_tick;
    std::cout << "IPC: " << ipc << std::endl;
}