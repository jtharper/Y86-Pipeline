#include "PipeReg.h"
#include "W.h"
#include "Status.h"
#include "WritebackStage.h"

bool WritebackStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
    return false;
}
void WritebackStage::doClockHigh(PipeReg ** pregs)
{
}


