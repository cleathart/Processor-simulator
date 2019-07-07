CXX = g++
CXXFLAGS = -std=c++11 -Wall -Werror -pedantic -g

simple: simple.cpp simulator.cpp simulator.hpp decoded_instruction.cpp decoded_instruction.hpp 
	$(CXX) $(CXXFLAGS) -o simulators/simple simple.cpp simulator.cpp decoded_instruction.cpp

pipeline: pipeline.cpp simulator.cpp simulator.hpp decoded_instruction.cpp decoded_instruction.hpp
	$(CXX) $(CXXFLAGS) -o simulators/pipeline pipeline.cpp simulator.cpp decoded_instruction.cpp
	
test: simulator.cpp simulator.hpp simple_test.cpp decoded_instruction.cpp decoded_instruction.hpp
	$(CXX) $(CXXFLAGS) -o simulators/test simulator.cpp simple_test.cpp decoded_instruction.cpp

assembler: assembler/assembler.cpp
	$(CXX) $(CXXFLAGS) -o assembler/assembler assembler/assembler.cpp

decoded_instruction: decoded_instruction.cpp decoded_instruction.hpp
	$(CXX) $(CXXFLAGS) -o bin/decoded_instruction.o -c decoded_instruction.cpp

reorder_buffer: reorder_buffer.cpp reorder_buffer.hpp
	$(CXX) $(CXXFLAGS) -o bin/reorder_buffer.o -c reorder_buffer.cpp

reorder_buffer_test: tests/rob_test.cpp bin/reorder_buffer.o
	$(CXX) $(CXXFLAGS) -o bin/reorder_buffer_test tests/rob_test.cpp bin/reorder_buffer.o

register_allocation_table: register_allocation_table.cpp register_allocation_table.hpp
	$(CXX) $(CXXFLAGS) -o bin/register_allocation_table.o -c register_allocation_table.cpp

register_allocation_table_test: tests/rat_test.cpp bin/register_allocation_table.o 
	$(CXX) $(CXXFLAGS) -o bin/register_allocation_table_test tests/rat_test.cpp bin/register_allocation_table.o

register_file: register_file.cpp register_file.hpp
	$(CXX) $(CXXFLAGS) -o bin/register_file.o -c register_file.cpp
 
register_file_test: tests/reg_file_test.cpp bin/register_file.o
	$(CXX) $(CXXFLAGS) -o bin/register_file_test tests/reg_file_test.cpp bin/register_file.o

reservation_station: reservation_station.cpp reservation_station.hpp 
	$(CXX) $(CXXFLAGS) -o bin/reservation_station.o -c reservation_station.cpp

reservation_station_test: tests/rs_test.cpp bin/reservation_station.o bin/register_file.o bin/register_allocation_table.o bin/reorder_buffer.o bin/decoded_instruction.o
	$(CXX) $(CXXFLAGS) -o bin/rs_test tests/rs_test.cpp bin/reservation_station.o bin/register_file.o bin/register_allocation_table.o bin/reorder_buffer.o bin/decoded_instruction.o

alu_unit: alu_unit.cpp alu_unit.hpp
	$(CXX) $(CXXFLAGS) -o bin/alu_unit.o -c alu_unit.cpp

branch_unit: branch_unit.cpp branch_unit.hpp
	$(CXX) $(CXXFLAGS) -o bin/branch_unit.o -c branch_unit.cpp

ls_unit: ls_unit.cpp ls_unit.hpp
	$(CXX) $(CXXFLAGS) -o bin/ls_unit.o -c ls_unit.cpp

OoO_Processor: OoO_Processor.cpp OoO_Processor.hpp
	$(CXX) $(CXXFLAGS) -o bin/OoO_Processor.o -c OoO_Processor.cpp

OoO_Processor_test: tests/OoO_proc_test.cpp bin/OoO_Processor.o bin/decoded_instruction.o bin/reservation_station.o bin/register_allocation_table.o bin/reorder_buffer.o bin/alu_unit.o bin/branch_unit.o bin/ls_unit.o
	$(CXX) $(CXXFLAGS) -o bin/OoO_test tests/OoO_proc_test.cpp bin/OoO_Processor.o bin/decoded_instruction.o bin/reservation_station.o bin/register_allocation_table.o bin/reorder_buffer.o bin/alu_unit.o bin/branch_unit.o bin/ls_unit.o