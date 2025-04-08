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
      uint64_t aluA(uint64_t e_icode,uint64_t e_valA,uint64_t e_valC);
      uint64_t aluB(uint64_t e_icode,uint64_t e_valB);
      uint64_t alufun(uint64_t e_icode,uint64_t e_ifun);
      bool set_cc(uint64_t e_icode);
      uint64_t e_dstE(uint64_t e_icode,uint64_t e_cnd);
      void CC(bool _ZF, bool _SF, bool _OF);
      uint64_t alu(uint64_t aluA,uint64_t aluB,uint64_t alufun, bool setCC);
      

   public:
      bool doClockLow(PipeReg ** pregs, Stage ** stages);
      void doClockHigh(PipeReg ** pregs);

      void setMInput(M *mreg, uint64_t stat, uint64_t icode, uint64_t valA, uint64_t valE, uint64_t dstE, uint64_t dstM, uint64_t e_cnd);

     
};
#endif