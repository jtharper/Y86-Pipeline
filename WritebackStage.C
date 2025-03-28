#include "PipeReg.h"
#include "W.h"
#include "Status.h"
#include "WritebackStage.h"
#include "Instructions.h"

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
}


