#include "../register_allocation_table.hpp"

int main(int argc, char const *argv[])
{
    Register_Allocation_Table *rat = new Register_Allocation_Table(16);

    std::cout << "Start of RAT" << std::endl;
    rat->print();

    rat->insert(3, 1);
    std::cout << "After inserting one element" << std::endl;
    rat->print();

    rat->insert(4, 5);
    std::cout << "After inserting another element" << std::endl;
    rat->print();

    rat->clear(3);
    std::cout << "After clearing 3rd value" << std::endl;
    rat->print();

    Register_Allocation_Table_Entry *test_read = rat->read(4);
    std::cout << "Test read reg val: " << test_read->rob_value << " Has been renamed: " << test_read->renamed << std::endl;

    return 0;
}