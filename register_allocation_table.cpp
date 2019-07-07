#include "register_allocation_table.hpp"

Register_Allocation_Table::Register_Allocation_Table(int size)
{
    this->size = size;

    for (int i = 0; i < size; i++)
    {
        Register_Allocation_Table_Entry *new_entry = (Register_Allocation_Table_Entry *) malloc(sizeof(Register_Allocation_Table_Entry));
        new_entry->rob_value = -1;
        new_entry->renamed = false;
        this->table.push_back(new_entry);
    }
}

Register_Allocation_Table::~Register_Allocation_Table()
{
}

void Register_Allocation_Table::insert(int reg_num, int rob_val)
{
    if (reg_num >= this->size)
    {
        std::cerr << "Tried to insert into register allocation table past the end" << std::endl;
        std::cerr << "Value tried to insert at: " << reg_num << std::endl;
        abort();
    }
    else
    {
        this->table.at(reg_num)->rob_value = rob_val;
        this->table.at(reg_num)->renamed = true;
    }
}

Register_Allocation_Table_Entry *Register_Allocation_Table::read(int reg_num)
{
    if (reg_num >= this->size)
    {
        std::cerr << "Tried reading value form register allocation table that was passed end" << std::endl;
        abort();
    }
    else
    {
        return this->table.at(reg_num);
    }
    
}

void Register_Allocation_Table::clear(int reg_num)
{
    if (reg_num >= this->size)
    {
        std::cerr << "Tried to clear entry in register allocation table past end" << std::endl;
        abort();
    }
    else if (!this->table.at(reg_num)->renamed)
    {
        std::cerr << "Tried clearing entry in register allocation table that is already clear" << std::endl;
    }
    else
    {
        this->table.at(reg_num)->rob_value = -1;
        this->table.at(reg_num)->renamed = false;
    }
}

void Register_Allocation_Table::print()
{
    std::cout << "Current state of RAT:" << std::endl << std::endl;
    std::cout << "Renamed Reg Value \tHas been renamed" << std::endl;

    for (int i = 0; i < this->size; i++)
    {
        std::cout << this->table.at(i)->rob_value << "\t\t\t";
        if (this->table.at(i)->renamed)
        {
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "False" << std::endl;
        }
    }
}
