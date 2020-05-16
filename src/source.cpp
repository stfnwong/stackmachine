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

/*
 * Symbol::==
 */
bool Symbol::operator==(const Symbol& that) const
{
    if(this->addr != that.addr)
        return false;
    if(this->sym != that.sym)
        return false;

    return true;
}

/*
 * Symbol::!=
 */
bool Symbol::operator!=(const Symbol& that) const
{
    return !(*this == that);
}

/*
 * Symbol::init()
 */
void Symbol::init(void)
{
    this->sym = "";
    this->addr = 0;
}

/*
 * Symbol::toString()
 */
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

/*
 * Symbol::init()
 */
void SymbolTable::init(void)
{
    this->syms.clear();
}

/*
 * Symbol::add()
 */
void SymbolTable::add(const Symbol& s)
{
    this->syms.push_back(s);
}

/*
 * Symbol::search()
 */
Symbol SymbolTable::search(const std::string& sym) const
{
    for(unsigned int s = 0; s < this->syms.size(); ++s)
    {
        if(this->syms[s].sym == sym)      
            return Symbol(this->syms[s]);       // return copy
    }

    return Symbol();
}

/*
 * Symbol::searchAddr()
 */
Symbol SymbolTable::searchAddr(uint32_t addr) const
{
    for(unsigned int s = 0; s < this->syms.size(); ++s)
    {
        if(this->syms[s].addr == addr)
            return Symbol(this->syms[s]);       // return copy
    }

    return Symbol();
}

/*
 * Symbol::size()
 */
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

/*
 * Token:==
 */
bool Token::operator==(const Token& that) const
{
    if(this->type != that.type)
        return false;
    if(this->val != that.val)
        return false;

    return true;
}

/*
 * Token::=
 */
bool Token::operator!=(const Token& that) const
{
    return !(*this == that);
}

/*
 * Token::init()
 */
void Token::init(void)
{
    this->type = TOK_NONE;
    this->val = "";
}

/*
 * Token::toString()
 */
std::string Token::toString(void) const
{
    std::ostringstream oss;

    switch(this->type)
    {
        case TOK_NONE:
            oss << "[NULL]";
            break;
        case TOK_REG:
            oss << "[REG]";
            break;
        case TOK_SYM:
            oss << "[SYM]";
            break;
        case TOK_INSTR:
            oss << "[INSTR]";
            break;
        case TOK_LITERAL:
            oss << "[LITERAL]";
            break;
        case TOK_LABEL:
            oss << "[LABEL]";
            break;
    }
    oss << " " << this->val;

    return oss.str();
}



// ================ LINEINFO ================ //

LineInfo::LineInfo() : 
    line_num(0),
    addr(0),
    opcode(Opcode()),
    literal(0),
    error(false),
    errstr(""),
    label(false),
    labelstr("")
{}


LineInfo::LineInfo(const LineInfo& that)
{
    this->line_num = that.line_num;
    this->addr     = that.addr;
    this->opcode   = that.opcode;
    this->literal  = that.literal;
    this->error    = that.error;
    this->errstr   = that.errstr;
    this->label    = that.label;
    this->labelstr = that.labelstr;
}


/*
 * LineInfo::==
 */
bool LineInfo::operator==(const LineInfo& that) const
{
    if(this->line_num != that.line_num)
        return false;
    if(this->addr != that.addr)
        return false;
    if(this->literal != that.literal)
        return false;
    if(this->error != that.error)
        return false;
    if(this->errstr != that.errstr)
        return false;
    if(this->labelstr != that.labelstr)
        return false;
    if(this->opcode != that.opcode)
        return false;
    
    return true;
}

/*
 * LineInfo::!=
 */
bool LineInfo::operator!=(const LineInfo& that) const
{
    return !(*this == that);
}

/*
 * LineInfo::init()
 */
void LineInfo::init(void)
{
    this->line_num  = 0;
    this->addr      = 0;
    this->literal   = 0;
    this->error     = false;
    this->errstr    = "";
    this->label     = false;
    this->labelstr  = "";
    this->opcode.init();
}

/*
 * LineInfo::toString()
 */
std::string LineInfo::toString(void) const
{
    std::ostringstream oss;

    // TODO: most of these aren't used yet...
    oss << "---------------------------------------------------------------------" << std::endl;
    oss << "Line  Type   Addr   Mnemonic    Opcode   Literal   error" << std::endl;

    oss << std::left << std::setw(6) << std::setfill(' ') << this->line_num;
    oss << "[";
    if(this->label)
        oss << "l";
    else
        oss << ".";
    oss << "]   ";
    // Address
    oss << std::right << "0x" << std::hex << std::setw(4) << std::setfill('0') << this->addr << "  ";
    // Mnemonic
    oss << std::left << std::setw(12) << std::setfill(' ') << this->opcode.mnemonic;
    // Instruction
    oss << "0x" << std::right << std::hex << std::setw(4) << std::setfill('0') << this->opcode.instr << "   ";
    // Literal
    oss << "0x" << std::right << std::hex << std::setw(4) << std::setfill('0') << this->literal << "  ";

    oss << std::endl;

    return oss.str();
}

/*
 * LineInfo::diffStr()
 */
std::string LineInfo::diffString(const LineInfo& that) const
{
    std::ostringstream oss;

    if(this->line_num != that.line_num)
        oss << "line_num (" << this->line_num << ") != (" << that.line_num << ")" << std::endl;
    if(this->addr != that.addr)
        oss << "addr (" << this->addr << ") != (" << that.addr << ")" << std::endl;
    if(this->opcode != that.opcode)
        oss << "opcode (" << this->opcode.toString() << ") != (" << that.opcode.toString() << ")" << std::endl;
    if(this->literal != that.literal)
        oss << "literal (" << this->literal << ") != (" << that.literal << ")" << std::endl;
    if(this->error != that.error)
        oss << "error (" << this->error << ") != (" << that.error << ")" << std::endl;
    if(this->errstr != that.errstr)
        oss << "errstr (" << this->errstr << ") != (" << that.errstr << ")" << std::endl;
    if(this->label != that.label)
        oss << "label (" << this->label << ") != (" << that.label << ")" << std::endl;
    if(this->labelstr != that.labelstr)
        oss << "labelstr (" << this->labelstr << ") != (" << that.labelstr << ")" << std::endl;

    return oss.str();
}



// ================ FILEINFO ================ //

FileInfo::FileInfo() {} 

/*
 * FileInfo::add()
 */
void FileInfo::add(const LineInfo& line) 
{
    this->lines.push_back(line);
}

/*
 * FileInfo::size()
 */
unsigned int FileInfo::size(void) const
{
    return this->lines.size();
}

/*
 * FileInfo::get()
 */
const LineInfo& FileInfo::get(unsigned int idx) const
{
    return this->lines[idx];        // TODO : bounds check
}

/*
 * FileInfo::toString()
 */
std::string FileInfo::toString(void) const
{
    std::ostringstream oss;

    return oss.str();
}
