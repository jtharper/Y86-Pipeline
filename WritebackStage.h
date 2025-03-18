#include <cstdint>
#include "Stage.h"
#include "PipeReg.h"
#include "PipeRegField.h"

#ifndef WRITEBACKSTAGE_H
#define WRITEBACKSTAGE_H

class WritebackStage: public Stage
{
   private:
      
   public:
      bool doClockLow(PipeReg ** pregs, Stage ** stages);
      void doClockHigh(PipeReg ** pregs);

};
#endif