#pragma once

#include <map>
#include <stdbool.h>
#include <cstdint>
#include <iostream>
#include <stdlib.h>

typedef struct 
{
    int opcode;
    int write_reg;
    int result;
    bool done;
} Reorder_Buffer_Entry;

class Reorder_Buffer 
{
    public:

    //constructor and destructor
    Reorder_Buffer(int size, int reg_file_size);
    ~Reorder_Buffer();

    //inserts element ROB with write reg beign the input value
    void insert(int ins_num, int write_reg, int opcode);
    //update reoder buffer entry
    void update(int ins_num, int value);
    //reads a value from the reorder buffer
    Reorder_Buffer_Entry *read(int ins_num);
    //checks if a value can be commited e.g. it is done and if will return otherwise will return null
    std::pair<int, Reorder_Buffer_Entry *>commit();
    //checks if the ROB is full 
    bool full();
    //clears out the reorder buffer for mispredicted branches and the such like
    void clear_all();
    //clears all entries after a certain key if bad branch taken
    void clear_after(int rob_val);
    //removes branch from reoder buffer 
    void remove_branch(int rob_val);
    //removes store instruction from reodrder buffer
    void remove_store(int rob_val);
    //checks if the reorder buffer is empty
    bool is_empty();
    //prints out the rorder buffers current state
    void print();

    private:
    
    int size;
    int reg_file_size;
    std::map<int,Reorder_Buffer_Entry *> table;
};