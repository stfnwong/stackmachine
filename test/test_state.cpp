/*
 * TEST_STATE
 *
 * Stefan Wong 2020
 */

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream>
#include "State.hpp"

TEST_CASE("test state init", "[classic]")
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
