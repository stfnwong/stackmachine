/*
 * LEXER
 * Lexer for Forth/Stack machine 
 *
 * Stefan Wong 2020
 */


#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.hpp"
#include "opcode.hpp"


// ================ LEXER ================ //
Lexer::Lexer() : verbose(false), cur_line(0), cur_pos(0), cur_char('\0'), cur_addr(0)
{
    // init sym table and so on here...
    this->token_buf_size = LEXER_TOKEN_BUF_SIZE;
    this->token_buf = new char[this->token_buf_size];
}

Lexer::~Lexer()
{
    delete[] this->token_buf;
}

void Lexer::init_instr_table(void)
{
    for(const Opcode& code : lex_opcodes)
        this->instr_table.add(code);
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
            this->line_info.error  = true;
            this->line_info.errstr = "Got blank symbol " + out_token.toString();
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
            this->line_info.error  = true;
            this->line_info.errstr = "Got blank symbol " + out_token.toString();
        }
        else
            this->cur_token = out_token;

        goto TOKEN_END;
    }

    // Check if this matches any instructions 
    op = this->instr_table.getName(token_str);       

    // TODO: debug, remove 
    std::cout << "[" << __func__ << "] got opcode " << op.toString() << std::endl;
          
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
    int line_num;
    Opcode op;

    this->line_info.init();
    line_num = this->cur_line;

    // get the next token
    this->scanToken();

    if(this->cur_token.type == TOK_LABEL)
    {

    }

    // Handle instructions
    if(this->cur_token.type == TOK_INSTR)
    {
        // valid check is already done in scanToken(), so don't need to check here
        op = this->instr_table.getName(this->cur_token.val);

        switch(op.instr)
        {
            case LEX_ADD:
                break;
            case LEX_AND:
                break;
        }

        goto PARSE_LINE_END;
    }

PARSE_LINE_END:
    if(this->verbose)
        std::cout << "[" << __func__ << "] TODO : print something useful here" << std::endl;

    this->file_info.add(this->line_info);       // TODO : this might not stay here
    this->cur_addr += LEXER_ADDR_INCR;
}

/*
 * Lexer::resolveLabels()
 */
void Lexer::resolveLabels(void)
{
    if(this->sym_table.size() == 0)
        return;

}


/*
 * Lexer::lex()
 */
void Lexer::lex(void)
{
    this->cur_line = 1;
    this->cur_pos = 0;

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


/*
 * Lexer::init()
 */
void Lexer::init(void)
{
    this->verbose = false;
    this->cur_line = 0;
    this->cur_pos = 0;
    this->cur_char = '\0';
    this->cur_addr = 0;
    this->init_instr_table();
}


/*
 * Lexer::read()
 */
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


/*
 * Lexer::setVerbose()
 */
void Lexer::setVerbose(void) 
{
    this->verbose = true;
}
/*
 *
 * Lexer::clearVerbose()
 */
void Lexer::clearVerbose(void) 
{
    this->verbose = false;
}

/*
 * Lexer::getVerbose()
 */
bool Lexer::getVerbose(void) const
{
    return this->verbose;
}
