#include "decoded_instruction.hpp"

Decoded_instruction::Decoded_instruction(int instruction)
{
    this->opcode = instruction >> 24;
    //alu instructions
    if (this->opcode <= 6)
    {
        this->operands.push_back((instruction >> 16) & 255);
        this->operands.push_back((instruction >> 8) & 255);
        this->operands.push_back((instruction) & 255);
    }
    //load store instructions
    else if (this->opcode <= 8 || this->opcode == 10)
    {
        this->operands.push_back((instruction >> 16) & 255);
        this->operands.push_back((instruction >> 8) & 255);
    }
    //ldc
    else if (this->opcode == 9)
    {
        this->operands.push_back((instruction >> 16) & 255); 
        this->operands.push_back((instruction & 65535));
    }
    //be
    else if (this->opcode == 11)
    {
        this->operands.push_back((instruction >> 16) & 255);
        this->operands.push_back((instruction & 65535));
    }
    //jump instruction
    else if (this->opcode == 17)
    {
        this->operands.push_back((instruction & 16777215));
    }
}

Decoded_instruction::~Decoded_instruction()
{
    this->operands.clear();
}

void Decoded_instruction::set_opcode(int opcode)
{
    this->opcode = opcode;
}

int Decoded_instruction::get_opcode()
{
    return this->opcode;
}

int Decoded_instruction::number_operands()
{
    return this->operands.size();
}

int Decoded_instruction::get_operand(int n)
{
    if (n >= (int)this->operands.size())
    {
        std::cerr << "Tried accesing an operand that does not exist for this decoded instruction" << std::endl;
        std::cerr << "Operand num tried to access " << n << std::endl;
        std::cerr << "Current decoded instruction" << std::endl;
        this->print_instruction();
        abort();
    }
    else 
    {
        return this->operands.at(n);
    }
    
}

void Decoded_instruction::print_instruction()
{
    std::cout << "Opcode: " << (int)this->opcode << std::endl;
    for (int i = 0; i < (int)this->operands.size(); i++)
    {
        std::cout << "Operand " << i << ": " << this->operands.at(i) << std::endl;
    }
}