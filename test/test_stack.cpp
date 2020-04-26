/*
 * TEST_STACK
 * A simple stack for stack machine
 *
 * Stefan Wong 2020
 */

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <cstdint>
#include <iostream>
#include "stack.hpp"


TEST_CASE("test stack init", "[classic]")
{
    Stack<uint32_t> test_stack;

    REQUIRE(test_stack.empty() == true);
    REQUIRE(test_stack.full() == false);
    REQUIRE(test_stack.capacity() == 32);
    REQUIRE(test_stack.size() == 0);
}

TEST_CASE("test stack fill", "[classic]")
{
    Stack<uint32_t> test_stack;

    REQUIRE(test_stack.empty() == true);
    REQUIRE(test_stack.full() == false);
    REQUIRE(test_stack.size() == 0);

    // start putting things into the stack
    for(uint32_t i = 0; i < test_stack.capacity(); ++i)
    {
        test_stack.push(2 * i);
        REQUIRE(test_stack.size() == i+1);
        if(i < test_stack.capacity()-2)     // only check full on last element
            REQUIRE(test_stack.full() == false);
        REQUIRE(test_stack.empty() == false);
    }

    REQUIRE(test_stack.full() == true);
    REQUIRE(test_stack.peek() == (test_stack.capacity() - 1) * 2);

    // adding more stuff will have no effect 
    test_stack.push(0x100);
    REQUIRE(test_stack.full() == true);
    REQUIRE(test_stack.peek() == (test_stack.capacity() - 1) * 2);
    for(int i = 0; i < 8; ++i)
    {
        REQUIRE(test_stack.full() == true);
        test_stack.push(i + 0x101);
        REQUIRE(test_stack.peek() == (test_stack.capacity() - 1) * 2);
        REQUIRE(test_stack.full() == true);
    }
}

TEST_CASE("test stack fill then empty", "[classic]")
{
    Stack<uint32_t> test_stack;

    REQUIRE(test_stack.empty() == true);
    REQUIRE(test_stack.full() == false);
    REQUIRE(test_stack.size() == 0);

    // Fill up the stack 
    for(int i = 0; i < test_stack.capacity(); ++i)
    {
        test_stack.push(i << 1);
    }
    REQUIRE(test_stack.full() == true);

    // Now lets empty the stack and check what comes back 
    for(int i = 0; i < test_stack.capacity(); ++i)
    {
        REQUIRE(test_stack.peek() == ((test_stack.capacity() - i - 1) << 1));
        auto stack_out = test_stack.pop();
        REQUIRE(stack_out == ((test_stack.capacity() - i - 1) << 1));
        REQUIRE(test_stack.full() == false);
        REQUIRE(test_stack.size() == (test_stack.capacity() - i - 1));
    }
    REQUIRE(test_stack.empty() == true);

    // If we try to pop further we just get the same value again 
    // (since there is no null output)
    REQUIRE(test_stack.peek() == 0);
    REQUIRE(test_stack.empty() == true);
    for(int i = 0; i < 64; ++i)
    {
        REQUIRE(test_stack.empty() == true);
        REQUIRE(test_stack.peek() == 0);        // since it turns out that stack[0] = 0
    }
}
