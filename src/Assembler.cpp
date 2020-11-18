/*
 * ASSEMBLER
 * Forth-style stack machine assembler 
 *
 * Stefan Wong 2020
 */

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Assembler.hpp"

// ======== INSTRUCTION ======== //
Instr::Instr() : addr(0), microcode(0) {} 

void Instr::init(void)
{
    this->addr = 0;
    this->microcode = 0;
}

bool Instr::operator==(const Instr& that) const
{
    if(this->addr != that.addr)
        return false;
    if(this->microcode != that.microcode)
        return false;

    return true;
}

bool Instr::operator!=(const Instr& that) const
{
    return !(*this == that);
}



// ======== ASSEMBLER ======== //
Assembler::Assembler() : verbose(false) {} 



// ======== INSTRUCTION ASSEMBLY ======== //
/*
 * Assembler::asm_add()
 */
uint32_t Assembler::asm_add(const LineInfo& line) const
{
    uint32_t instr = 0;

    instr = instr | (0x09) << MO_ALU_FUNC;  // A + B + Cin

    return instr;
}

/*
 * Assembler::asm_and()
 */
uint32_t Assembler::asm_and(const LineInfo& line) const
{
    uint32_t instr = 0;

    instr = instr | (0x1B) << MO_ALU_FUNC;  // N1 AND N2

    return instr;
}


/*
 * Assembler::asm_or()
 */
uint32_t Assembler::asm_or(const LineInfo& line) const
{
    uint32_t instr = 0;

    instr = instr | (0x1E << MO_ALU_FUNC);  // N1 OR N2

    return instr;
}

/*
 * Assembler::assem()
 */
int Assembler::assem(const FileInfo& file_info)
{
    int status = 0;

    if(this->verbose)
        std::cout << "[" << __func__ << "] assembling " << file_info.size() 
            << " lines" << std::endl;

    this->cur_addr = ASM_START_ADDR;        // TODO : figure out how much mem and what the mem map is

    // 
    Instr instr;
    LineInfo line;
    for(unsigned int i = 0; i < file_info.size(); ++i)
    {
        instr.init();
        line = file_info.get(i);

        switch(line.opcode.instr)
        {
            case LEX_ADD:
                instr.microcode = this->asm_add(line);
                break;
            case LEX_AND:
                instr.microcode = this->asm_and(line);
                break;

            default:
                status = 0;
                goto ASSEM_END;
        }

        instr.addr = this->cur_addr;
        this->program.push_back(instr);
        this->cur_addr += ASM_ADDR_INCR;

    }

ASSEM_END:
    if(this->verbose && status < 0)
    {
        std::cout << "[" << __func__ << "] failed to assemble for some reason..." << std::endl;
    }

    return status;
}



/*
 * Assembler::setVerbose()
 */
void Assembler::setVerbose(void)
{
    this->verbose = true;
}
/*
 * Assembler::clearVerbose()
 */
void Assembler::clearVerbose(void)
{
    this->verbose = false;
}
