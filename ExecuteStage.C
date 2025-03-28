#include "PipeReg.h"
#include "M.h"
#include "E.h"
#include "ExecuteStage.h"

bool ExecuteStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
    M* mreg = static_cast<M*>(pregs[MREG]);  
    E* ereg = static_cast<E*>(pregs[EREG]);  

    // Get values needed from the D register
    uint64_t stat = ereg->getstat()->getOutput();
    uint64_t icode = ereg->geticode()->getOutput();
    
    uint64_t valA= ereg->getvalA()->getOutput();
    uint64_t valB = ereg->getvalB()->getOutput();  
   
    uint64_t dstE = ereg->getdstE()->getOutput(); 
    uint64_t dstM = ereg->getdstM()->getOutput();
    uint64_t e_cnd = 0;

    uint64_t valE = 0;
    setMInput(mreg, stat, icode, valA, valE, dstE, dstM, e_cnd);
    return false;
} 
void ExecuteStage::doClockHigh(PipeReg ** pregs)
{
    M* mreg = static_cast<M*>(pregs[MREG]);
    mreg->getstat()->normal();
    mreg->geticode()->normal();
    mreg->getvalE()->normal();
    mreg->getvalA()->normal();
    mreg->getdstE()->normal();
    mreg->getdstM()->normal();
    mreg->getCnd()->normal();

}
void ExecuteStage::setMInput(M* mreg, uint64_t stat, uint64_t icode, uint64_t valA, uint64_t valE, uint64_t dstE, uint64_t dstM, uint64_t e_cnd) {
    mreg->getstat()->setInput(stat);
    mreg->geticode()->setInput(icode);
    mreg->getvalE()->setInput(valE);
    mreg->getvalA()->setInput(valA);
    mreg->getCnd()->setInput(e_cnd);
    mreg->getdstE()->setInput(dstE);
    mreg->getdstM()->setInput(dstM);
}


