#include <cstdint>
#include "PipeRegField.h"
#include "Stage.h"
#ifndef DECODESTAGE_H
#define DECODESTAGE_H
class DecodeStage: public Stage{
   private:
      void setEInput(PipeReg * dreg, uint64_t stat, uint64_t icode, uint64_t ifun, 
                     uint64_t rA, uint64_t rB,
                     uint64_t valC, uint64_t valP);
   public:
      bool doClockLow(PipeReg ** pregs, Stage ** stages);
      void doClockHigh(PipeReg ** pregs);

      void setEInput(E *ereg, uint64_t stat, uint64_t icode, uint64_t ifun, uint64_t valC, uint64_t dstE, uint64_t dstM, uint64_t srcA, uint64_t srcB);
};
#endif