#include "reservation_station.hpp"

int Reservation_Station::counter = 1;

Reservation_Station::Reservation_Station()
{
    this->rs_id = counter++;
    this->busy = false;
    this->operand_regs.reserve(2);
    this->values.reserve(2);
    this->values_collected.reserve(2);
}

Reservation_Station::~Reservation_Station()
{
}

void Reservation_Station::insert(int rob_val, Decoded_instruction *input)
{
    this->rob_val = rob_val;
    this->opcode = input->get_opcode();
    //alu instructions
    if (this->opcode <= 6)
    {
        this->write_reg = input->get_operand(0);
        this->operand_regs.resize(2);
        this->values.resize(2);
        this->values_collected.resize(2);

        this->operand_regs.at(0) = input->get_operand(1);
        this->operand_regs.at(1) = input->get_operand(2);

        if (this->opcode ==2)
        {
            this->values.at(1) = this->operand_regs.at(1);
            this->values_collected.at(1) = true;
        }
    }
    //mov ldr and ldc instrusctions
    else if (this->opcode <= 9)
    {
        this->write_reg = input->get_operand(0);
        this->operand_regs.resize(1);
        this->values.resize(1);
        this->values_collected.resize(1);

        this->operand_regs.at(0) = input->get_operand(1);
        if (this->opcode == 9)
        {
            this->values.at(0) = this->operand_regs.at(0);
            this->values_collected.at(0) = true;
        }
    }
    else if (this->opcode == 10)
    {
        this->write_reg = -1;
        this->operand_regs.resize(2);
        this->values.resize(2);
        this->values_collected.resize(2);

        this->operand_regs.at(0) = input->get_operand(0);
        this->operand_regs.at(1) = input->get_operand(1);
    }
    //be
    else if (this->opcode == 11)
    {
        this->write_reg = -1;
        this->operand_regs.resize(1);
        this->values.resize(2);
        this->values_collected.resize(2);

        this->operand_regs.at(0) = input->get_operand(0);
        //label
        this->values.at(1) = input->get_operand(1);
        this->values_collected.at(1) = true; 
    }
    //free(input);
    this->busy = true;
}

std::vector<int> Reservation_Station::ask_reg_nums()
{
    std::vector<int> return_vec;

    for (int i = 0; i < (int)this->operand_regs.size(); i++)
    {
        if (!this->values_collected.at(i))
        {
            return_vec.push_back(this->operand_regs.at(i));
        }
    }
    return return_vec;
}

void Reservation_Station::get_reg_nums(std::vector<int> updated_reg_vals)
{
    for (int i = 0; i < (int)updated_reg_vals.size(); i++)
    {
        if (!this->values_collected.at(i))
        {
            this->operand_regs.at(i) = updated_reg_vals.at(i);
        }
    }
}

std::map<int,int> Reservation_Station::ask_data()
{
    std::map<int,int> op_num_op_val;
    if (!this->ready_dispatch())
    {
        for (int i = 0; i < (int)this->operand_regs.size(); i++)
        {
            if(!this->values_collected.at(i))
            {
                std::pair<int,int> new_pair (i, this->operand_regs.at(i));
                op_num_op_val.insert(new_pair);
            }
        }
    }
    return op_num_op_val;
}

void Reservation_Station::get_data(int op_num, int value)
{
    if (!this->values_collected.at(op_num))
    {
        this->values.at(op_num) = value;
        this->values_collected.at(op_num) = true;
    }
    else
    {
        std::cerr << "Tried writing value to RS that already has its value" << std::endl;
        abort();
    }
}

bool Reservation_Station::ready_dispatch()
{
    if (!this->busy)
    {
        return false;
    }
    for (auto itr = this->values_collected.begin(); itr != this->values_collected.end(); itr++)
    {
        if (!(*itr))
        {
            return false;
        }
    }
    return true;
}

rs_output *Reservation_Station::dispatch_rs()
{
    rs_output *return_out = (rs_output *)malloc(sizeof(rs_output) + sizeof(this->values.size()));
    return_out->values->resize(this->values.size());

    return_out->dispatch_num = this->rob_val;
    return_out->opcode = this->opcode;
    for (int i = 0; i < (int)this->values.size(); i++)
    {
        return_out->values->at(i) = this->values.at(i);
    }

    //clear rs
    this->rob_val = 0;
    this->opcode = 0;
    this->write_reg = 0;
    this->operand_regs.clear();
    this->values.clear();
    this->values_collected.clear();
    this->busy = false;

    return return_out;
}

void Reservation_Station::bad_branch(int rob_val)
{
    //if instruction is passed the branch then remove it from rs
    if (rob_val < this->rob_val)
    {
        this->rob_val = 0;
        this->opcode = 0;
        this->write_reg = 0;
        this->operand_regs.clear();
        this->values.clear();
        this->values_collected.clear();
        this->busy = false;
    }
}

bool Reservation_Station::is_busy()
{
    return this->busy;
}

int Reservation_Station::get_wb_reg()
{
    return this->write_reg;
}

int Reservation_Station::get_id()
{
    return this->rs_id;
}

void Reservation_Station::print()
{
    std::cout << std::endl << "Current state of RS: " << this->rs_id << std::endl;
    std::cout << "Dispatch #: " << this->rob_val << std::endl;
    std::cout << "Opcode: " << this->opcode << std::endl;
    std::cout << "Operand values: ";
    for(auto itr = this->operand_regs.begin(); itr != this->operand_regs.end(); ++itr)
    {
        std::cout << (*itr) << " ";
    }
    std::cout << std::endl;
    std::cout << "Values collected: ";
    for(int i = 0; i < (int)this->values.size(); i++)
    {
        if (this->values_collected.at(i))
        {
            std::cout << this->values.at(i) << " ";
        }
        else 
        {
            std::cout << "X ";
        }
    }
    std::cout << std::endl;
}