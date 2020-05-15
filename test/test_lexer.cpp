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
#include <string>
#include "lexer.hpp"


static std::string all_instr_file = "asm/all_instr.asm";

// Not actually that much to test on init
TEST_CASE("lexer init", "[classic]")
{
    Lexer test_lexer;

    REQUIRE(test_lexer.getVerbose() == false);
}


TEST_CASE("lex all instructions", "[classic]")
{
    int status;
    Lexer test_lexer;

    test_lexer.setVerbose();
    REQUIRE(test_lexer.getVerbose() == true);
    status = test_lexer.read(all_instr_file);
    REQUIRE(status == 0);       // NOTE that this test means nothing for now...

    status = test_lexer.lex();
    REQUIRE(status == 0);
}

