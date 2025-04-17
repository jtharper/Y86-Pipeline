#include "PipeReg.h"
#include "M.h"
#include "E.h"
#include "ExecuteStage.h"
#include "Instructions.h"
#include "RegisterFile.h"
#include "ConditionCodes.h"
#include "Tools.h"
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
    
    uint64_t ifun = ereg->getifun()->getOutput();

    uint64_t e_cnd = 0;  // default condition flag
    uint64_t E_valC = ereg->getvalC()->getOutput();
    uint64_t A = aluA(icode, valA, E_valC);
    uint64_t B = aluB(icode, valB);
    uint64_t fn = alufun(icode, ifun);
    bool docc = set_cc(icode);
    uint64_t result = alu(A,B,fn,docc);

    e_valE = result;
    e_dstE = dstE;
   
    // Pass the computed values to Memory stage (M register)
    setMInput(mreg, stat, icode, valA, e_valE, e_dstE, dstM, e_cnd);

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
//Pass valA or valC depending on instruction
uint64_t ExecuteStage::aluA(uint64_t e_icode,uint64_t e_valA,uint64_t e_valC)
{
    if (e_icode == IRRMOVQ || e_icode == IOPQ)
    {
        return e_valA;
    }
    else if (e_icode == IIRMOVQ || e_icode == IRMMOVQ || e_icode == IMRMOVQ)
    {
        return e_valC;
    }
    else if (e_icode == ICALL || e_icode == IPUSHQ)
    {
        return -8;
    }
    else if (e_icode == IRET || e_icode == IPOPQ)
    {
        return 8;
    }
    else
    {
        return 0;
    }

}
//Return valB or zero depending on instruction
uint64_t ExecuteStage::aluB(uint64_t e_icode,uint64_t e_valB)
{
    if (e_icode == IRMMOVQ || e_icode == IMRMOVQ || e_icode == IOPQ || e_icode == ICALL || e_icode == IPUSHQ || e_icode == IRET || e_icode == IPOPQ)
    {
        return e_valB;
    }
    else if (e_icode == IRRMOVQ || e_icode == IIRMOVQ)
    {
        return 0;
    }
    else
    {
        return 0;
    }
}
//Returns correct fun based off of icode
uint64_t ExecuteStage::alufun(uint64_t e_icode,uint64_t e_ifun)
{
    if(e_icode == IOPQ)
    {
        return e_ifun;
    }
    else
    {
        return ADDQ;
    }
}
//Returns true if condition codes need to be set
bool ExecuteStage::set_cc(uint64_t e_icode)
{
    return (e_icode == IOPQ);
}

//Takes flags from alu and sets condition codes if need be
void ExecuteStage::CC(bool _ZF, bool _SF, bool _OF)
{
    bool error = false;
    ConditionCodes::getInstance()->setConditionCode(_ZF,ZF, error);
    ConditionCodes::getInstance()->setConditionCode(_SF,SF, error);
    ConditionCodes::getInstance()->setConditionCode(_OF,OF, error);
}
//Determines opertation to perform. Returns a value and sets condition codes
uint64_t ExecuteStage::alu(uint64_t aluA,uint64_t aluB,uint64_t alufun, bool setCC)
{
    bool toOverflow;
    uint64_t returnValue;
    switch (alufun)
    {
        case ADDQ:
            toOverflow = Tools::addOverflow(aluA, aluB);
            returnValue = aluA + aluB;
            break;
        
        case XORQ:
            returnValue = aluA ^ aluB;
            break;
        
        case SUBQ:
            toOverflow = Tools::subOverflow(aluA, aluB);
            returnValue = aluB - aluA;
            break;
        
        case ANDQ:
            returnValue = aluA & aluB;
            break;

        default:
            break;
    } 
    if(setCC)  
    {
        bool signflag = Tools::sign(returnValue);
        bool zeroflag = (returnValue == 0);
        CC(zeroflag, signflag, toOverflow);
    }
return returnValue;
        
}