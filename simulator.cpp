#include "simulator.hpp"

Simulator::Simulator(int num_register, int memory_size, std::string program_file)
{
    std::ifstream program_FILE;
    program_FILE.open(program_file, std::ifstream::in);
    //load program in
    while(!program_FILE.eof())
    {
        std::string file_line;
        std::getline(program_FILE, file_line);
        int line_val = atoi(file_line.c_str());
        program.push_back(line_val);
    }

    this->register_file.resize(num_register);
    this->memory.resize(memory_size);
    this->pc = 0;
}

Simulator::~Simulator()
{
}

int Simulator::num_registers()
{
    return (int)this->register_file.size();
}

int Simulator::memory_size()
{
    return (int)this->memory.size();
}

int Simulator::pc_val()
{
    return this->pc;
}

int Simulator::fetch()
{
    if (this->pc >= (int)this->program.size())
    {
        return 0;
    }
    else
    {
        int fetch_val = this->program.at(this->pc);
        this->pc++;
        return fetch_val;
    }
    
}

Decoded_instruction *Simulator::decode(int instruction)
{
    Decoded_instruction *first = new Decoded_instruction(instruction);
    
    return first;
}

int Simulator::execute(Decoded_instruction *instruction)
{
    int result = 0;
    switch (instruction->get_opcode())
    {   
        //add
        case 1:
            result = this->register_file.at(instruction->get_operand(1)) + this->register_file.at(instruction->get_operand(2));
            break;
        //addi
        case 2:
            result = this->register_file.at(instruction->get_operand(1)) + instruction->get_operand(2);
            break;
        //sub
        case 3:
            result = this->register_file.at(instruction->get_operand(1) - this->register_file.at(instruction->get_operand(2)));
            break;
        //mul
        case 4:
            result = this->register_file.at(instruction->get_operand(1) * this->register_file.at(instruction->get_operand(2)));
            break;
        //div
        case 5:
            result = (int) this->register_file.at(instruction->get_operand(1) / this->register_file.at(instruction->get_operand(2)));
            break;
        //cmp
        case 6:
            result = (int) this->register_file.at(instruction->get_operand(1)) - this->register_file.at(instruction->get_operand(2));
            if (result > 0)
            {
                result = 1;
            }
            else if (result < 0)
            {
                result = -1;
            }
            break;
        //mov
        case 7:
            result = this->register_file.at(instruction->get_operand(1));
            break;
        //ldr
        case 8:
            result = this->register_file.at(instruction->get_operand(1));
            break;
        // ldc
        case 9:
            result = instruction->get_operand(1);
            break;
        // sto
        case 10:
            result = this->register_file.at(instruction->get_operand(1));
            break;
        //be
        case 11:
            if (this->register_file.at(instruction->get_operand(0)) == 0)
            {
                this->pc = instruction->get_operand(1);
            }
            break;
        // j
        case 17:
            this->pc = instruction->get_operand(0);
            break;
    }
    return result;
}

void Simulator::write_back(Decoded_instruction *instruction, int exec_res)
{
    if (instruction->get_opcode() <= 7)
    {
        this->register_file.at(instruction->get_operand(0)) = exec_res;
    }
    //ldr
    else if (instruction->get_opcode() == 8)
    {
        this->register_file.at(instruction->get_operand(0)) = this->memory.at(exec_res);
    }
    //ldc
    else if (instruction->get_opcode() == 9)
    {
        this->register_file.at(instruction->get_operand(0)) = exec_res;
    }
    //store instructions
    else if (instruction->get_opcode() == 10)
    {
        this->memory.at(this->register_file.at(instruction->get_operand(0))) = exec_res;
    }
    //instruction->print_instruction();
}

int Simulator::get_register_val(int reg_num)
{
    return this->register_file.at(reg_num);
}

void Simulator::print_register_file()
{
    for (int i = 0; i < this->num_registers(); i++)
    {
        std::cout << "\tr" << i << ": " << this->register_file.at(i) << std::endl;
    }
}

void Simulator::print_memory()
{
    for (int i = 0; i < this->memory_size(); i++)
    {
        std::cout << i << ": " << this->memory.at(i) << " ";
        if (i % 4 == 3)
        {
            std::cout << std::endl;
        } 
    }
}