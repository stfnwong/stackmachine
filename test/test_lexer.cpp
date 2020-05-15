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
#include "source.hpp"


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
    //REQUIRE(status == 0);     // TODO : status is broken for now anyway

    FileInfo lex_file = test_lexer.getFileInfo();


    // TODO : could also try to make an iterator, except that I hate iterators...
    for(unsigned int l = 0; l < lex_file.size(); ++l)
    {
        LineInfo cur_line = lex_file.get(l);
        std::cout << cur_line.toString() << std::endl;
    }


    REQUIRE(lex_file.size() == 16);         // should be equal to the number of instructions supported by the machine

}
