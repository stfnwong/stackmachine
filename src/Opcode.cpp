/*
 * OPCODE
 * Forth/Stack machine opcodes
 *
 * Stefan Wong 2020
 */

#include <iomanip>
#include <sstream>
#include "Opcode.hpp"

// ================ OPCODE ================ //
Opcode::Opcode() : instr(0), mnemonic("") {} 

Opcode::Opcode(uint32_t i, const std::string& m) : instr(i), mnemonic(m) {} 

Opcode::Opcode(const Opcode& that)
{
    this->instr    = that.instr;
    this->mnemonic = that.mnemonic;
}

/*
 * Opcode:==
 */
bool Opcode::operator==(const Opcode& that) const
{
    if(this->mnemonic != that.mnemonic)
        return false;
    if(this->instr != that.instr)
        return false;
    
    return true;
}

/*
 * Opcode::!=
 */
bool Opcode::operator!=(const Opcode& that) const
{
    return !(*this == that);
}

/*
 * Opcode::init()
 */
void Opcode::init(void)
{
    this->mnemonic = "";
    this->instr = 0;
}

/*
 * Opcode::toString()
 */
std::string Opcode::toString(void) const
{
    std::ostringstream oss;

    oss << "[" << this->mnemonic << "] ";
    oss << "0x" << std::hex << std::setfill('0')  << std::setw(4) << this->instr;

    return oss.str();
}

// ================ OPCODE TABLE ================ //
OpcodeTable::OpcodeTable() {}

/*
 * OpcodeTable::add()
 */
void OpcodeTable::add(const Opcode& op)
{
    this->table.push_back(op);
}

/*
 * OpcodeTable::get()
 */
Opcode OpcodeTable::get(const Opcode& op)
{
    for(unsigned int i = 0; i < this->table.size(); ++i)
    {
        if(op == this->table[i])
            return Opcode(this->table[i]);
    }

    return Opcode();
}
/*
 * OpcodeTable::getIdx()
 */
Opcode OpcodeTable::getIdx(unsigned int idx)
{
    if(idx < this->table.size())
        return Opcode(this->table[idx]);

    return Opcode();
}

/*
 * OpcodeTable::getName()
 */
Opcode OpcodeTable::getName(const std::string& name)
{
    for(unsigned int i = 0; i < this->table.size(); ++i)
    {
        if(this->table[i].mnemonic == name)
            return Opcode(this->table[i]);
    }

    return Opcode();
}

/*
 * OpcodeTable::size()
 */
unsigned int OpcodeTable::size(void) const
{
    return this->table.size();
}
