#include "branch_unit.hpp"

Branch_Unit::Branch_Unit()
{
    this->current_ins = NULL;
    this->busy = false;
    this->output = (branch_unit_output *) malloc(sizeof(branch_unit_output));
}

Branch_Unit::~Branch_Unit()
{
}

void Branch_Unit::get_ins(rs_output *ins, int current_clock)
{
    if (ins->opcode == 11)
    {
        this->current_ins = ins;
        this->start_clock = current_clock;
        this->done_clock = current_clock+1;
        this->busy = true;
    }
    else
    {
        std::cerr << "Tried to give an non branching function to branch unit" << std::endl;
        std::cerr << "This occured on clock cycle " << current_clock << " with the insturction having opcode " << ins->opcode << std::endl;
        abort();
    }
}

void Branch_Unit::exec()
{
    this->output->issue_num = this->current_ins->dispatch_num;
    //branch occurs
    if (this->current_ins->values->at(0) == 0)
    {
        this->output->branch_taken = true;
        this->output->result = current_ins->values->at(1);
    }
    else
    {
        this->output->branch_taken = false;
        this->output->result = -1;
    }
}

bool Branch_Unit::finished_exec(int current_clock)
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

branch_unit_output *Branch_Unit::get_output(int current_clock)
{
    if (this->finished_exec(current_clock))
    {
        this->busy = false;
        this->start_clock = 0;
        this->done_clock = 0;
        this->output = NULL;
        this->current_ins->values->clear();
        free(this->current_ins);
        return this->output;
    }
    else
    {
        std::cerr << "Tried to get output of branch unit before it had finished executing" << std::endl;
        abort();
    }
}

bool Branch_Unit::is_busy()
{
    return this->busy;
}

void print()
{
    
}