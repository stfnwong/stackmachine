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

/*
 *  Lexer 
 */
class Lexer
{
    // generic stuff
private:
    bool verbose;

    // source position
private:
    int cur_line;
    int cur_pos;
    char cur_char;
    char* token_buf;
    int token_buf_size;

private:
    Token cur_token;
    // LineInfo?

    // Move thorugh source 
private:
    std::string source_text;
    std::string filename;
    bool isSpace(void) const;
    bool isComment(void) const;
    bool exhausted(void) const;
    void advance(void);
    void skipWhitespace(void);
    void skipSeperators(void);
    void skipComment(void);
    void scanToken(void);
    void nextToken(void);

    // extract tokens
private:
    Token extractReg(const std::string& token, unsigned int start_offset, unsigned int end_offset);
    Token extractLiteral(const std::string& token, unsigned int start_offset, unsigned int end_offset);
    //void extractInstr(void):

private:
    void parseLine(void);
    void resolveLabels(void);

public:
    Lexer();
    Lexer(const Lexer& that) = delete;
    Lexer(const Lexer&& that) = delete;
    ~Lexer();

    Lexer& operator=(const Lexer& that) = delete;       // don't allow assignment

    void lex(void);
    int read(const std::string& filename);
};


#endif /*__SM_ASSEMBLER_HPP*/
