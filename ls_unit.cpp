#include "ls_unit.hpp"

LS_Unit::LS_Unit(int mem_size)
{
    this->memory.resize(mem_size);
    this->busy = false;
    this->current_ins = NULL;
    this->start_clock = 0;
    this->done_clock = 0;
    this->result = 0;
}

LS_Unit::~LS_Unit()
{
}

void LS_Unit::get_ins(rs_output *ins, int current_clock)
{
    if (ins->opcode > 6 && ins->opcode <=10)
    {
        this->busy = true;
        this->current_ins = ins;
        this->start_clock = current_clock;
    }
    else
    {
        std::cerr << "Tried putting a non load store instruction into a load store unit" << std::endl;
        abort();
    }
}

void LS_Unit::exec()
{
    switch (this->current_ins->opcode)
    {
        //mov
        case 7:
            this->done_clock = this->start_clock+1;
            this->result = this->current_ins->values->at(0);
            break;
        //ldr
        case 8:
            this->done_clock = this->start_clock+5;
            this->result = this->memory.at(this->current_ins->values->at(0));
            break;
        //ldc
        case 9:
            this->done_clock = this->start_clock+1;
            this->result = this->current_ins->values->at(0);
            break;
        //sto
        case 10:
            this->done_clock = this->start_clock+5;
            this->memory.at(this->current_ins->values->at(0)) = this->current_ins->values->at(1);
            break;
    }
}

bool LS_Unit::finished_exec(int current_clock)
{
    if (current_clock >= this->done_clock && this->busy)
    {
        return true;
    }
    else
    {
        return false;
    }
}

ls_unit_output *LS_Unit::get_output(int current_clock)
{
    if(this->finished_exec(current_clock))
    {
        ls_unit_output *return_val = (ls_unit_output *) malloc(sizeof(ls_unit_output));
        return_val->issue_num = this->current_ins->dispatch_num;
        return_val->result = this->result;

        this->current_ins->values->clear();
        free(this->current_ins);
        this->busy = false;
        return return_val;
    }
    else
    {
        std::cerr << "Tried getting output of LS unit before instruction had finished executing" << std::endl;
        abort();
    }
}

void LS_Unit::print_memory()
{
    for (int i = 0; i < (int) this->memory.size(); i++)
    {
        std::cout << i << ": " << this->memory.at(i) << " ";
        if (i % 8 == 7)
        {
            std::cout << std::endl;
        } 
    }
}

bool LS_Unit::is_busy()
{
    return this->busy;
}

