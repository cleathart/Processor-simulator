#include "simulator.hpp"
#include <list>
#include <map>
#include <utility>

bool check_can_exec(Decoded_instruction *ins_check, std::map<int,std::pair<Decoded_instruction *, int>> exec_output)
{
    //checks alu operation does not have an operand that has value that has not been 
    //writen back yet
    if (ins_check->get_opcode() < 7)
    {
        int operand_1 = ins_check->get_operand(1);
        int operand_2 = ins_check->get_operand(2);
        for (auto itr = exec_output.begin(); itr != exec_output.end(); ++itr)
        {
            if (itr->second.first->get_opcode() <= 8)
            {
                int write_reg = itr->second.first->get_operand(0);
                if (write_reg == operand_1 || write_reg == operand_2)
                {
                    return false;
                }
            } 
        }
    }
    else if (ins_check->get_opcode() == 11)
    {
        int operand_0 = ins_check->get_operand(0);
        for (auto itr = exec_output.begin(); itr != exec_output.end(); itr++)
        {
            if ((itr->second.first->get_opcode()) <= 8)
            {
                int write_reg = itr->second.first->get_operand(0);
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
    int bad_branches = 0;

    Simulator *proc_sim = new Simulator(num_reg, mem_size, argv[1]);

    std::map<int,int> fetch_output;
    std::map<int,Decoded_instruction *> decode_output;
    std::map<int,std::pair<Decoded_instruction *, int>> exec_output;
    Decoded_instruction *stall_buffer = NULL;

    bool run_fetch = true;
    bool not_halt = true;

    while(run_fetch || fetch_output.size() > 0 || decode_output.size() > 0 || exec_output.size() > 0)
    {
        if (run_fetch && not_halt)
        {
            int ins_fetched = proc_sim->fetch();

            if (ins_fetched == 0)
            {
                run_fetch = false;
                
            }
            else
            {
                fetch_output[clock_tick+1] = ins_fetched;
            }

        }

        if (fetch_output.find(clock_tick) != fetch_output.end() && not_halt)
        {
            Decoded_instruction *ins_decoded = proc_sim->decode(fetch_output.at(clock_tick));

            fetch_output.erase(clock_tick);

            decode_output[clock_tick+1] = ins_decoded;

        }

        if (decode_output.find(clock_tick) != decode_output.end() && not_halt)
        {
            Decoded_instruction *ins_execute = decode_output.at(clock_tick);

            //ins_execute->print_instruction();

            decode_output.erase(clock_tick);

            //check can execute
            if (check_can_exec(ins_execute, exec_output))
            {
                not_halt = true;
                //execute if fine
                int pc_before = proc_sim->pc_val();
                int exec_result = proc_sim->execute(ins_execute);
                int pc_after = proc_sim->pc_val();

                ins_executed++;
                
                std::pair<Decoded_instruction *, int> exec_pair = std::make_pair(ins_execute, exec_result);

                exec_output[clock_tick+1] = exec_pair;

                //if branch clear pipeline and make sure fetch still occurs
                if (pc_before != pc_after)
                {
                    fetch_output.clear();
                    run_fetch = true;
                    decode_output.clear();
                    bad_branches++;
                }
            }
            else
            {
                not_halt = false;
                stall_buffer = ins_execute;
            }
            //put in map if all is good
        }
        else if(!not_halt)
        {
            //stall_buffer->print_instruction();
            if(check_can_exec(stall_buffer, exec_output))
            {   
                not_halt = true;
                int pc_before = proc_sim->pc_val();
                int exec_result = proc_sim->execute(stall_buffer);
                int pc_after = proc_sim->pc_val();

                ins_executed++;

                std::pair<Decoded_instruction *, int> exec_pair = std::make_pair(stall_buffer, exec_result);

                exec_output[clock_tick+1] = exec_pair;

                //if branch clear pipeline and make sure fetch still occurs
                if (pc_before != pc_after)
                {
                    fetch_output.clear();
                    run_fetch = true;
                    decode_output.clear();
                    bad_branches++;
                }

                stall_buffer = NULL;
            }

            //change key values of outputs of fetch and decode
            for (auto rit = fetch_output.rbegin(); rit != fetch_output.rend(); rit++)
            {
                int ins = rit->second;
                fetch_output[rit->first+1] = ins;
            }
            auto f_itr = fetch_output.begin();
            fetch_output.erase(f_itr->first);
            for (auto rit = decode_output.rbegin(); rit != decode_output.rend(); rit++)
            {
                Decoded_instruction *ins = rit->second;
                decode_output[rit->first+1] = ins;
            }
            auto d_itr = decode_output.begin();
            decode_output.erase(d_itr->first);
        }

        if (exec_output.find(clock_tick) != exec_output.end())
        {
            std::pair<Decoded_instruction *, int> ins_wb = exec_output.at(clock_tick);
            exec_output.erase(clock_tick);
            proc_sim->write_back(ins_wb.first, ins_wb.second);
        }

        // std::cout << "Clock tick: " << clock_tick << std::endl;

        // proc_sim->print_register_file();
        // proc_sim->print_memory();

        // if (clock_tick == 10)
        // {
        //     break;
        // }

        clock_tick++;
    }

    proc_sim->print_memory();

    std::cout << "Program has run" << std::endl;
    std::cout << "Clock cycles: " << clock_tick << std::endl;
    std::cout << "Instructions executed: " << ins_executed << std::endl;
    std::cout << "Bad Branches: " << bad_branches << std::endl;
    float ipc = (float)ins_executed / (float)clock_tick;
    std::cout << "IPC: " << ipc << std::endl;
}