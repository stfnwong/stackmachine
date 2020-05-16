/*
 * LEXER
 * Lexer for Forth/Stack machine 
 *
 * Stefan Wong 2020
 */

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.hpp"
#include "opcode.hpp"


// ================ LEXER ================ //
Lexer::Lexer() : 
    verbose(false), 
    cur_line(0), 
    cur_col(0), 
    cur_pos(0), 
    cur_char('\0'), 
    cur_addr(0)
{
    // init sym table and so on here...
    this->token_buf_size = LEXER_TOKEN_BUF_SIZE;
    this->token_buf = new char[this->token_buf_size];
    this->init_instr_table();
}

Lexer::~Lexer()
{
    delete[] this->token_buf;
}

/*
 * Lexer::init_instr_table()
 */
void Lexer::init_instr_table(void)
{
    for(const Opcode& code : lex_opcodes)
        this->instr_table.add(code);
}

/*
 * Lexer::exhausted()
 */
bool Lexer::exhausted(void) const
{
    return (this->cur_char == '\0' ||
            this->cur_pos >= this->source_text.size()) ? true : false;
}

/*
 * Lexer::advance()
 */
void Lexer::advance(void)
{
    this->cur_pos++;
    // dont let the cur_pos go over the end of the source text array
    if(this->cur_pos >= this->source_text.size())
        this->cur_char = '\0';
    else
        this->cur_char = this->source_text[this->cur_pos];

    this->cur_col++;
    if(this->cur_char == '\n')
    {
        this->cur_line = this->cur_line + 1;
        this->cur_col = 0;
    }
}


/*
 * Lexer::isSpace()
 */
bool Lexer::isSpace(void) const
{
    return (this->cur_char == ' '  || 
            this->cur_char == '\t' ||
            this->cur_char == '\n') ? true : false;
}

/*
 * Lexer::isComment()
 */
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
 * Lexer::skipComment()
 */
void Lexer::skipComment(void)
{
    while(this->cur_char != '\n')
        this->advance();
}

/*
 * Lexer::skipSeperators()
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
 * Lexer::scanToken()
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
 * Lexer::nextToken()
 */
void Lexer::nextToken(void)
{
    Opcode op;
    Token out_token;
    std::string token_str;

    this->scanToken();
    token_str = std::string(this->token_buf);
    // set the offset string for the current token back to null
    this->cur_token.init();

    // From here on we can force the case to match the case we are using 
    // in the source (in this instance, uppercase)
    std::transform(token_str.begin(), token_str.end(), token_str.begin(),
            [](unsigned char c){ return std::toupper(c); });

    if(token_str[0] == '$')
    {
        out_token = this->extractLiteral(token_str, 1);
        if(out_token.type == TOK_NONE)
        {
            this->line_info.error  = true;
            this->line_info.errstr = "Got blank symbol " + out_token.toString();
        }
        else
            this->cur_token = out_token;
        goto NEXT_TOKEN_END;
    }

    // For now literals always start with '$' and are in hexadecimal
    // Note for myself - we almost never have literals because this
    // is a stack machine
    if(token_str[0] == '$')
    {
        out_token = this->extractLiteral(token_str, 0); 
        if(out_token.type == TOK_NONE)
        {
            this->line_info.error  = true;
            this->line_info.errstr = "Got blank symbol " + out_token.toString();
        }
        else
            this->cur_token = out_token;

        goto NEXT_TOKEN_END;
    }

    // Check if this matches any instructions 
    op = this->instr_table.getName(token_str);       

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

NEXT_TOKEN_END:
    if(this->verbose)
    {
        std::cout << "[" << __func__ << "] (line " << std::dec << 
            this->cur_line << ") got " << this->cur_token.toString() << 
            " token <" << token_str << "> with value <" << 
            this->cur_token.val << ">" << std::endl;
    }
}



// === extract tokens ==== //

/*
 * Lexer::extractLiteral()
 */ 
Token Lexer::extractLiteral(const std::string& token, unsigned int start_offset)
{
    unsigned int tok_ptr;
    std::string literal_token;
    Token out_token;
    
    // At the moment, literals start with '$' and are always hexadecimal
    if(token[0] != '$')
        return out_token;

    tok_ptr = start_offset;
    while(std::isdigit(token[tok_ptr]))
        tok_ptr++;

    if(tok_ptr == start_offset)     // no chars were consumed
        goto EXTRACT_LITERAL_END;

    out_token.val  = token.substr(start_offset, tok_ptr - start_offset);
    out_token.type = TOK_LITERAL;

EXTRACT_LITERAL_END:
    return out_token;       // shut linker up
}

/*
 * Lexer::convertLiteral()
 */
int Lexer::convertLiteral(const std::string& literal)
{
    return std::stoi(literal, nullptr, 16);
}

// === lex out tokens ==== //
/*
 * Lexer::parseLine()
 */
int Lexer::parseLine(void)
{
    int line_num;
    Opcode op;
    Symbol sym;

    this->line_info.init();
    line_num = this->cur_line;
    this->nextToken();

    if(this->cur_token.type == TOK_LABEL)
    {
        // add symbol to table, remove any trailing characters
        char last_char = this->cur_token.val[
            this->cur_token.val.length()
        ];

        if((last_char == ';') || 
           (last_char == '#') ||
           (last_char == ':'))
        {
            sym.sym = this->cur_token.val.substr(
                    0, this->cur_token.val.length() - 1
            );
        }
        else
            sym.sym = this->cur_token.val;

        sym.addr = this->cur_addr;
        // add to symbol table 
        this->sym_table.add(sym);

        this->advance();        // skip ahead to align pointer for next token
        // scan in the next token
        this->nextToken(); 
        line_num = this->cur_line;   // if the label is on a seperate line then record the new current line
    }

    // Handle instructions
    if(this->cur_token.type == TOK_INSTR)
    {
        op = this->instr_table.getName(this->cur_token.val);
        this->line_info.opcode = op;

        switch(op.instr)
        {
            // NOTE: the only two instructions that take arguments right now are store and fetch
            case LEX_STORE:
            case LEX_FETCH:
                this->nextToken();
                std::cout << "[" << __func__ << "] this->cur_token : " << this->cur_token.toString() << std::endl;
                if(this->cur_token.type != TOK_LITERAL)
                {
                    this->line_info.error = true;
                    this->line_info.errstr = "Expected literal, got " + cur_token.toString();
                    goto PARSE_LINE_END;
                }
                this->line_info.literal = this->convertLiteral(this->cur_token.val);
                break;

            case LEX_ADD:
            case LEX_AND:
            case LEX_DUP:
            case LEX_DROP:
            case LEX_OR:
            case LEX_OVER:
            case LEX_RPUSH:
            case LEX_RPOP:
            case LEX_SUB:
            case LEX_XOR:
            case LEX_IF:
            case LEX_CALL:
            case LEX_EXIT:
            case LEX_SWAP:
            case LEX_LIT:
                break;

            default:
                this->line_info.error = true;
                this->line_info.errstr = "Unknown opcode " + op.toString();
                break;
        }

        goto PARSE_LINE_END;
    }


PARSE_LINE_END:
    if(this->verbose)
    {
        if(this->line_info.error)
            std::cout << "[" << __func__ << "] error : " << this->line_info.errstr << std::endl;
    }

    this->line_info.line_num = line_num;
    this->line_info.addr     = this->cur_addr;
    this->file_info.add(this->line_info);       // TODO : this might not stay here
    this->cur_addr += LEXER_ADDR_INCR;

    return (this->line_info.error) ? 1 : 0;
}

/*
 * Lexer::resolveLabels()
 */
void Lexer::resolveLabels(void)
{
    if(this->sym_table.size() == 0)
        return;

    std::cout << "[" << __func__ << "] when there are symbols to resolve, resolve them here" << std::endl;
}


/*
 * Lexer::lex()
 */
int Lexer::lex(void)
{
    int status = 0;
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
            std::cout << "[" << __func__ << "] found comment on line " 
                << this->cur_line << " at column (skipping to end of line) " << this->cur_col << std::endl;
            this->skipComment();
            continue;
        }
        // TODO : remove 
        std::cout << "[" << __func__ << "] parsing line " << this->cur_line 
            << " starting at column " << this->cur_col << std::endl;
        status = this->parseLine();

        if(status < 0)
            goto LEX_END;
    }

    // Resolve symbols
    this->resolveLabels();

LEX_END:
    if(status < 0)
        std::cout << "[" << __func__ << "] print something useful here about errors" << std::endl;

    return status;
}

/*
 * Lexer::init()
 */
void Lexer::init(void)
{
    this->verbose = false;
    this->cur_line = 0;
    this->cur_col = 0;
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

/*
 * Lexer::getFileInfo()
 */
const FileInfo& Lexer::getFileInfo(void) const
{
    return this->file_info;
}
