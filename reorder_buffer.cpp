#include "reorder_buffer.hpp"

Reorder_Buffer::Reorder_Buffer(int size, int reg_file_size)
{
    this->size = size;
    this->reg_file_size = reg_file_size;
}

Reorder_Buffer::~Reorder_Buffer()
{
}

void Reorder_Buffer::insert(int ins_num, int write_reg, int opcode)
{
    if (this->full())
    {
        std::cerr << "Tried to insert an elment to reorder buffer when it is full" << std::endl;
        abort();
    }
    else if ((int)write_reg > this->reg_file_size)
    {
        std::cerr << "Tried to insert an element in to reodrder buffer that has a write reg too big" << std::endl;
        abort();
    }
    else
    {
        Reorder_Buffer_Entry *entry = (Reorder_Buffer_Entry*) malloc(sizeof(Reorder_Buffer_Entry));
        entry->write_reg = write_reg;
        entry->done = false;
        entry->opcode = opcode;
        this->table.insert(std::pair<int, Reorder_Buffer_Entry*>(ins_num, entry));
    }
}

Reorder_Buffer_Entry *Reorder_Buffer::read(int ins_num)
{
    return this->table.at(ins_num);
}

void Reorder_Buffer::update(int ins_num, int value)
{
    this->table.at(ins_num)->result = value;
    this->table.at(ins_num)->done = true;
}

std::pair<int,Reorder_Buffer_Entry *>Reorder_Buffer::commit()
{
    if (this->table.begin()->second->done)
    {
        Reorder_Buffer_Entry *commit_entry = this->table.begin()->second;
        std::pair<int,Reorder_Buffer_Entry *> return_val(this->table.begin()->first, commit_entry);
        this->table.erase(this->table.begin());
        return return_val;
    }
    else
    {
        std::pair<int,Reorder_Buffer_Entry *> empty_pair(0,NULL);
        return empty_pair;
    }
}

bool Reorder_Buffer::full()
{
    if (this->size == (int)this->table.size())
    {
        return true;
    }
    else if (this->size > (int)this->table.size())
    {
        return false;
    }
    else
    {
        std::cerr << "Reorder buffer has grown too big" << std::endl;
        abort();
    }
}

bool Reorder_Buffer::is_empty()
{
    if ((int)this->table.size() == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Reorder_Buffer::clear_all()
{
    this->table.clear();
}

void Reorder_Buffer::clear_after(int key_val)
{
    for (auto itr = this->table.begin(); itr != this->table.end(); itr++)
    {
        if ((*itr).first >= key_val)
        {
            this->table.erase((*itr).first);
        }
    }
}

void Reorder_Buffer::remove_branch(int key_val)
{
    if (this->table.at(key_val)->opcode == 11)
    {
        this->table.erase(key_val);
    }
    else
    {
        this->print();
        std::cerr << "Tried removing a non branch isntruction from ROB using remove branch instruction" << std::endl;
        std::cerr << "Key val " << key_val << std::endl; 
        std::cerr << "Opcode " << this->table.at(key_val)->opcode << std::endl;
        abort();
    }
}

void Reorder_Buffer::remove_store(int key_val)
{
    if (this->table.at(key_val)->opcode == 10)
    {
        this->table.erase(key_val);
    }
    else
    {
        this->print();
        std::cerr << "Tried removing a non store isntruction from ROB using remove store functions" << std::endl;
        std::cerr << "Key val " << key_val << std::endl; 
        std::cerr << "Opcode " << this->table.at(key_val)->opcode << std::endl;
        abort();
    }
}

void Reorder_Buffer::print()
{
    std::cout << std::endl << "Current state of the reorder buffer: " << std::endl;

    std::cout << "Number of elements in the ROB; " << this->table.size() << std::endl;

    std::cout << "Write reg\t" << "Value\t" << "Done" << std::endl; 

    for (auto itr = this->table.begin(); itr != this->table.end(); itr++)
    {
        std::cout << (int) (*itr).second->write_reg << "\t\t" << (*itr).second->result << "\t";
        if ((*itr).second->done)
        {
            std::cout << "True";
        }
        else
        {
            std::cout << "False";
        }
        std::cout << std::endl;
    }
}