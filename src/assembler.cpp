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
#include "assembler.hpp"

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


// ================ LEXER ================ //
Lexer::Lexer() : verbose(false), cur_line(0), cur_pos(0), cur_char('\0') 
{
    // init sym table and so on here...
    this->token_buf_size = 64;
    this->token_buf = new char[this->token_buf_size];
}

Lexer::~Lexer()
{
    delete[] this->token_buf;
}

bool Lexer::exhausted(void) const
{
    return (this->cur_char == '\0' ||
            this->cur_pos >= this->source_text.size()) ? true : false;
}

void Lexer::advance(void)
{
    this->cur_pos++;
    // dont let the cur_pos go over the end of the source text array
    if(this->cur_pos >= this->source_text.size())
        this->cur_char = '\0';
    else
        this->cur_char = this->source_text[this->cur_pos];

    if(this->cur_char == '\n')
        this->cur_line = this->cur_line + 1;
}


/*
 * isSpace()
 */
bool Lexer::isSpace(void) const
{
    return (this->cur_char == ' '  || 
            this->cur_char == '\t' ||
            this->cur_char == '\n') ? true : false;
}

bool Lexer::isComment(void) const
{
    return (this->cur_char == ';') || (this->cur_char == '#');
}

/*
 * skipWhitespace()
 */
void Lexer::skipWhitespace(void) 
{
    while(!this->exhausted())
    {
        if(this->cur_char == ' '  || 
           this->cur_char == '\n' || 
           this->cur_char == '\t')
            this->advance();
        else
            break;
    }
}

/*
 * skipComment()
 */
void Lexer::skipComment(void)
{
    while(this->cur_char != '\n')
        this->advance();
}

/*
 * skipSeperators()
 */
void Lexer::skipSeperators(void)
{
    while(!this->exhausted())
    {
        if(this->cur_char == ',' ||
           this->cur_char == ':' ||
           this->cur_char == ';')
            this->advance();
        else
            break;
    }
}

/*
 * scanToken()
 * Scan a complete token into the token buffer
 */
void Lexer::scanToken(void)
{
    int idx = 0;

    this->skipWhitespace();     // eat any leading whitespace 
    this->skipSeperators();     // eat any seperators that might be left
    while(idx < (this->token_buf_size-1))
    {
        if(this->cur_char == ' ')       // space 
            break;
        if(this->cur_char == '\n')      // newline
            break;
        if(this->cur_char == ';')       // comment
            break;
        if(this->cur_char == '#')       // also comment
        {
            this->skipComment();
            break;
        }
        if(this->cur_char == ',')       // seperator
            break;
        if(this->cur_char == ':')       // end of label
            break;

        this->token_buf[idx] = this->cur_char;
        this->advance();
        idx++;
    }
    this->token_buf[idx] = '\0';
    // If we are on a seperator now, advance the source pointer 
    if(this->cur_char == ',')
        this->advance();
}

/*
 * nextToken()
 */
void Lexer::nextToken(void)
{
    Opcode op;
    Token out_token;
    std::string token_str;
    unsigned int start_offset; 
    unsigned int end_offset;

    this->scanToken();
    token_str = std::string(this->token_buf);
    // set the offset string for the current token back to null
    this->cur_token.init();

    // From here on we can force the case to match the case we are using 
    // in the source (in this instance, uppercase)
    std::transform(token_str.begin(), token_str.end(), token_str.begin(),
            [](unsigned char c){ return std::tolower(c); });

    start_offset = 0;
    // Check digits, which may be either literals or register offsets 
    if(std::isdigit(token_str[0]))
    {
        out_token = this->extractLiteral(token_str, start_offset, end_offset);
        if(out_token.type == TOK_NONE)
        {
            // got blank symbol
        }
        else
            this->cur_token = out_token;
        goto TOKEN_END;
    }

    // Check registers or expand parenthesis
    if((token_str[0] == '$') || (token_str[0] == '('))
    {
        out_token = this->extractReg(token_str, start_offset, end_offset);
        if(out_token.type == TOK_NONE)
        {
            // got blank symbol
        }
        else
            this->cur_token = out_token;

        goto TOKEN_END;
    }

    // Check if this matches any instructions 
    //op = this->instr_code_table.get(token_str);       // TODO : need a table for this
          
    // Found an instruction
    if(op.mnemonic != "\0")
    {
        this->cur_token.type = TOK_INSTR;
        this->cur_token.val  = std::string(this->token_buf);
    }
    // Exhausted all options - assign as label 
    else
    {
        this->cur_token.type = TOK_LABEL;
        this->cur_token.val  = token_str;
    }

TOKEN_END:
    if(this->verbose)
    {
        std::cout << "[" << __func__ << "] (line " << std::dec << 
            this->cur_line << ") got " << this->cur_token.toString() << 
            " token <" << token_str << "> with value <" << 
            this->cur_token.val << ">" << std::endl;
    }
}



// === extract tokens ==== //
Token Lexer::extractReg(const std::string& token, unsigned int start_offset, unsigned int end_offset)
{
    Token out_token;

    return out_token;       // shut linker up
}

Token Lexer::extractLiteral(const std::string& token, unsigned int start_offset, unsigned int end_offset) 
{
    Token out_token;

    return out_token;       // shut linker up
}

// === lex out tokens ==== //
void Lexer::parseLine(void)
{
    // get the next token
    this->scanToken();

    //if(this->
}

void Lexer::resolveLabels(void)
{

}


void Lexer::lex(void)
{
    this->cur_line = 1;
    this->cur_pos = 0;
    //this->text_addr = this->start_addr;     // TODO : proper address init..

    while(!this->exhausted())
    {
        // eat spaces 
        if(this->isSpace())
        {
            this->advance();
            continue;
        }

        // eat comments 
        if(this->isComment())
        {
            this->skipComment();
            continue;
        }
        this->parseLine();
    }

    // Resolve symbols
    this->resolveLabels();
}

int Lexer::read(const std::string& filename)
{
    std::ifstream infile(filename);
    std::string line;
    int status = 0;             // TODO : actually do some checks and return status (rather than throw)

    // save the filename
    this->filename = filename;
    while(std::getline(infile, line))
    {
        this->source_text += line;
        this->source_text.push_back('\n');
    }

    if(this->verbose)
    {
        std::cout << "[" << __func__ << "] read " << 
            std::dec << this->source_text.length() << " characters from file ["
            << filename << "]" << std::endl;
    }

    infile.close();
    this->source_text.push_back('\0');
    this->cur_char = this->source_text[0];

    return status;
}
