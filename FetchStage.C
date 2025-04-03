#include <string>
#include <cstdint>
#include "RegisterFile.h"
#include "PipeRegField.h"
#include "PipeReg.h"
#include "F.h"
#include "D.h"
#include "M.h"
#include "W.h"
#include "Stage.h"
#include "FetchStage.h"
#include "Status.h"
#include "Debug.h"
#include "Instructions.h"
#include "Memory.h"
#include "Tools.h"
#include <ostream>
#include <iostream>

/*
 * doClockLow:
 * Performs the Fetch stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pregs - array of the pipeline register sets (F, D, E, M, W instances)
 * @param: stages - array of stages (FetchStage, DecodeStage, ExecuteStage,
 *         MemoryStage, WritebackStage instances)
 */
bool FetchStage::doClockLow(PipeReg **pregs, Stage **stages)
{
    F *freg = (F *)pregs[FREG];
    D *dreg = (D *)pregs[DREG];
   
    uint64_t icode = INOP, ifun = FNONE;
    uint64_t rA = RNONE, rB = RNONE, valC = 0, valP = 0, stat = SAOK;

    // Fetch the instruction byte
    uint64_t f_pc = selectPC((F *)pregs[FREG], (M *)pregs[MREG], (W *)pregs[WREG]);

    bool mem_error = false;
    uint8_t instruction = Memory::getInstance()->getByte(f_pc, mem_error);
    if (mem_error) {
        stat = 1; // Assuming address error
        return false;
    }
  ifun = Tools::getBits(instruction,0,3);  // Extract ifun
  icode = Tools::getBits(instruction,4,7); // Extract icode
    

    

    
    // Fetch rA and rB if needed
    if (needRegIds(icode)) {
        
        getRegIds(&rA,&rB,f_pc);
       
    }

    if(need_valC(icode))
    {
        valC = buildValC(f_pc,needRegIds(icode));
       
    }

    // Skip fetching valC; use default value of 0
    

    // Calculate valP using PCincrement
    valP = PCincrement(f_pc, needRegIds(icode), need_valC(icode)); // Set `needValC` to `false`

    // Predict next PC
    uint64_t f_predPC = predictPC(icode, valP, valC);
    freg->getpredPC()->setInput(f_predPC);

    // Pass inputs to D register
    setDInput(dreg, stat, icode, ifun, rA, rB, valC, valP);

    return false;
}



/* doClockHigh
 * applies the appropriate control signal to the F
 * and D register intances
 *
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
 */
void FetchStage::doClockHigh(PipeReg ** pregs)
{
   F * freg = (F *) pregs[FREG];
   D * dreg = (D *) pregs[DREG];

   freg->getpredPC()->normal();
   dreg->getstat()->normal();
   dreg->geticode()->normal();
   dreg->getifun()->normal();
   dreg->getrA()->normal();
   dreg->getrB()->normal();
   dreg->getvalC()->normal();
   dreg->getvalP()->normal();
}

/* setDInput
 * provides the input to potentially be stored in the D register
 * during doClockHigh
 *
 * @param: dreg - pointer to the D register instance
 * @param: stat - value to be stored in the stat pipeline register within D
 * @param: icode - value to be stored in the icode pipeline register within D
 * @param: ifun - value to be stored in the ifun pipeline register within D
 * @param: rA - value to be stored in the rA pipeline register within D
 * @param: rB - value to be stored in the rB pipeline register within D
 * @param: valC - value to be stored in the valC pipeline register within D
 * @param: valP - value to be stored in the valP pipeline register within D
*/
void FetchStage::setDInput(D * dreg, uint64_t stat, uint64_t icode, 
                           uint64_t ifun, uint64_t rA, uint64_t rB,
                           uint64_t valC, uint64_t valP)
{
   dreg->getstat()->setInput(stat);
   dreg->geticode()->setInput(icode);
   dreg->getifun()->setInput(ifun);
   dreg->getrA()->setInput(rA);
   dreg->getrB()->setInput(rB);
   dreg->getvalC()->setInput(valC);
   dreg->getvalP()->setInput(valP);
}


bool FetchStage::needRegIds(uint64_t f_icode)
{
	return (f_icode == IRRMOVQ || f_icode == IOPQ || f_icode == IPUSHQ || f_icode == IPOPQ || f_icode == IIRMOVQ || f_icode == IRMMOVQ || f_icode == IMRMOVQ);
}

bool FetchStage::need_valC(uint64_t f_icode)
{
   return (f_icode == IIRMOVQ || f_icode ==IJXX || f_icode==ICALL || f_icode ==IRMMOVQ || f_icode ==IRMMOVQ);
} 

//predictPC method: inputs are f_icode, f_valC, f_valP

uint64_t FetchStage:: predictPC(uint64_t f_icode,uint64_t f_valP,uint64_t f_valC)
{
   if (f_icode == IJXX || f_icode == ICALL) {
    return f_valC;
} else {
    return f_valP;
}
}


//selectPC method: input is F, M, and W registers

uint64_t FetchStage::selectPC(F* freg, M* mreg, W* wreg)
{
    uint64_t M_icode = mreg->geticode()->getOutput();
    uint64_t M_cnd = mreg->getCnd()->getOutput();
    uint64_t W_icode = wreg->geticode()->getOutput();

    if (M_icode == IJXX && !M_cnd)
    {
        return mreg->getvalA()->getOutput(); // Corrected spelling
    }
    else if (W_icode == IRET)
    {
        return wreg->getvalM()->getOutput(); // Corrected spelling
    }
    return freg->getpredPC()->getOutput();  
}

uint64_t FetchStage::PCincrement(uint64_t f_pc, bool regIdResult, bool valCr)
{
   if(valCr)
   {
      f_pc = f_pc + 8;
   }
   if(regIdResult)
   {
      f_pc = f_pc + 1;
   }
   f_pc = f_pc+1;
   return f_pc;
}

void FetchStage::getRegIds(uint64_t * rA, uint64_t * rB, uint64_t f_pc){

    bool figures = false;
    uint64_t byte = Memory::getInstance()->getByte(f_pc + 1, figures);
    uint64_t RB = Tools::getBits(byte,0,3);
    uint64_t RA = Tools::getBits(byte,4,7);
    *rB = RB;
    *rA= RA;

}

uint64_t FetchStage::buildValC(uint64_t f_pc, bool needRegID)
{
uint8_t valC[8]; // Ensure the array is correctly initialized
bool error = false;
f_pc++; 

if (needRegID) {
    f_pc++;
}

// Fetch 8 bytes into valC
for (int i = 0; i < 8; i++) {
   valC[i] = Memory::getInstance()->getByte(f_pc + i, error);
    if (error) return 0; // Handle memory access errors properly
}

// Convert the byte array into a 64-bit integer using buildLong
uint64_t result = Tools::buildLong(valC);


return result;

}