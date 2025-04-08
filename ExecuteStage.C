#include "PipeReg.h"
#include "M.h"
#include "E.h"
#include "ExecuteStage.h"
#include <iostream>

// Execute actions when the clock is low
bool ExecuteStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
    // Get the Execute and Memory stage registers
    M* mreg = static_cast<M*>(pregs[MREG]);
    E* ereg = static_cast<E*>(pregs[EREG]);

    // Retrieve instruction info and register values from E register
    uint64_t stat = ereg->getstat()->getOutput();
    uint64_t icode = ereg->geticode()->getOutput();
    
    uint64_t valA = ereg->getvalA()->getOutput();
    uint64_t valB = ereg->getvalB()->getOutput();
   
    uint64_t dstE = ereg->getdstE()->getOutput(); 
    uint64_t dstM = ereg->getdstM()->getOutput();

    uint64_t e_cnd = 0;  // default condition flag
    uint64_t E_valC = ereg->getvalC()->getOutput();
    uint64_t e_valE = E_valC;  // For now were just using valC as valE
   
    // Pass the computed values to Memory stage (M register)
    setMInput(mreg, stat, icode, valA, e_valE, dstE, dstM, e_cnd);

    return false; 
}
// Execute actions when the clock is high it will update registers
void ExecuteStage::doClockHigh(PipeReg ** pregs)
{
    M* mreg = static_cast<M*>(pregs[MREG]);

    // Update Memory stage register values officially
    mreg->getstat()->normal();
    mreg->geticode()->normal();
    mreg->getvalE()->normal();
    mreg->getvalA()->normal();
    mreg->getdstE()->normal();
    mreg->getdstM()->normal();
    mreg->getCnd()->normal();
}

// Helper function to set values for Memory stage M register
void ExecuteStage::setMInput(M* mreg, uint64_t stat, uint64_t icode, uint64_t valA, uint64_t valE, uint64_t dstE, uint64_t dstM, uint64_t e_cnd) {
    mreg->getstat()->setInput(stat);
    mreg->geticode()->setInput(icode);
    mreg->getvalE()->setInput(valE);
    mreg->getvalA()->setInput(valA);
    mreg->getCnd()->setInput(e_cnd);
    mreg->getdstE()->setInput(dstE);
    mreg->getdstM()->setInput(dstM);
}

