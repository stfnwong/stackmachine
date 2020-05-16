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

// Expected output when lexing all instruction file
FileInfo get_all_instr_file_fileinfo(void)
{
    FileInfo info;
    LineInfo line;

    // ADD (line 9)
    line.init();
    line.opcode = Opcode(LEX_ADD, "ADD");
    line.addr   = 0x0;          // NOTE: once a start address is selected this may change
    line.line_num = 9;
    info.add(line);

    // STORE $60
    line.init();
    line.opcode = Opcode(LEX_STORE, "STORE");
    line.addr   = 0x2;          
    line.line_num = 10;
    line.literal = 0x60;
    info.add(line);

    // SUB
    line.init();
    line.opcode = Opcode(LEX_SUB, "SUB");
    line.addr   = 0x4;          
    line.line_num = 11;
    info.add(line);

    // RPUSH
    line.init();
    line.opcode = Opcode(LEX_RPUSH, "RPUSH");
    line.addr   = 0x6;          
    line.line_num = 12;
    info.add(line);

    // FETCH $500
    line.init();
    line.opcode = Opcode(LEX_FETCH, "FETCH");
    line.addr   = 0x8;          
    line.line_num = 13;
    line.literal = 0x500;
    info.add(line);

    // AND
    line.init();
    line.opcode = Opcode(LEX_AND, "AND");
    line.addr   = 0xA;          
    line.line_num = 14;
    info.add(line);

    // DROP
    line.init();
    line.opcode = Opcode(LEX_DROP, "DROP");
    line.addr   = 0xC;          
    line.line_num = 15;
    info.add(line);

    // DUP
    line.init();
    line.opcode = Opcode(LEX_DUP, "DUP");
    line.addr   = 0xE;          
    line.line_num = 16;
    info.add(line);

    // OR
    line.init();
    line.opcode = Opcode(LEX_OR, "OR");
    line.addr   = 0x10;          
    line.line_num = 17;
    info.add(line);

    // OVER
    line.init();
    line.opcode = Opcode(LEX_OVER, "OVER");
    line.addr   = 0x12;          
    line.line_num = 18;
    info.add(line);

    // RPOP
    line.init();
    line.opcode = Opcode(LEX_RPOP, "RPOP");
    line.addr   = 0x14;          
    line.line_num = 19;
    info.add(line);

    // SWAP
    line.init();
    line.opcode = Opcode(LEX_SWAP, "SWAP");
    line.addr   = 0x16;          
    line.line_num = 20;
    info.add(line);

    // XOR
    line.init();
    line.opcode = Opcode(LEX_XOR, "XOR");
    line.addr   = 0x18;          
    line.line_num = 21;
    info.add(line);

    // IF
    line.init();
    line.opcode = Opcode(LEX_IF, "IF");
    line.addr   = 0x1A;          
    line.line_num = 22;
    info.add(line);

    // CALL
    line.init();
    line.opcode = Opcode(LEX_CALL, "CALL");
    line.addr   = 0x1C;          
    line.line_num = 23;
    info.add(line);

    // EXIT
    line.init();
    line.opcode = Opcode(LEX_EXIT, "EXIT");
    line.addr   = 0x1E;          
    line.line_num = 24;
    info.add(line);

    // LIT
    line.init();
    line.opcode = Opcode(LEX_LIT, "LIT");
    line.addr   = 0x20;          
    line.line_num = 25;
    info.add(line);

    return info;
}


// ==== ALL INSTRUCTION TEST 
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

    FileInfo out_fileinfo = test_lexer.getFileInfo();

    std::cout << "Found " << out_fileinfo.size() << " instructions in source file " 
        << all_instr_file << std::endl;

    // should be equal to the number of instructions supported by the machine
    // Compare against expected output 
    FileInfo exp_fileinfo = get_all_instr_file_fileinfo();
    REQUIRE(out_fileinfo.size() == exp_fileinfo.size());         

    for(unsigned int l = 0; l < out_fileinfo.size(); ++l)
    {
        LineInfo out_line = out_fileinfo.get(l);
        LineInfo exp_line = exp_fileinfo.get(l);

        if(exp_line != out_line)
        {
            std::cout << "ERROR in line " << std::dec << l << std::endl;
            std::cout << "Expecting  " << std::endl << exp_line.toString() << std::endl;
            std::cout << "Got        " << std::endl << out_line.toString() << std::endl;
            std::cout << "Diff       " << std::endl << out_line.diffString(exp_line) << std::endl;
        }
        REQUIRE(exp_line == out_line);
    }
}
