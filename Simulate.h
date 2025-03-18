#ifndef SIMULATE_H
#define SIMULATE_H
//Driver class for the yess simulator
class Simulate
{
   private:
      PipeReg ** pregs;
      Stage ** stages;
   public:
      Simulate();
      void run();
      bool doClockLow();
      void doClockHigh();
      void dumpPipeRegs();
};
#endif