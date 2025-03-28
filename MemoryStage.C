#include "PipeReg.h"
#include "W.h"
#include "M.h"
#include "E.h"
#include "Stage.h"
#include "MemoryStage.h"
#include "Instructions.h"
bool MemoryStage::doClockLow(PipeReg ** pregs, Stage ** stages) {
    // Get the M (MemoryStage) and W (WritebackStage) pipeline registers
    M* mreg = static_cast<M*>(pregs[MREG]);
    W* wreg = static_cast<W*>(pregs[WREG]);  // Correctly access W register (WREG)

    // Extract values from M register
    uint64_t stat = mreg->getstat()->getOutput();
    uint64_t icode = mreg->geticode()->getOutput();
    uint64_t dstM = mreg->getdstM()->getOutput();
    uint64_t dstE = mreg->getdstE()->getOutput();
    uint64_t valE = mreg->getvalE()->getOutput();
    uint64_t valM = 0; // Assuming memory read/write is not implemented yet

    // Call setWinput to update W register
    setWinput(wreg, stat, icode, valE, dstE, dstM, valM);

    return false; // Continue execution until WritebackStage halts
}

void MemoryStage::doClockHigh(PipeReg ** pregs)
{
  W* wreg = static_cast<W*>(pregs[WREG]);  
  wreg->getdstE()->normal();
  wreg->getdstM()->normal();
  wreg->getvalE()->normal();
  wreg->getvalM()->normal();
  wreg->getstat()->normal();
  wreg->geticode()->normal();

}

void MemoryStage::setWinput(W * wreg, uint64_t stat,uint64_t icode, uint64_t valE, uint64_t dstE,uint64_t dstM, uint64_t valM)
{
    wreg->getstat()->setInput(stat);
    wreg->geticode()->setInput(icode);
    wreg->getvalE()->setInput(valE);
    wreg->getvalM()->setInput(0);
    wreg->getdstE()->setInput(dstE);
    wreg->getdstM()->setInput(dstM);

}


