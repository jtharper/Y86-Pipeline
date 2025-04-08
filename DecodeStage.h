#include <cstdint>
#include "PipeRegField.h"
#include "Stage.h"
#ifndef DECODESTAGE_H
#define DECODESTAGE_H
class DecodeStage: public Stage{
   private:
   
      uint64_t set_d_valA(uint64_t d_rvalA,E* ereg,uint64_t d_srcA,M* mreg, W* wreg);
      uint64_t set_d_valB(uint64_t d_rvalB, uint64_t d_srcB, E* ereg, M* mreg, W* wreg);

      void setEInput(E *ereg, uint64_t stat, uint64_t icode, uint64_t ifun, uint64_t valC, uint64_t dstE, uint64_t dstM, uint64_t srcA, uint64_t srcB);
      uint64_t set_d_srcA(uint64_t D_icode, uint64_t D_rA);
      uint64_t set_d_srcB(uint64_t D_icode, uint64_t D_rB);

      uint64_t set_d_dstE(uint64_t D_icode, uint64_t D_rB);
      uint64_t set_d_dstM(uint64_t D_icode, uint64_t D_rA);
      uint64_t set_d_valA(uint64_t d_rvalA);
      uint64_t set_d_valB(uint64_t d_rvalB);
   public:
      bool doClockLow(PipeReg ** pregs, Stage ** stages);
      void doClockHigh(PipeReg ** pregs);
};
#endif