/*
 * STATE
 * Stack machine state
 *
 * Stefan Wong 2020
 */

#include <cstring>
#include "State.hpp"


/*
 * ================ MEMORY ================ 
 */
Memory::Memory(unsigned int size) : mem_size(size)
{
    this->mem = new uint8_t[this->mem_size];
}

Memory::Memory(const Memory& that)
{
    this->mem_size = that.mem_size;
    this->mem = new uint8_t[this->mem_size];
}

Memory::~Memory()
{
    delete[] this->mem;
}

Memory& Memory::operator=(const Memory& that)
{
    if(this != &that)
    {
        if(this->mem_size < that.mem_size)
        {
            delete[] this->mem;
            this->mem = new uint8_t[that.mem_size];
        }
        for(unsigned int i = 0; i < that.mem_size; ++i)
            this->mem[i] = that.mem[i];
        this->mem_size = that.mem_size;
    }

    return *this;
}

/*
 * Comparisons (slow)
 */
bool Memory::operator==(const Memory& that) const
{
    if(this->mem_size != that.mem_size)
        return false;
    for(unsigned int i = 0; i < this->mem_size; ++i)
    {
        if(this->mem[i] != that.mem[i])
            return false;
    }

    return true;
}

bool Memory::operator!=(const Memory& that) const
{
    return !(*this == that);
}

// Access
uint8_t& Memory::operator[](unsigned int idx)
{
    return this->mem[idx];
}

const uint8_t& Memory::operator[](unsigned int idx) const
{
    return this->mem[idx];
}

/*
 * size()
 */
unsigned int Memory::size(void) const
{
    return this->mem_size;
}

/*
 * clear()
 */
void Memory::clear(void)
{
    for(unsigned int i = 0; i < this->mem_size; ++i)
        this->mem[i] = 0;
}


/*
 * ================ STATE ================ 
 */
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


State& State::operator=(const State& that)
{
    if(this != &that)
    {
        this->pc = that.pc;
        this->mar = that.mar;
    }

    return *this;
}

/*
 * Comparisons.
 * Note that we don't compare the memory contents by default
 */

/*
 * State::store()
 */
void State::store(uint16_t addr, uint8_t data)
{
    if(addr < SM_MEM_SIZE)
        this->mem[addr] = data;
}

/*
 * State::load()
 */
uint8_t State::load(uint16_t addr) const
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

