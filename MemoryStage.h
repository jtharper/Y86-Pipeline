#include <cstdint>
#include "PipeReg.h"

#ifndef MEMORYSTAGE_H
#define MEMORYSTAGE_H

class MemoryStage: public Stage
{
   private:
      void setWInput(PipeReg * wreg, uint64_t stat, uint64_t icode, uint64_t ifun, 
                     uint64_t rA, uint64_t rB,
                     uint64_t valC, uint64_t valP);
   public:
      bool doClockLow(PipeReg ** pregs, Stage ** stages);
      void doClockHigh(PipeReg ** pregs);

      void setWinput(W *wreg, uint64_t stat, uint64_t icode, uint64_t valE, uint64_t dstE, uint64_t dstM, uint64_t valM);
      uint64_t mem_addr(uint64_t m_icode, uint64_t m_valE, uint64_t m_valA);
      bool mem_read(uint64_t m_icode);
      bool mem_write(uint64_t m_icode);
};
#endif