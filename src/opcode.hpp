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

// Symbols accepted by lexer
typedef enum 
{
    LEX_NULL,
    LEX_AND,
    LEX_DROP,
    LEX_DUP,
    LEX_FETCH,
    LEX_IF,
    LEX_OR,
    LEX_OVER,
    LEX_STORE,
    LEX_SWAP,
    LEX_XOR,
} lex_instr_codes;


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

    //Opcode& operator=(const Opcode& that);
    bool operator==(const Opcode& that) const;
    bool operator!=(const Opcode& that) const;

    void init(void);
    std::string toString(void) const;
};

// List of opcodes accepted by assembler 
const Opcode lex_opcodes[] = {
    Opcode(LEX_NULL, ""),
    Opcode(LEX_AND, "and"),
    Opcode(LEX_DROP, "drop"),
    Opcode(LEX_DUP, "dup"),
    Opcode(LEX_FETCH, "fetch"),  // NOTE: Forth uses the symbol '@' for this 
    Opcode(LEX_IF, "if"),
    Opcode(LEX_OR, "or"),
    Opcode(LEX_OVER, "over"),
    Opcode(LEX_STORE, "store"),  // NOTE: Forth uses the symbol '!' for this 
    Opcode(LEX_SWAP, "swap"),
    Opcode(LEX_XOR, "xor"),
};

#endif /*__SM_OPCODE_HPP*/
