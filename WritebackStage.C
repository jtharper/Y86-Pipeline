#include "PipeReg.h"
#include "W.h"
#include "Status.h"
#include "WritebackStage.h"
#include "Instructions.h"
#include "RegisterFile.h"
#include <ostream>
#include <iostream>

// Writeback actions when the clock is low
bool WritebackStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
    W * wreg = (W *) pregs[WREG];

    // Gets the status and instruction code from w register
    uint64_t w_stat = wreg->getstat()->getOutput();
    uint64_t w_icode = wreg->geticode()->getOutput();

    // Check if there's an error or halt condition - We might need to update this after looking through lab 8
    if (w_stat != SAOK) {
        return true; // Stops the simulation on error
    }

    if (w_icode == IHALT) {
        return true; 
    }

    return false; // Continue normally 
}

// Writeback actions when the clock is high
void WritebackStage::doClockHigh(PipeReg ** pregs)
{
    W * wreg = (W *) pregs[WREG];
    bool placeholder = false;

    // Gets values from w register to write back to registers
    uint64_t W_valE = wreg->getvalE()->getOutput();
    uint64_t W_dstE = wreg->getdstE()->getOutput();

    // Write the values into the register file
    RegisterFile::getInstance()->writeRegister(W_valE, W_dstE, placeholder);
}