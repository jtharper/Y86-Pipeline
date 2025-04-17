#include "PipeReg.h"
#include "D.h"
#include "E.h"
#include "F.h"
#include "M.h"
#include "W.h"
#include "DecodeStage.h"
#include "Instructions.h"
#include "RegisterFile.h"
#include <iostream>

// Decode step when the clock is low
bool DecodeStage::doClockLow(PipeReg** pregs, Stage** stages) {
    // Get the D and E stage registers
    D* dreg = static_cast<D*>(pregs[DREG]);
    E* ereg = static_cast<E*>(pregs[EREG]);
    M* mreg = static_cast<M*>(pregs[MREG]);
    W* wreg = static_cast<W*>(pregs[WREG]);

    // Pull instruction data from the D register
    uint64_t stat = dreg->getstat()->getOutput();
    uint64_t icode = dreg->geticode()->getOutput();
    uint64_t ifun = dreg->getifun()->getOutput();
    uint64_t valC = dreg->getvalC()->getOutput();

    // Get the register numbers used by the instruction
    uint64_t D_rA = dreg->getrA()->getOutput();
    uint64_t D_rB = dreg->getrB()->getOutput();

    // Decide which registers we need to read and write
    uint64_t d_srcA = set_d_srcA(icode, D_rA);
    uint64_t d_srcB = set_d_srcB(icode, D_rB);
    uint64_t d_dstE = set_d_dstE(icode, D_rB);
    uint64_t d_dstM = set_d_dstM(icode, D_rA);

    bool unused = false; // Just a placeholder
    // Read values from the register file
    uint64_t d_rvalA = RegisterFile::getInstance()->readRegister(d_srcA, unused);
    uint64_t d_rvalB = RegisterFile::getInstance()->readRegister(d_srcB, unused);
    uint64_t fwdA = set_d_valA(d_rvalA, ereg, d_srcA, mreg, wreg);
    uint64_t fwdB = set_d_valB(d_rvalB, d_srcB, ereg, mreg, wreg);

    // Pass the register values to the next stage (E stage)
    setEInput(ereg, stat, icode, ifun, valC, d_dstE, d_dstM, d_srcA, d_srcB);
    ereg->getvalA()->setInput(fwdA);
    ereg->getvalB()->setInput(fwdB);
    return false; 
}

// When clock is high, update the values 
void DecodeStage::doClockHigh(PipeReg ** pregs) {
    E* ereg = static_cast<E*>(pregs[EREG]);

    // Update all the E stage values
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

// Load values into the Execute register E stage
void DecodeStage::setEInput(E * ereg, uint64_t stat, uint64_t icode, uint64_t ifun,
                            uint64_t valC,
                            uint64_t dstE, uint64_t dstM, uint64_t srcA, uint64_t srcB) {
    ereg->getstat()->setInput(stat);
    ereg->geticode()->setInput(icode);
    ereg->getifun()->setInput(ifun);
    ereg->getvalC()->setInput(valC);
    ereg->getvalA()->setInput(0); // start with 0
    ereg->getvalB()->setInput(0); // start with 0
    ereg->getdstE()->setInput(dstE);
    ereg->getdstM()->setInput(dstM);
    ereg->getsrcA()->setInput(srcA);
    ereg->getsrcB()->setInput(srcB);
}

// Decide source register A based on the instruction
uint64_t DecodeStage::set_d_srcA(uint64_t D_icode, uint64_t D_rA) {
    if (D_icode == IRRMOVQ || D_icode == IRMMOVQ || D_icode == IOPQ || D_icode == IPUSHQ)
        return D_rA;
    else if (D_icode == IPOPQ || D_icode == IRET)
        return RSP;
    return RNONE;
}

// Decide source register B based on the instruction
uint64_t DecodeStage::set_d_srcB(uint64_t D_icode, uint64_t D_rB) {
    if (D_icode == IOPQ || D_icode == IRMMOVQ || D_icode == IMRMOVQ)
        return D_rB;
    else if (D_icode == IPUSHQ || D_icode == IPOPQ || D_icode == ICALL || D_icode == IRET)
        return RSP;
    return RNONE;
}

// Decide where to store results destination E
uint64_t DecodeStage::set_d_dstE(uint64_t D_icode, uint64_t D_rB) {
    if (D_icode == IRRMOVQ || D_icode == IIRMOVQ || D_icode == IOPQ)
        return D_rB;
    else if (D_icode == IPUSHQ || D_icode == IPOPQ || D_icode == ICALL || D_icode == IRET)
        return RSP;
    return RNONE;
}

// Decide where to store memory data destination M
uint64_t DecodeStage::set_d_dstM(uint64_t D_icode, uint64_t D_rA) {
    if (D_icode == IMRMOVQ || D_icode == IPOPQ)
        return D_rA;
    return RNONE;
}

// Returns valA to be forwarded
uint64_t DecodeStage::set_d_valA(uint64_t d_rvalA,E* ereg, 
    uint64_t d_srcA,M* mreg, W* wreg)
     { 
        uint64_t m_valE = mreg->getvalE()->getOutput();
        uint64_t w_dstE = wreg->getdstE()->getOutput();
        uint64_t e_dstE = ereg->getdstE()->getOutput();
        uint64_t e_valE = ereg->getvalC()->getOutput();
        uint64_t m_dstE = mreg->getdstE()->getOutput();
        uint64_t w_valE = wreg->getvalE()->getOutput();

    if (d_srcA == e_dstE)
        {
            return e_valE;
        }
    if (d_srcA == m_dstE)
        {
            return m_valE;
        }
    if (d_srcA == w_dstE)
        {
            return w_valE;
        }
    else
        {
            return d_rvalA;
        }
 }
//Returns valB to be forwarded
uint64_t DecodeStage::set_d_valB(uint64_t d_rvalB, uint64_t d_srcB, E* ereg, M* mreg, W* wreg){
        
        uint64_t m_valE = mreg->getvalE()->getOutput();
        uint64_t w_dstE = wreg->getdstE()->getOutput();
        uint64_t e_dstE = ereg->getdstE()->getOutput();
        uint64_t e_valE = ereg->getvalC()->getOutput();
        uint64_t m_dstE = mreg->getdstE()->getOutput();
        uint64_t w_valE = wreg->getvalE()->getOutput();

    if (d_srcB == e_dstE)
        {
            return e_valE;
        }
    if (d_srcB == m_dstE)
        {
            return m_valE;
        }
    if (d_srcB == w_dstE)
        {
            return w_valE;
        }
    else
        {
            return d_rvalB;
        }
    }