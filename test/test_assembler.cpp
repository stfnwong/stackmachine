/*
 * TEST_ASSEMBLER
 * Tests for assembler and components
 *
 * Stefan Wong 2020
 */

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <cstdint>
#include <iostream>
#include "assembler.hpp"

// TODO : worth doing basic token and opcode tests?
TEST_CASE("test lexer init", "[classic]")
{
    Lexer test_lexer;
}
