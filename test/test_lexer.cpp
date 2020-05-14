/*
 * TEST_LEXER
 * Unit tests for Stack Machine Lexer
 *
 * Stefan Wong 2020
 */

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <cstdint>
#include <iostream>
#include "lexer.hpp"



// Not actually that much to test on init
TEST_CASE("lexer init", "[classic]")
{
    Lexer test_lexer;

    REQUIRE(test_lexer.getVerbose() == false);
}



