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

#include "source.hpp"

/*
 * Assembler.
 * Accept a FileInfo structure and produce a binary
 */
class Assembler
{
private:
    bool verbose;

private:


public:
    Assembler();

    int assem(const FileInfo& file_info);
    void setVerbose(void);
    void clearVerbose(void);
};



#endif /*__SM_ASSEMBLER_HPP*/
