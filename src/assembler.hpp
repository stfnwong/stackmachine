/*
 * ASSEMBLER
 * Forth-style stack machine assembler 
 *
 * Stefan Wong 2020
 */

#ifndef __SM_ASSEMBLER_HPP
#define __SM_ASSEMBLER_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "source.hpp"

const uint16_t constexpr ASM_START_ADDR = 0x0800;   // why not?

// Implement this as a WISC/16 assembler first

/*
 * Instr
 * Represents a single microcoded instruction
 */
struct Instr
{
    uint16_t addr;
    uint32_t microcode;

public:
    Instr();
    Instr(const Instr& that) = default;
    Instr& operator=(const Instr& that) = default;

    void init(void);
    bool operator==(const Instr& that) const;
    bool operator!=(const Instr& that) const;
};

// microcode bit offsets 
constexpr int MO_BUS_SRC  = 0;
constexpr int MO_BUS_DEST = 4;
constexpr int MO_ALU_FUNC = 8;


typedef enum
{
    BUS_DP,     // data stack pointer
    BUS_DS,     // data stack
    BUS_RP,     // return stack pointer
    BUS_RS,     // return stack
    BUS_DLO,    // data low 
    BUS_DHI,    // data high
} ASM_BUS_MODULE;

const uint16_t constexpr ASM_ADDR_INCR = 4;
/*
 * Assembler.
 * Accept a FileInfo structure and produce a binary
 */
class Assembler
{
private:
    bool verbose;
    uint16_t cur_addr;
    std::vector<Instr> program;

    // instruction assembly
private:
    uint8_t  alu_function_sel(void) const;      // lookup ALU function codes

    // functions to turn each instr into microcoded micro-ops
    uint32_t asm_add(const LineInfo& line) const;
    uint32_t asm_and(const LineInfo& line) const;
    uint32_t asm_or(const LineInfo& line) const;
    uint32_t asm_select_bus_src(const LineInfo& line) const;
    uint32_t asm_select_bus_dest(const LineInfo& line) const;


public:
    Assembler();

    int assem(const FileInfo& file_info);
    void setVerbose(void);
    void clearVerbose(void);
};



#endif /*__SM_ASSEMBLER_HPP*/
