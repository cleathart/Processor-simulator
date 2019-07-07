#include "../reorder_buffer.hpp"

int main(int argc, char const *argv[])
{
    Reorder_Buffer *test_rob = new Reorder_Buffer(12,16);

    std::cout << "State of ROB after intialiation" << std::endl;
    test_rob->print();
    
    test_rob->insert(1);
    std::cout << "Stage of ROB after insertion of one element" << std::endl;
    test_rob->print();

    return 0;
}
