#include "Instruction.h"

namespace i960 {

REGFormatInstruction::REGFormatInstruction(const DecodedInstruction& inst) : Base(inst), _src1(inst.getSrc1()), 
    _src2(inst.getSrc2()), _srcDest(inst.getSrcDest()), 
    _m1(inst.getM1()),
    _m2(inst.getM2()),
    _m3(inst.getM3()) { }

} // end namespace i960
