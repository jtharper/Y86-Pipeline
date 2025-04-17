#include "PipeReg.h"
#include "W.h"
#include "M.h"
#include "E.h"
#include "Stage.h"
#include "MemoryStage.h"
#include "Instructions.h"

// Memory actions when the clock is low
bool MemoryStage::doClockLow(PipeReg ** pregs, Stage ** stages) {
    // Accessing memory and writeback stage pipeline registers
    M* mreg = static_cast<M*>(pregs[MREG]);
    W* wreg = static_cast<W*>(pregs[WREG]);

    // Gets the values stored in the M register
    uint64_t stat = mreg->getstat()->getOutput();
    uint64_t icode = mreg->geticode()->getOutput();
    uint64_t dstM = mreg->getdstM()->getOutput();
    uint64_t dstE = mreg->getdstE()->getOutput();
    uint64_t valE = mreg->getvalE()->getOutput();
    uint64_t valM = 0;

    if (icode == INOP){
        valE = 0;
    }
    // Passes values forward to the Writeback stage
    setWinput(wreg, stat, icode, valE, dstE, dstM, valM);

    return false; 
}

// Updates the values officially when clock is high
void MemoryStage::doClockHigh(PipeReg ** pregs)
{
    W* wreg = static_cast<W*>(pregs[WREG]);  

    // Updates Writeback stage registers
    wreg->getdstE()->normal();
    wreg->getdstM()->normal();
    wreg->getvalE()->normal();
    wreg->getvalM()->normal();
    wreg->getstat()->normal();
    wreg->geticode()->normal();
}

// Helper function that load values into writeback stage
void MemoryStage::setWinput(W * wreg, uint64_t stat, uint64_t icode, uint64_t valE, uint64_t dstE, uint64_t dstM, uint64_t valM)
{
    wreg->getstat()->setInput(stat);
    wreg->geticode()->setInput(icode);
    wreg->getvalE()->setInput(valE);
    wreg->getvalM()->setInput(0); // No data for now will probably need to update
    wreg->getdstE()->setInput(dstE);
    wreg->getdstM()->setInput(dstM);
}