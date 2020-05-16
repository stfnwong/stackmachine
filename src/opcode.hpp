/*
 * OPCODE
 * Forth/Stack machine opcodes
 *
 * Stefan Wong 2020
 */


#ifndef __SM_OPCODE_HPP
#define __SM_OPCODE_HPP

#include <cstdint>
#include <string>
#include <vector>


// Symbols accepted by lexer
typedef enum 
{
    LEX_NULL,
    LEX_ADD,
    LEX_AND,
    LEX_CALL,
    LEX_DROP,
    LEX_DUP,
    LEX_EXIT,
    LEX_FETCH,
    LEX_IF,
    LEX_LIT,
    LEX_OR,
    LEX_OVER,
    LEX_RPUSH,
    LEX_RPOP,
    LEX_STORE,
    LEX_SWAP,
    LEX_SUB,
    LEX_XOR,
} LEX_INSTR_CODES;


/*
 * Opcode
 */
struct Opcode
{
    uint32_t instr;
    std::string mnemonic;

public:
    Opcode();
    Opcode(uint32_t i, const std::string& m);
    Opcode(const Opcode& that);

    Opcode& operator=(const Opcode& that) = default;
    //Opcode& operator=(const Opcode& that);
    bool operator==(const Opcode& that) const;
    bool operator!=(const Opcode& that) const;

    void init(void);
    std::string toString(void) const;
};

// List of opcodes accepted by assembler 
const Opcode lex_opcodes[] = {
    Opcode(LEX_NULL, ""),
    Opcode(LEX_ADD, "ADD"),
    Opcode(LEX_AND, "AND"),
    Opcode(LEX_CALL, "CALL"),
    Opcode(LEX_DROP, "DROP"),
    Opcode(LEX_DUP, "DUP"),
    Opcode(LEX_EXIT, "EXIT"),
    Opcode(LEX_FETCH, "FETCH"),  // NOTE: Forth uses the symbol '@' for this 
    Opcode(LEX_IF, "IF"),
    Opcode(LEX_LIT, "LIT"),
    Opcode(LEX_OR, "OR"),
    Opcode(LEX_OVER, "OVER"),
    Opcode(LEX_RPUSH, "RPUSH"),
    Opcode(LEX_RPOP, "RPOP"),
    Opcode(LEX_STORE, "STORE"),  // NOTE: Forth uses the symbol '!' for this 
    Opcode(LEX_SWAP, "SWAP"),
    Opcode(LEX_SUB, "SUB"),
    Opcode(LEX_XOR, "XOR"),
};


/*
 * OpcodeTable
 */
struct OpcodeTable
{
    std::vector<Opcode> table;

public:
    OpcodeTable();
   
    OpcodeTable& operator=(const OpcodeTable& that) = default;

    void init(void);
    void add(const Opcode& op);
    Opcode get(const Opcode& op);
    Opcode getIdx(unsigned int idx);
    Opcode getName(const std::string& name);

    unsigned int size(void) const;
};


#endif /*__SM_OPCODE_HPP*/
