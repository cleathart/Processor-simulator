#include "../register_file.hpp"

int main(int argc, char const *argv[])
{
    Register_File *reg_file = new Register_File(16);

    reg_file->write(2, 4);

    reg_file->print();

    reg_file->write(1,231);

    reg_file->print();

    if (reg_file->read(1) == 231)
    {
        std::cout << "Great sucess! read works" << std::endl; 
    }
    else
    {
        std::cout << "Read is not workign correctly" << std::endl;
    }

    return 0;
}
