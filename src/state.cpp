/*
 * STATE
 * Stack machine state
 *
 * Stefan Wong 2020
 */

#include <cstring>
#include "state.hpp"


State::State() : pc(0), mar(0), data_stack(SM_STACK_SIZE), return_stack(SM_STACK_SIZE)
{
    this->mem = new uint8_t[SM_MEM_SIZE];
    this->init_mem();
    this->init_reg();
}

State::~State()
{
    delete[] this->mem;
}


State::State(const State& that)
{
    memcpy(this->mem, that.mem, SM_MEM_SIZE);
    for(int r = 0; r < SM_NUM_REG; ++r)
        this->reg[r] = that.reg[r];
    this->pc = that.pc;
    this->mar = that.mar;
    this->data_stack = that.data_stack;
    this->return_stack = that.return_stack;
}

/*
 * State::store()
 */
void State::store(uint32_t addr, uint8_t data)
{
    if(addr < SM_MEM_SIZE)
        this->mem[addr] = data;
}

/*
 * State::load()
 */
uint8_t State::load(uint32_t addr) const
{
    if(addr < SM_MEM_SIZE)
        return this->mem[addr];

    return 0;       // kind of shit... worry about it later
}

/*
 * State::init_mem()
 */
void State::init_mem(void)
{
    for(unsigned int m = 0; m < SM_MEM_SIZE; ++m)
        this->mem[m] = 0;       // probably faster to use memset here...
}

/*
 * State::init_reg()
 */
void State::init_reg(void)
{
    for(int i = 0; i < SM_NUM_REG; ++i)
        this->reg[i] = 0;
}

