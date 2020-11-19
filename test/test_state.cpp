/*
 * TEST_STATE
 *
 * Stefan Wong 2020
 */

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream>
#include "State.hpp"

// Tests creation, set and read, and comparison
TEST_CASE("test_memory_init", "state")
{
    Memory mem(0x1000);

    REQUIRE(mem.size() == 0x1000);
    mem.clear();
    for(unsigned int i = 0; i < mem.size(); ++i)
        REQUIRE(mem[i] == 0);

    Memory mem2(0x100);

    REQUIRE(mem != mem2);

    Memory mem3(0x100);
    mem2.clear();
    mem3.clear();

    REQUIRE(mem2 == mem3);
    for(unsigned int i = 0; i < mem3.size(); ++i)
        mem3[i] = i % 256;
    REQUIRE(mem2 != mem3);
}


TEST_CASE("test_state_init", "state")
{
    State test_state;

    // Check defaults are correct
    REQUIRE(test_state.pc == 0);
    REQUIRE(test_state.mar == 0);
    REQUIRE(test_state.mem != nullptr);
    for(int r = 0; r < SM_NUM_REG; ++r)
        REQUIRE(test_state.reg[r] == 0);
    // check that the stacks are correct as well
    REQUIRE(test_state.data_stack.capacity() == SM_STACK_SIZE);
    REQUIRE(test_state.return_stack.capacity() == SM_STACK_SIZE);
    REQUIRE(test_state.data_stack.size() == 0);
    REQUIRE(test_state.return_stack.size() == 0);
}
