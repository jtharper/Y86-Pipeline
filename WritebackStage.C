#include "PipeReg.h"
#include "W.h"
#include "Status.h"
#include "WritebackStage.h"
#include "Instructions.h"
#include "RegisterFile.h"
#include <ostream>
#include <iostream>

bool WritebackStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
     W * wreg = (W *) pregs[WREG];
     uint64_t w_stat= wreg->getstat()->getOutput();
     uint64_t w_icode= wreg->geticode()->getOutput(); 
     if(w_stat!=SAOK)
     {
        return true;
     }
     if(w_icode == IHALT)
     {
      return true;
     }
    return false;
}
void WritebackStage::doClockHigh(PipeReg ** pregs)
{
   W * wreg = (W *) pregs[WREG];
   bool haha = false;
   uint64_t W_valE = wreg->getvalE()->getOutput();
   uint64_t W_dstE = wreg->getdstE()->getOutput();
   

   RegisterFile::getInstance()->writeRegister(W_valE,W_dstE, haha);

}


