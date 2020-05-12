/*
 * SOURCE
 *  Stuff for handling source transformations 
 *
 *  Stefan Wong 2020
 */


#ifndef __SM_SOURCE_HPP
#define __SM_SOURCE_HPP

#include <cstdint>
#include <string>
#include <vector>

/* 
 * Symbol - resolved in a later pass of the Lexer
 */
struct Symbol
{
    std::string sym;
    uint32_t addr;

public:
    Symbol();
    Symbol(const std::string& s, uint32_t a);
    Symbol(const Symbol& that);

    bool operator==(const Symbol& that) const;
    bool operator!=(const Symbol& that) const;

    void init(void);
    std::string toString(void) const;
};

/*
 * symbol table
 */
class SymbolTable
{
    std::vector<Symbol> syms;

public:
    SymbolTable();
    SymbolTable(const SymbolTable& that) = delete;

    void init(void);
    void add(const Symbol& s);
    Symbol search(const std::string& sym) const;
    Symbol searchAddr(uint32_t addr) const;
    unsigned int size(void) const;
};


// Don't worry about string tokens for now, at first
// the machine will just manipulate digits
typedef enum 
{
    TOK_NONE,
    TOK_REG,
    TOK_SYM,
    TOK_INSTR,
    TOK_LITERAL, 
    TOK_LABEL
} TokenType;

/* 
 * Token
 */
struct Token
{
    TokenType type;
    std::string val;

public:
    Token();
    Token(TokenType t, const std::string& v);
    Token(const Token& that);

    bool operator==(const Token& that) const;
    bool operator!=(const Token& that) const;
    //Token& operator=(const Token& that) const = default;

    void init(void);
    std::string toString(void) const;
};


#endif /*__SM_SOURCE_HPP*/