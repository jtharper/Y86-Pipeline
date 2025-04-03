#include "PipeReg.h"
#include "D.h"
#include "E.h"
#include "F.h"
#include "DecodeStage.h"
#include "Instructions.h"
#include "RegisterFile.h"
#include <iostream>

bool DecodeStage::doClockLow(PipeReg** pregs, Stage** stages) {
    // D and E registers
    D* dreg = static_cast<D*>(pregs[DREG]);  
    E* ereg = static_cast<E*>(pregs[EREG]);  

    // Get values needed from the D register
    uint64_t stat = dreg->getstat()->getOutput();
    uint64_t icode = dreg->geticode()->getOutput();
    uint64_t ifun = dreg->getifun()->getOutput();  
    uint64_t valC = dreg->getvalC()->getOutput();
     
     
    uint64_t D_rA = dreg->getrA()->getOutput(); 
    uint64_t D_rB = dreg->getrB()->getOutput();  
    uint64_t d_dstE = 0xf; 
    uint64_t d_dstM = 0xf;
    

    uint64_t d_srcA = set_d_srcA(icode, D_rA);
    uint64_t d_srcB = set_d_srcB(icode, D_rB);
    d_dstE = set_d_dstE(icode, D_rB);
    d_dstM = set_d_dstM(icode, D_rA);
    

    bool blah = false;
    uint64_t d_rvalA = RegisterFile::getInstance()->readRegister(d_srcA,blah);
    uint64_t d_rvalB = RegisterFile::getInstance()->readRegister(d_srcB,blah);

    uint64_t valA = set_d_valA(d_rvalA);
    uint64_t valB = set_d_valB(d_rvalB); 
   
    setEInput(ereg, stat, icode, ifun, valC, d_dstE, d_dstM, d_srcA, d_srcB);

    return false;
}

void DecodeStage::doClockHigh(PipeReg ** pregs)
{
    E* ereg = static_cast<E*>(pregs[EREG]);  
   

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

uint64_t DecodeStage::set_d_srcA(uint64_t D_icode, uint64_t D_rA) {
    if (D_icode == IRRMOVQ || D_icode == IRMMOVQ || D_icode == IOPQ || D_icode == IPUSHQ)
        return D_rA;
    else if (D_icode == IPOPQ || D_icode == IRET)
        return RSP;
    return RNONE;
}

uint64_t DecodeStage::set_d_srcB(uint64_t D_icode, uint64_t D_rB) {
    if (D_icode == IOPQ || D_icode == IRMMOVQ || D_icode == IMRMOVQ)
        return D_rB;
    else if (D_icode == IPUSHQ || D_icode == IPOPQ || D_icode == ICALL || D_icode == IRET)
        return RSP;
    return RNONE;
}

uint64_t DecodeStage::set_d_dstE(uint64_t D_icode, uint64_t D_rB) {
    if (D_icode == IRRMOVQ || D_icode == IIRMOVQ || D_icode == IOPQ)
        return D_rB;
    else if (D_icode == IPUSHQ || D_icode == IPOPQ || D_icode == ICALL || D_icode == IRET)
        return RSP;
    return RNONE;
}

uint64_t DecodeStage::set_d_dstM(uint64_t D_icode, uint64_t D_rA) {
    if (D_icode == IMRMOVQ || D_icode == IPOPQ)
        return D_rA;
    return RNONE;
}

uint64_t DecodeStage::set_d_valA(uint64_t d_rvalA) {
    return d_rvalA;
}

uint64_t DecodeStage::set_d_valB(uint64_t d_rvalB) {
    return d_rvalB;
}


