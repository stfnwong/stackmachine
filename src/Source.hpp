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
#include "Opcode.hpp"

/* 
 * Symbol - resolved in a later pass of the Lexer
 */
struct Symbol
{
    std::string sym;
    uint16_t    addr;

public:
    Symbol();
    Symbol(const std::string& s, uint16_t a);
    Symbol(const Symbol& that);

    Symbol& operator=(const Symbol& that) = default;
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
    Symbol searchAddr(uint16_t addr) const;
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


/*
 * LineInfo
 * Represents a single line of Stack Machine assembly
 */
struct LineInfo
{
    int         line_num;
    uint16_t    addr;
    Opcode      opcode;
    int         literal;
    bool        error;
    std::string errstr;
    std::string labelstr;

public:
    LineInfo();
    LineInfo(const LineInfo& that);

    LineInfo& operator=(const LineInfo& that) = default;
    bool operator==(const LineInfo& that) const;
    bool operator!=(const LineInfo& that) const;

    void init(void);
    std::string toString(void) const;
    std::string diffString(const LineInfo& that) const;
};


// TODO : this processor does not yet have a data/text segment distinction
/*
 * FileInfo
 * Represents a file (collection of LineInfos)
 */
struct FileInfo
{
    std::vector<LineInfo> lines;

public:
    FileInfo();

    void add(const LineInfo& line);
    //void update(const LineInfo& line, unsigned int idx);
    unsigned int size(void) const;

    const LineInfo& get(unsigned int idx) const;
    std::string toString(void) const;
};


#endif /*__SM_SOURCE_HPP*/
