/*
 * OPCODE
 * Forth/Stack machine opcodes
 *
 * Stefan Wong 2020
 */

#include <iomanip>
#include <sstream>
#include "opcode.hpp"

// ================ OPCODE ================ //
Opcode::Opcode() : instr(0), mnemonic("") {} 

Opcode::Opcode(uint32_t i, const std::string& m) : instr(i), mnemonic(m) {} 

bool Opcode::operator==(const Opcode& that) const
{
    if(this->mnemonic != that.mnemonic)
        return false;
    if(this->instr != that.instr)
        return false;
    
    return true;
}

bool Opcode::operator!=(const Opcode& that) const
{
    return !(*this == that);
}

void Opcode::init(void)
{
    this->mnemonic = "";
    this->instr = 0;
}

std::string Opcode::toString(void) const
{
    std::ostringstream oss;

    oss << "[" << this->mnemonic << "] ";
    oss << "0x" << std::hex << std::setfill('0')  << std::setw(4) << this->instr;

    return oss.str();
}

