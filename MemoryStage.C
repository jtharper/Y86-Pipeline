#include "PipeReg.h"
#include "W.h"
#include "M.h"
#include "E.h"
#include "Stage.h"
#include "MemoryStage.h"

bool MemoryStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
    return false;
}
void MemoryStage::doClockHigh(PipeReg ** pregs)
{
}


