#include "PipeReg.h"
#include "D.h"
#include "E.h"
#include "F.h"
#include "DecodeStage.h"

bool DecodeStage::doClockLow(PipeReg** pregs, Stage** stages) {
    // D and E registers
    D* dreg = static_cast<D*>(pregs[DREG]);  
    E* ereg = static_cast<E*>(pregs[EREG]);  

    // Get values needed from the D register
    uint64_t stat = dreg->getstat()->getOutput();
    uint64_t icode = dreg->geticode()->getOutput();
    uint64_t ifun = dreg->getifun()->getOutput();  
    uint64_t valC = dreg->getvalC()->getOutput();
    uint64_t valA = dreg->getrA()->getOutput();  
    uint64_t valB = dreg->getrB()->getOutput();  
    uint64_t srcA = dreg->getrA()->getOutput(); 
    uint64_t srcB = dreg->getrB()->getOutput();  
    uint64_t dstE = 0xf; 
    uint64_t dstM = 0xf; 


    // Logic to compute valA, valB, srcA, srcB, dstE, and dstM could go here
    // For example, depending on icode or other pipeline state, values may be overridden

    // Call setEInput to pass these values to the E pipeline register
    setEInput(ereg, stat, icode, ifun, valC, dstE, dstM, srcA, srcB);

    // Return false (default behavior as per the prompt; can adjust if needed)
    return false;
}

void DecodeStage::doClockHigh(PipeReg ** pregs)
{
    E* ereg = static_cast<E*>(pregs[EREG]);  
    //not sure yet might need to call normal on dreg
    D* dreg = static_cast<D*>(pregs[DREG]);  

    // Call the normal method 
    ereg->getstat()->normal();
    ereg->geticode()->normal();
    ereg->getifun()->normal();
    ereg->getvalC()->normal();
    ereg->getvalA()->normal();
    ereg->getvalB()->normal();
    ereg->getdstE()->normal();
    ereg->getdstM()->normal();
    ereg->getsrcA()->normal();
    ereg->getsrcB()->normal();



}

void DecodeStage::setEInput(E * ereg, uint64_t stat, uint64_t icode, uint64_t ifun,
                            uint64_t valC,
                            uint64_t dstE, uint64_t dstM, uint64_t srcA, uint64_t srcB) {
                                
    ereg->getstat()->setInput(stat);
    ereg->geticode()->setInput(icode);
    ereg->getifun()->setInput(ifun);
    ereg->getvalC()->setInput(valC);
    ereg->getvalA()->setInput(0);
    ereg->getvalB()->setInput(0);
    ereg->getdstE()->setInput(dstE);
    ereg->getdstM()->setInput(dstM);
    ereg->getsrcA()->setInput(srcA);
    ereg->getsrcB()->setInput(srcB);
}


