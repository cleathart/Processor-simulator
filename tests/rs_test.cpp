#include "../reservation_station.hpp"
#include "../register_allocation_table.hpp"
#include "../register_file.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    Register_File *test_reg_file = new Register_File(16);
    Register_Allocation_Table *test_rat = new Register_Allocation_Table(16, 8);
    Reservation_Station *rs_1 = new Reservation_Station();
    Reservation_Station *rs_2 = new Reservation_Station();

    test_reg_file->write(0,2);
    test_reg_file->write(1,4);
    test_reg_file->write(2,8);
    test_reg_file->write(3,16);

    Decoded_instruction *test_ins = new Decoded_instruction(16909060);

    test_ins->print_instruction();
    std::cout << std::endl;

    std::cout << "RS_1 id: " << rs_1->get_id() << std::endl;
    std::cout << "RS_1 id: " << rs_2->get_id() << std::endl;

    Register_Allocation_Table_Entry *read_rat = test_rat->read(test_ins->get_operand(0));

    std::cout << std::endl << "Reg num: " << read_rat->reg_num << " renamed: " << read_rat->renamed << std::endl;

    if (!rs_1->is_busy())
    {
        rs_1->populate(test_ins);
    }

    rs_1->print();

    return 0;
}
