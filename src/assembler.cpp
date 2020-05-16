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


Assembler::Assembler() : verbose(false) {} 


/*
 * Assembler::assem()
 */
int Assembler::assem(const FileInfo& file_info)
{
    int status = 0;

    // 
    for(unsigned int i = 0; i < file_info.size(); ++i)
    {
        LineInfo line = file_info.get(i);

        switch(line.opcode.instr)
        {

            case LEX_ADD:
                break;

            default:
                status = 0;
                goto ASSEM_END;
        }
    }

ASSEM_END:
    if(this->verbose && status < 0)
    {
        std::cout << "[" << __func__ << "] failed to assemble for some reason..." << std::endl;
    }

    return status;
}



/*
 * Assembler::setVerbose()
 */
void Assembler::setVerbose(void)
{
    this->verbose = true;
}
/*
 * Assembler::clearVerbose()
 */
void Assembler::clearVerbose(void)
{
    this->verbose = false;
}
