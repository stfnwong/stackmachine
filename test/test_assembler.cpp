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
#include "Lexer.hpp"
#include "Assembler.hpp"
#include "Source.hpp"

static std::string all_instr_file = "asm/all_instr.asm";
static std::string test_label_file = "asm/test_label.asm";

// TODO : worth doing basic token and opcode tests?
//TEST_CASE("test lexer init", "[classic]")
//{
//    Lexer test_lexer;
//    Assembler test_assembler;
//}

// TODO : what is the format for the source output?

FileInfo get_all_instr_file_info(void)
{
    FileInfo file_info;
    LineInfo line;

    // TODO : until memory map is sorted, addresses are just zero
    // ADD
    line.init();
    line.line_num = 9;
    line.addr = 0;       // fix later


    return file_info;
}


TEST_CASE("test assemble all instrs", "[classic]")
{
    int status;
    Lexer test_lexer;
    Assembler test_assembler;

    status = test_lexer.read(all_instr_file);
    status = test_lexer.lex();
    REQUIRE(status == 0);     

    FileInfo out_fileinfo = test_lexer.getFileInfo();

    REQUIRE(out_fileinfo.size() == 17);
    test_assembler.setVerbose();
    status = test_assembler.assem(out_fileinfo);

    REQUIRE(status == 0);
}
