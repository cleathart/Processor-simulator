#include <iostream>
#include <vector>
#include <utility>

typedef struct {
    int rob_value;
    bool renamed;
} Register_Allocation_Table_Entry;

class Register_Allocation_Table
{
    public:
    Register_Allocation_Table(int size);
    ~Register_Allocation_Table();

    void insert(int reg_num, int rob_val);

    Register_Allocation_Table_Entry *read(int reg_num);

    void clear(int reg_num);

    void print();

    private:

    int size;
    std::vector<Register_Allocation_Table_Entry *> table;
};