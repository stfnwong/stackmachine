/*
 * LEXER
 * Lexer for Forth/Stack machine 
 *
 * Stefan Wong 2020
 */

#ifndef __SM_LEXER_HPP
#define __SM_LEXER_HPP

#include <cstdint>
#include <string>

#include "source.hpp"

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


#endif /*__SM_LEXER_HPP*/
