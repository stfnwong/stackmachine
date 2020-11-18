/*
 * STATE
 * Stack machine state
 *
 * Trying to make something like the generic canonical state machine
 * https://users.ece.cmu.edu/~koopman/stack_computers/sec3_2.html
 * Stefan Wong 2020
 */

#ifndef __STACK_STATE_HPP
#define __STACK_STATE_HPP

#include <cstdint>
#include <memory>
#include "Stack.hpp"

const constexpr int SM_MEM_SIZE = 0x1000;    // lets start with 16k of mem
const constexpr int SM_NUM_REG = 0x4;
const constexpr int SM_STACK_SIZE = 0x20;


struct State
{
    uint16_t pc;
    uint16_t mar;
    uint8_t  reg[SM_NUM_REG];
    Stack<uint8_t> data_stack;
    Stack<uint8_t> return_stack;
    // memory
    uint8_t* mem;       // TODO: change to vector?
    
    public:
        // TODO: remove raw pointers?
        State();
        ~State();
        State(const State& that);

        void next(void);
        void store(uint16_t addr, uint8_t data);
        uint8_t load(uint16_t addr) const;


        void init_mem(void);
        void init_reg(void);
};


#endif /*__STACK_STATE_HPP*/
