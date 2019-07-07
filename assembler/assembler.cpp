#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <utility>
#include <stdlib.h>

//gets the labels program counters in assembly programs so can be used for branching and jump instructions
void get_labels(std::map<std::string, int> *labels, std::ifstream *file_in)
{
    int pc = 0;
    std::string line;

    while(!file_in->eof())
    {
        std::getline(*file_in, line);

        while (line.length() == 0 || line[0] == '#')
        {
            std::getline(*file_in, line);
        }

        if (line.find(':') == line.length() - 1)
        {
            int end_label = line.find(' ');
            line.resize(end_label);

            labels->insert(std::make_pair(line, pc));
        }
        pc++;
    }
}

//converts assembly instructions into 32 bit ints with 8 bit starting opcode followed by 16 bit of operands
void get_instructions(std::vector<int> *instructions, std::map<std::string, int> *labels, std::ifstream *file_in)
{
    std::string asm_line;
    int pc;

    while(!file_in->eof())
    {
        int instruction = 0;
        std::getline(*file_in, asm_line);
        
        while (asm_line.length() == 0 || asm_line[0] == '#' || asm_line.find(':') == asm_line.length() - 1)
        {
            std::getline(*file_in, asm_line);
        }

        if (asm_line.compare(0, 4, "add ") == 0)
        {
            int register_1, register_2, register_3;

            instruction = instruction | (1 << 24);
            
            int register_pos = asm_line.find('r');
            register_1 = atoi(&asm_line[register_pos+1]);
            
            register_pos = asm_line.find('r', register_pos+1);
            register_2 = atoi(&asm_line[register_pos+1]);

            register_pos = asm_line.find('r', register_pos+1);
            register_3 = atoi(&asm_line[register_pos+1]);

            instruction = instruction | (register_1 << 16);
            instruction = instruction | (register_2 << 8);
            instruction = instruction | register_3;

            instructions->push_back(instruction);
        }
        else if (asm_line.compare(0, 4, "addi") == 0)
        {
            int register_1, register_2, constant;

            instruction = instruction | (2 << 24);

            int register_pos = asm_line.find('r');
            register_1 = atoi(&asm_line[register_pos+1]);

            register_pos = asm_line.find('r', register_pos+1);
            register_2 = atoi(&asm_line[register_pos+1]);

            register_pos = asm_line.find('c', register_pos+1);
            constant = atoi(&asm_line[register_pos+1]);

            instruction = instruction | (register_1 << 16);
            instruction = instruction | (register_2 << 8);
            instruction = instruction | constant;

            instructions->push_back(instruction);
        }
        else if (asm_line.compare(0, 3, "sub") == 0)
        {
            int register_1, register_2, register_3;

            instruction = instruction | (3 << 24);
            
            int register_pos = asm_line.find('r');
            register_1 = atoi(&asm_line[register_pos+1]);
            
            register_pos = asm_line.find('r', register_pos+1);
            register_2 = atoi(&asm_line[register_pos+1]);

            register_pos = asm_line.find('r', register_pos+1);
            register_3 = atoi(&asm_line[register_pos+1]);

            instruction = instruction | (register_1 << 16);
            instruction = instruction | (register_2 << 8);
            instruction = instruction | register_3;

            instructions->push_back(instruction);
        }
        else if (asm_line.compare(0, 3, "mul") == 0)
        {
            int register_1, register_2, register_3;

            instruction = instruction | (4 << 24);
            
            int register_pos = asm_line.find('r');
            register_1 = atoi(&asm_line[register_pos+1]);
            
            register_pos = asm_line.find('r', register_pos+1);
            register_2 = atoi(&asm_line[register_pos+1]);

            register_pos = asm_line.find('r', register_pos+1);
            register_3 = atoi(&asm_line[register_pos+1]);

            instruction = instruction | (register_1 << 16);
            instruction = instruction | (register_2 << 8);
            instruction = instruction | register_3;

            instructions->push_back(instruction);
        }
        else if (asm_line.compare(0, 3, "div") == 0)
        {
            int register_1, register_2, register_3;

            instruction = instruction | (5 << 24);
            
            int register_pos = asm_line.find('r');
            register_1 = atoi(&asm_line[register_pos+1]);
            
            register_pos = asm_line.find('r', register_pos+1);
            register_2 = atoi(&asm_line[register_pos+1]);

            register_pos = asm_line.find('r', register_pos+1);
            register_3 = atoi(&asm_line[register_pos+1]);

            instruction = instruction | (register_1 << 16);
            instruction = instruction | (register_2 << 8);
            instruction = instruction | register_3;

            instructions->push_back(instruction);
        }
        else if (asm_line.compare(0, 3, "cmp") == 0)
        {
            int register_1, register_2, register_3;

            instruction = instruction | (6 << 24);
            
            int register_pos = asm_line.find('r');
            register_1 = atoi(&asm_line[register_pos+1]);
            
            register_pos = asm_line.find('r', register_pos+1);
            register_2 = atoi(&asm_line[register_pos+1]);

            register_pos = asm_line.find('r', register_pos+1);
            register_3 = atoi(&asm_line[register_pos+1]);

            instruction = instruction | (register_1 << 16);
            instruction = instruction | (register_2 << 8);
            instruction = instruction | register_3;

            instructions->push_back(instruction);
        }
        else if (asm_line.compare(0, 3, "mov") == 0)
        {
            int register_1, register_2;

            instruction = instruction | (7 << 24);
        
            int register_pos = asm_line.find('r', 3);
            register_1 = atoi(&asm_line[register_pos+1]);
            
            register_pos = asm_line.find('r', register_pos+1);
            register_2 = atoi(&asm_line[register_pos+1]);

            instruction = instruction | (register_1 << 16);
            instruction = instruction | (register_2 << 8);

            instructions->push_back(instruction);
        }
        else if (asm_line.compare(0, 3, "ldr") == 0)
        {
            int register_1, register_2;

            instruction = instruction | (8 << 24);

            int first_r = asm_line.find('r');
            int register_pos = asm_line.find('r', first_r+1);
            register_1 = atoi(&asm_line[register_pos+1]);
            
            register_pos = asm_line.find('r', register_pos+1);
            register_2 = atoi(&asm_line[register_pos+1]);

            instruction = instruction | (register_1 << 16);
            instruction = instruction | (register_2 << 8);

            instructions->push_back(instruction);
        }
        else if (asm_line.compare(0, 3, "ldc") == 0)
        {
            int register_1, constant;

            instruction = instruction | (9 << 24);
            
            int register_pos = asm_line.find('r');
            register_1 = atoi(&asm_line[register_pos+1]);
            
            register_pos = asm_line.find('c', register_pos+1);
            constant = atoi(&asm_line[register_pos+1]);

            instruction = instruction | (register_1 << 16);
            instruction = instruction | constant;

            instructions->push_back(instruction);
        }
        else if (asm_line.compare(0, 3, "sto") == 0)
        {
            int register_1, register_2;

            instruction = instruction | (10 << 24);
            
            int register_pos = asm_line.find('r');
            register_1 = atoi(&asm_line[register_pos+1]);
            
            register_pos = asm_line.find('r', register_pos+1);
            register_2 = atoi(&asm_line[register_pos+1]);

            instruction = instruction | (register_1 << 16);
            instruction = instruction | (register_2 << 8);

            instructions->push_back(instruction);
        }
        else if (asm_line.compare(0, 2, "be") == 0)
        {
            int register_1, label_pc;

            instruction = instruction | (11 << 24);

            int register_pos = asm_line.find('r');
            register_1 = atoi(&asm_line[register_pos+1]);

            instruction = instruction | (register_1 << 16);

            register_pos = asm_line.find(' ', register_pos + 1);
            int string_len = asm_line.length();
            std::string label_name = asm_line.substr(register_pos+1, string_len - register_pos +1);
            label_pc = labels->at(label_name);
            label_pc--;

            instruction = instruction | label_pc;

            instructions->push_back(instruction);

        }
        else if (asm_line.compare(0, 1, "j") == 0)
        {
            int label_pc;

            instruction = instruction | (17 << 24);

            std::string label_name = asm_line.substr(2, asm_line.length()-2);
            label_pc = labels->at(label_name);

            instruction = instruction | label_pc;

            instructions->push_back(instruction);
        }
        else if (asm_line.compare(0, 3, "sys") == 0)
        {
            printf("sys command found\n");
        }
        pc++;
    }
}

void write_insturctions(std::ofstream *file_out, std::vector<int> *instructions)
{
    for (std::vector<int>::iterator it = instructions->begin(); it != instructions->end(); ++it)
    {
        *file_out << *it << std::endl;
    }
}

int main(int argc, char const *argv[])
{
    std::string in_file = "assembler/assembly_progs/" + std::string(argv[1]);
    std::string out_file = "assembler/assembled_progs/" + std::string(argv[2]);
    
    std::cout << in_file << std::endl;
    std::cout << out_file << std::endl;
    
    std::ifstream file_in(in_file);
    std::ofstream file_out(out_file);

    std::vector<int> instructions;
    std::map<std::string, int> labels;

    get_labels(&labels, &file_in);

    file_in.seekg(file_in.beg);
    
    get_instructions(&instructions, &labels, &file_in);

    file_in.close();

    write_insturctions(&file_out, &instructions);

    file_out.close();

    std::cout << in_file << " assembled with output into: " << out_file << std::endl;

    return 0;
}
