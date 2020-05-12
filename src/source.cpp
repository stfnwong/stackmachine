/*
 * SOURCE
 *  Stuff for handling source transformations 
 *
 *  Stefan Wong 2020
 */

#include <iomanip>
#include <sstream>
#include "source.hpp"


// ================ SYMBOL ================ //
Symbol::Symbol() : sym(""), addr(0) {} 

Symbol::Symbol(const std::string& s, uint32_t a) : sym(s), addr(a) {} 

Symbol::Symbol(const Symbol& that)
{
    this->sym = that.sym;
    this->addr = that.addr;
}

bool Symbol::operator==(const Symbol& that) const
{
    if(this->addr != that.addr)
        return false;
    if(this->sym != that.sym)
        return false;

    return true;
}

bool Symbol::operator!=(const Symbol& that) const
{
    return !(*this == that);
}

void Symbol::init(void)
{
    this->sym = "";
    this->addr = 0;
}

std::string Symbol::toString(void) const
{
    std::ostringstream oss;

    oss << "[" << this->sym << "]  0x" << std::hex 
        << std::setw(4) << std::setfill('0') 
        << this->addr;

    return oss.str();
}

/*
 * SymbolTable
 */
SymbolTable::SymbolTable() {} 

void SymbolTable::init(void)
{
    this->syms.clear();
}

void SymbolTable::add(const Symbol& s)
{
    this->syms.push_back(s);
}

Symbol SymbolTable::search(const std::string& sym) const
{
    for(unsigned int s = 0; s < this->syms.size(); ++s)
    {
        if(this->syms[s].sym == sym)      
            return Symbol(this->syms[s]);       // return copy
    }

    return Symbol();
}

Symbol SymbolTable::searchAddr(uint32_t addr) const
{
    for(unsigned int s = 0; s < this->syms.size(); ++s)
    {
        if(this->syms[s].addr == addr)
            return Symbol(this->syms[s]);       // return copy
    }

    return Symbol();
}

unsigned int SymbolTable::size(void) const
{
    return this->syms.size();
}

// ================ TOKEN ================ //
Token::Token() : type(TOK_NONE), val("") {} 

Token::Token(TokenType t, const std::string& v) : type(t), val(v) {} 

Token::Token(const Token& that) 
{
    this->type = that.type;
    this->val  = that.val;
}

bool Token::operator==(const Token& that) const
{
    if(this->type != that.type)
        return false;
    if(this->val != that.val)
        return false;

    return true;
}

bool Token::operator!=(const Token& that) const
{
    return !(*this == that);
}

void Token::init(void)
{
    this->type = TOK_NONE;
    this->val = "";
}

std::string Token::toString(void) const
{
    std::ostringstream oss;

    switch(this->type)
    {
        case TOK_NONE:
            oss << "[NULL]";
        case TOK_REG:
            oss << "[REG]";
        case TOK_SYM:
            oss << "[SYM]";
        case TOK_INSTR:
            oss << "[INSTR]";
        case TOK_LITERAL:
            oss << "[LITERAL]";
        case TOK_LABEL:
            oss << "[LABEL]";
    }
    oss << " " << this->val;

    return oss.str();
}

