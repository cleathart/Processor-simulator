#include "alu_unit.hpp"

int ALU_Unit::counter = 1;

ALU_Unit::ALU_Unit()
{
    this->current_ins = NULL;
    this->busy = false;
    this->id = counter;
    counter++;
}

ALU_Unit::~ALU_Unit()
{
}

void ALU_Unit::get_ins(rs_output *ins, int current_clock)
{
    if (ins->opcode <= 6)
    {
        this->current_ins = ins;
        this->start_clock = current_clock;
        this->busy = true;
    }
    else
    {
        std::cerr << "Tried to give ALU " << this->id << " and instruction that is not an alu instruction" << std::endl;
        std::cerr << "Instruction opcode " << ins->opcode << " instruction number " << ins->dispatch_num << std::endl;
        abort();
    }
}

void ALU_Unit::exec()
{
    if (this->current_ins != NULL)
    {
        switch (this->current_ins->opcode)
        {
            //add
            case 1:
                this->done_clock = this->start_clock + 1;
                this->result = this->current_ins->values->at(0) + this->current_ins->values->at(1);
                break;
            //addi
            case 2:
                this->done_clock = this->start_clock + 1;
                this->result = this->current_ins->values->at(0) + this->current_ins->values->at(1);
                break;
            //sub
            case 3:
                this->done_clock = this->start_clock + 1;
                this->result = this->current_ins->values->at(0) - this->current_ins->values->at(1);
                break;
            //mul
            case 4:
                this->done_clock = this->start_clock + 5;
                this->result = this->current_ins->values->at(0) * this->current_ins->values->at(1);
                break;
            //div
            case 5:
                this->done_clock = this->start_clock + 15;
                this->result = this->current_ins->values->at(0) / this->current_ins->values->at(1);
                break;
            //cmp
            case 6:
                this->done_clock = this->start_clock + 1;
                int result = this->current_ins->values->at(0) - this->current_ins->values->at(1);
                if (result == 0)
                {
                    this->result = 0;
                }
                else if (result > 0)
                {
                    this->result = 1;
                }
                else
                {
                    this->result = -1;
                }
                break;
        }
    }
}

bool ALU_Unit::finished_exec(int current_clock)
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

alu_output *ALU_Unit::get_output(int current_clock)
{
    if (this->finished_exec(current_clock))
    {
        alu_output *return_val = (alu_output *) malloc(sizeof(alu_output));
        return_val->issue_num = this->current_ins->dispatch_num;
        return_val->result = this->result;
        //this->current_ins->values->clear();
        free(this->current_ins);
        this->busy = false;
        return return_val;
    }
    else
    {
        std::cerr << "Tried to get output of ALU " << this->id << " before exec had finished" << std::endl;
        abort();
    }
}

bool ALU_Unit::is_busy()
{
    return this->busy;
}

void ALU_Unit::print()
{
    std::cout << std::endl << "From ALU " << this->id << std::endl;
    if (this->busy)
    {
        std::cout << "Instruction dispatch # " << this->current_ins->dispatch_num << " Opcode: " << this->current_ins->opcode << " output value: " << this->result << " from ";
        for (int i = 0; i < 2; i++)
        {
            std::cout << this->current_ins->values->at(i) << " ";
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << "---ALU is not currently being used so nothing to show---" << std::endl;
    }
}
