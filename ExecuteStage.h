#include <cstdint>
#include "Stage.h"
#include "PipeReg.h"

#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H
class ExecuteStage: public Stage
{
   private:
      void setMInput(PipeReg * reg, uint64_t stat, uint64_t icode, uint64_t ifun, 
                     uint64_t rA, uint64_t rB,
                     uint64_t valC, uint64_t valP);
   public:
      bool doClockLow(PipeReg ** pregs, Stage ** stages);
      void doClockHigh(PipeReg ** pregs);

      void setMinput(M * mreg, uint64_t stat, uint64_t icode, uint64_t valA, uint64_t valE, uint64_t dstE, uint64_t dstM, uint64_t e_cnd);
};
#endif