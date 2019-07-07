#include "OoO_Processor.hpp"

OoO_Processor::OoO_Processor(int reg_num, int mem_size, int num_alus)
{
    this->pc = 0;
    this->clock_tick = 0;
    this->ins_exec = 0;
    this->register_file.resize(reg_num);
    for (int i = 0; i < reg_num; i++)
    {
        register_file.at(i) = i+1;
    }
    this->rat = new Register_Allocation_Table(reg_num);
    this->rob = new Reorder_Buffer(200,reg_num);
    this->instructions_issued = reg_num;
    this->alu_rs.reserve(num_alus *2);
    for (int i = 0; i < num_alus*2; i++)
    {
        Reservation_Station *new_rs = new Reservation_Station();
        this->alu_rs.push_back(new_rs);
    }
    this->branch_rs = new Reservation_Station();
    this->ls_rs.reserve(2);
    for (int i = 0; i < 2; i++)
    {
        Reservation_Station *new_rs = new Reservation_Station();
        this->ls_rs.push_back(new_rs);
    }
    this->alus.reserve(num_alus);
    for (int j = 0; j < num_alus; j++)
    {
        ALU_Unit *new_alu = new ALU_Unit();
        this->alus.push_back(new_alu);
    }
    this->branch = new Branch_Unit();
    this->load_store = new LS_Unit(mem_size);
    this->cdb = NULL;
    this->decode_out = NULL;
}

OoO_Processor::~OoO_Processor()
{
}

void OoO_Processor::load_program(std::string program_file)
{
    std::ifstream program_FILE;
    program_FILE.open(program_file, std::ifstream::in);
    while(!program_FILE.eof())
    {
        std::string file_line;
        std::getline(program_FILE, file_line);
        int line_val = atoi(file_line.c_str());
        if (line_val > 0)
        {
            this->program_file.push_back(line_val);
        }
    }
    program_FILE.close();
    std::cout << "Program file has been loaded in." << std::endl;
}

void OoO_Processor::fetch()
{
    if (this->pc < (int) this->program_file.size())
    {
        int ins = this->program_file.at(this->pc);
        this->instruction_queue.push_back(ins);
        this->pc++;
    }
}

void OoO_Processor::decode()
{
    if ((!this->instruction_queue.empty()) && this->decode_out == NULL)
    {
        int ins_decode = this->instruction_queue.front();
        this->decode_out = new Decoded_instruction(ins_decode);
        //execute J in decode
        if (this->decode_out->get_opcode() == 17)
        {
            this->pc = this->decode_out->get_operand(0);
            this->instruction_queue.clear();
            this->decode_out = NULL;
            this->ins_exec++;
        }
        else
        {
            this->instruction_queue.pop_front();
        }
    }
}

void OoO_Processor::issue()
{
    //get values from registers for ALU RS
    for (auto itr = this->alu_rs.begin(); itr != this->alu_rs.end(); itr++)
    {
        if ((*itr)->is_busy())
        {
            std::map<int,int> op_num_op_reg = (*itr)->ask_data();
            for (auto map_itr = op_num_op_reg.begin(); map_itr != op_num_op_reg.end(); ++map_itr)
            {
                int reg_num = (*map_itr).second;
                //get value from register file
                if (reg_num < (int)this->register_file.size())
                {
                    int value = this->register_file.at(reg_num);
                    (*itr)->get_data((*map_itr).first, value);
                }
                //get value from rob
                else
                {
                    Reorder_Buffer_Entry *rob_e = this->rob->read(reg_num);
                    if (rob_e->done)
                    {
                        int value = rob_e->result;
                        (*itr)->get_data((*map_itr).first, value);
                    }
                }
            }
            op_num_op_reg.clear();
        }
    }
    //get values from register or rob for LS RS
    for (auto itr = this->ls_rs.begin(); itr != this->ls_rs.end(); itr++)
    {
        if ((*itr)->is_busy())
        {
            std::map<int,int> op_num_op_reg = (*itr)->ask_data();
            for (auto map_itr = op_num_op_reg.begin(); map_itr != op_num_op_reg.end(); ++map_itr)
            {
                int reg_num = (*map_itr).second;
                //get value from register file
                if (reg_num < (int) this->register_file.size())
                {
                    int value = this->register_file.at(reg_num);
                    (*itr)->get_data((*map_itr).first, value);
                }
                else
                {
                    Reorder_Buffer_Entry *rob_e = this->rob->read(reg_num);
                    if (rob_e->done)
                    {
                        int value = rob_e->result;
                        (*itr)->get_data((*map_itr).first, value);
                    }
                }
            }
            op_num_op_reg.clear();
        }
    }
    
    //get data for the branch rs
    if (branch_rs->is_busy() && !branch_rs->ready_dispatch())
    {
        std::map<int,int> op_num_op_reg = branch_rs->ask_data();
        int reg_num = op_num_op_reg.begin()->second;
        if (reg_num < (int) this->register_file.size())
        {
            int value = this->register_file.at(reg_num);
            branch_rs->get_data(op_num_op_reg.begin()->first, value);
        }
        else
        {
            Reorder_Buffer_Entry *rob_e = this->rob->read(reg_num);
            if (rob_e->done)
            {
                branch_rs->get_data(op_num_op_reg.begin()->first, rob_e->result);
            }
        }
        op_num_op_reg.clear();
    }
    //insert new instruction in ALU RS
    if (this->decode_out != NULL && this->decode_out->get_opcode() <= 6)
    {
        //if decode has output check for free reservation station
        int rs_free = -1;
        for (int i = 0; i < (int)this->alu_rs.size(); i++)
        {
            if (!alu_rs.at(i)->is_busy())
            {
                rs_free = i;
                break;
            }
        }
        //insert data into reservation station
        if (rs_free != -1)
        {
            alu_rs.at(rs_free)->insert(this->instructions_issued, this->decode_out);
            //collect true reg values via rat for rs 
            std::vector<int> reg_check = alu_rs.at(rs_free)->ask_reg_nums();
            std::vector<int> renamed_reg;
            for (auto itr = reg_check.begin(); itr != reg_check.end(); itr++)
            {
                Register_Allocation_Table_Entry *rat_e = this->rat->read((*itr));
                if (rat_e->renamed)
                {
                    renamed_reg.push_back(rat_e->rob_value);
                }
                else
                {
                    renamed_reg.push_back((*itr));
                }
            }
            alu_rs.at(rs_free)->get_reg_nums(renamed_reg);
            //insert instruction into rob with value dispatch num
            int wb_reg = this->alu_rs.at(rs_free)->get_wb_reg();
            
            this->rob->insert(this->instructions_issued, wb_reg, this->decode_out->get_opcode());
            if (wb_reg >= 0)
            {
                this->rat->insert(wb_reg, this->instructions_issued);
            }
            this->instructions_issued++;
            this->decode_out->~Decoded_instruction();
            this->decode_out = NULL;   
        }
    }
    //insert insturction into ls_rs
    else if (this->decode_out != NULL && this->decode_out->get_opcode() <= 10)
    {
        //find free ls_rs
        int free_rs = -1;
        for (int i = 0; i < (int)this->ls_rs.size(); i++)
        {
            if (!this->ls_rs.at(i)->is_busy())
            {
                free_rs = i;
                break;
            }
        }
        // insert data into RS
        if (free_rs != -1)
        {
            this->ls_rs.at(free_rs)->insert(this->instructions_issued, this->decode_out);
            std::vector<int> reg_check = this->ls_rs.at(free_rs)->ask_reg_nums();
            std::vector<int> renamed_reg;
            //iteratte over all registers to find renamed ones
            for (auto itr = reg_check.begin(); itr != reg_check.end(); itr++)
            {
                Register_Allocation_Table_Entry *rat_e = this->rat->read((*itr));
                if (rat_e->renamed)
                {
                    renamed_reg.push_back(rat_e->rob_value);
                }
                else
                {
                    renamed_reg.push_back((*itr));
                }
            }
            this->ls_rs.at(free_rs)->get_reg_nums(renamed_reg);
            //insert into rob and check if need inserting into rat
            int wb_reg = this->ls_rs.at(free_rs)->get_wb_reg();
            this->rob->insert(this->instructions_issued, wb_reg, this->decode_out->get_opcode());
            //insert into rat
            if (wb_reg >= 0)
            {
                this->rat->insert(wb_reg, this->instructions_issued);
            }
            this->instructions_issued++;
            this->decode_out->~Decoded_instruction();
            this->decode_out = NULL;
        }
    }
    //branch ins
    else if (this->decode_out != NULL && this->decode_out->get_opcode() == 11 && !this->branch_rs->is_busy())
    {
        this->branch_rs->insert(this->instructions_issued,this->decode_out);

        //collect register that is be used as check
        std::vector<int> reg = this->branch_rs->ask_reg_nums();
        std::vector<int> renamed_reg;
        if(this->rat->read(reg.at(0))->renamed)
        {
            renamed_reg.push_back(this->rat->read(reg.at(0))->rob_value);
            this->branch_rs->get_reg_nums(renamed_reg);
        }
        this->rob->insert(this->instructions_issued,-1, this->decode_out->get_opcode());
        this->instructions_issued++;
        this->decode_out->~Decoded_instruction();
        this->decode_out = NULL;
    }
}

void OoO_Processor::dispatch()
{
    //dispatch branch instruction
    if (this->branch_rs->ready_dispatch() && !this->branch->is_busy())
    {
        this->branch->get_ins(this->branch_rs->dispatch_rs(), this->clock_tick);
        this->branch->exec();
        this->ins_exec++;
    }
    
    //dispatch ls instructions
    for (int i = 0; i < (int) this->ls_rs.size(); i++)
    {
        if (this->ls_rs.at(i)->ready_dispatch() && !this->load_store->is_busy())
        {
            rs_output *disp_ins = this->ls_rs.at(i)->dispatch_rs();
            this->load_store->get_ins(disp_ins, this->clock_tick);
            this->load_store->exec();
            this->ins_exec++;
        }
    }

    //check RS is ready to dispatch if so and there is an avaiable exec unit then dispatch instruction to it
    for (int i = 0; i < (int) this->alu_rs.size(); i++)
    {
        if (this->alu_rs.at(i)->ready_dispatch())
        {
            //find exec unit for it and run
            for (int j = 0; j < (int) this->alus.size(); j++)
            {
                if (!this->alus.at(j)->is_busy())
                {
                    rs_output *disp_ins = this->alu_rs.at(i)->dispatch_rs();

                    //fail point
                    this->alus.at(j)->get_ins(disp_ins, this->clock_tick);
                    this->alus.at(j)->exec();
                    this->ins_exec++;
                    break;
                }
            }
        }
    }
}

void OoO_Processor::transmitCDB()
{
    //want to do branch unit first and then check alus
    if (this->branch->finished_exec(this->clock_tick))
    {
        branch_unit_output *branch_res = this->branch->get_output(this->clock_tick);
        if (branch_res->branch_taken)
        {
            this->pc = branch_res->result;
            this->instruction_queue.clear();
            this->decode_out = NULL;
            for (int i = 0; i < (int)this->alu_rs.size(); i++)
            {
                this->alu_rs.at(i)->bad_branch(branch_res->issue_num);
            }
            this->rob->clear_after(branch_res->issue_num);
        }
        else
        {
            this->rob->remove_branch(branch_res->issue_num);
        }
        return;
    }

    //check load store next for transmission
    if (this->load_store->finished_exec(this->clock_tick))
    {
        ls_unit_output *ls_res = this->load_store->get_output(this->clock_tick);
        //mov ldr ldc instructions
        if (this->rob->read(ls_res->issue_num)->opcode <= 9)
        {
            this->cdb = (alu_output *) malloc(sizeof(alu_output));
            this->cdb->issue_num = ls_res->issue_num;
            this->cdb->result = ls_res->result;
            rob->update(this->cdb->issue_num, this->cdb->result);
            //check all alu rs to see if values can be updated
            for (int j = 0; j < (int) this->alu_rs.size(); j++)
            {
                std::map<int,int> op_num_op_regs = this->alu_rs.at(j)->ask_data();
                for (auto map_itr = op_num_op_regs.begin(); map_itr != op_num_op_regs.end(); map_itr++)
                {
                    if ((*map_itr).second == this->cdb->issue_num)
                    {
                        this->alu_rs.at(j)->get_data((*map_itr).first, this->cdb->result);
                    }
                }
                op_num_op_regs.clear();
            }
            //check all ls_rs see if values can be updated
            for (int j = 0; j < (int) this->ls_rs.size(); j++)
            {
                std::map<int,int> op_num_op_regs = this->ls_rs.at(j)->ask_data();
                for (auto map_itr = op_num_op_regs.begin(); map_itr != op_num_op_regs.end(); map_itr++)
                {
                    if ((*map_itr).second == this->cdb->issue_num)
                    {
                        this->ls_rs.at(j)->get_data((*map_itr).first, this->cdb->result);
                    }
                }
                op_num_op_regs.clear();
            }
            //check branch rs
            std::map<int,int> op_num_op_regs = this->branch_rs->ask_data();
            for (auto map_itr = op_num_op_regs.begin(); map_itr != op_num_op_regs.end(); map_itr++)
            {
                if ((*map_itr).second == this->cdb->issue_num)
                {
                    this->branch_rs->get_data((*map_itr).first, this->cdb->result);
                }
            }
            op_num_op_regs.clear();
        }
        //sto instruction do not need to be writeen back and do not need to be trnamistted just removed from rob
        else
        {
            this->rob->remove_store(ls_res->issue_num);
        }
    }

    //check all alus to see in they have results ready
    for(int i = 0; i < (int) this->alus.size(); i++)
    {
        if (this->alus.at(i)->finished_exec(this->clock_tick))
        {
            //get result and put into cdb
            this->cdb = this->alus.at(i)->get_output(this->clock_tick);
            //put cdb result into rob
            rob->update(this->cdb->issue_num, this->cdb->result);
            //check all alu rs to see if values can be updated
            for (int j = 0; j < (int) this->alu_rs.size(); j++)
            {
                std::map<int,int> op_num_op_regs = this->alu_rs.at(j)->ask_data();
                for (auto map_itr = op_num_op_regs.begin(); map_itr != op_num_op_regs.end(); map_itr++)
                {
                    if ((*map_itr).second == this->cdb->issue_num)
                    {
                        this->alu_rs.at(j)->get_data((*map_itr).first, this->cdb->result);
                    }
                }
                op_num_op_regs.clear();
            }
            //check all ls_rs see if values can be updated
            for (int j = 0; j < (int) this->ls_rs.size(); j++)
            {
                std::map<int,int> op_num_op_regs = this->ls_rs.at(j)->ask_data();
                for (auto map_itr = op_num_op_regs.begin(); map_itr != op_num_op_regs.end(); map_itr++)
                {
                    if ((*map_itr).second == this->cdb->issue_num)
                    {
                        this->ls_rs.at(j)->get_data((*map_itr).first, this->cdb->result);
                    }
                }
                op_num_op_regs.clear();
            }
            //check branch rs values can be updated
            std::map<int,int> op_num_op_regs = this->branch_rs->ask_data();
            for (auto map_itr = op_num_op_regs.begin(); map_itr != op_num_op_regs.end(); map_itr++)
            {
                if ((*map_itr).second == this->cdb->issue_num)
                {
                    this->branch_rs->get_data((*map_itr).first, this->cdb->result);
                }
            }
            op_num_op_regs.clear();
            //will only transmit 1 alu at a time
            return;
        }
    }
    //this->cdb = NULL;
}

void OoO_Processor::write_back()
{
    if (!this->rob->is_empty())
    {
        std::pair<int,Reorder_Buffer_Entry*> write_info = this->rob->commit();
        if (write_info.second != NULL)
        {
            //alu write back
            if (write_info.second->opcode <= 9)
            {
                this->register_file.at(write_info.second->write_reg) = write_info.second->result;

                if (this->rat->read(write_info.second->write_reg)->rob_value == write_info.first)
                {
                    this->rat->clear(write_info.second->write_reg);
                }
            }
        }
    }
}

bool OoO_Processor::can_run()
{
    //instruction to still fetch
    if (this->pc < (int) this->program_file.size())
    {
        return true;
    }
    //instruction still excuting or waiting to be written back
    else if (!this->rob->is_empty())
    {
        return true;
    }
    //something in the instruction queue that needs to decoded
    else if (!this->instruction_queue.empty())
    {
        return true;
    }
    //something still on the decoder out latches
    else if (this->decode_out != NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void OoO_Processor::print_mem()
{
    this->load_store->print_memory();
}

void OoO_Processor::print_rf()
{
    std::cout << std::endl <<  "Currnet state of Register file" << std::endl;
    std::cout << "Reg #\t" << "Value" << std::endl;
    for(int i = 0; i < (int) this->register_file.size(); i++)
    {
        std::cout << i << "\t" << this->register_file.at(i) << std::endl;
    }   
    std::cout << std::endl;
}

void OoO_Processor::print_rat()
{
    this->rat->print();
}

void OoO_Processor::print_rob()
{
    this->rob->print();
}

void OoO_Processor::print_decode_out()
{
    if (this->decode_out != NULL)
    {
        std::cout << std::endl << "Current output of the decoder" << std::endl;
        this->decode_out->print_instruction();
    }
    else
    {
        std::cout << std::endl << "---The decoder is currently empty---" << std::endl;
    }   
}

void OoO_Processor::print_alu_rs()
{
    for (auto itr = this->alu_rs.begin(); itr != this->alu_rs.end(); ++itr)
    {
        (*itr)->print();
    }
}

void OoO_Processor::print_branch_rs()
{
    this->branch_rs->print();
}

void OoO_Processor::print_ls_rs()
{
    for (auto itr = this->ls_rs.begin(); itr != this->ls_rs.end(); ++itr)
    {
        (*itr)->print();
    }
}

void OoO_Processor::print_alus()
{
    for (auto itr = this->alus.begin(); itr != this->alus.end(); itr++)
    {
        (*itr)->print();
    }
}

void OoO_Processor::print_CDB()
{
    if (this->cdb != NULL)
    {
        std::cout << std::endl << "Currently on the CDB" << std::endl;
        std::cout << "Instruction #: " << this->cdb->issue_num << " value: " << this->cdb->result << std::endl;
    }
    else
    {
        std::cout << std::endl << "---CDB is empty---" << std::endl;
    }
}

void OoO_Processor::increment_clock()
{
    this->clock_tick++;
}

int OoO_Processor::current_clock()
{
    return this->clock_tick;
}

int OoO_Processor::num_exec()
{
    return this->ins_exec;
}