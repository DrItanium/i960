#include "Instruction.h"

namespace i960 {

EncodedInstruction
REGFormatInstruction::encode() const noexcept {
    /// @todo fix this
    return 0;
}

REGFormatInstruction::REGFormatInstruction(const DecodedInstruction& inst) : GenericFormatInstruction(inst), 
    _src1(0x1F & inst.getLowerHalf()), 
    _src2((inst.getLowerHalf() >> 14) & 0b11111),
    _srcDest((inst.getLowerHalf() >> 19) & 0b11111),
    _m1((inst.getLowerHalf() >> 11) & 1),
    _m2((inst.getLowerHalf() >> 12) & 1),
    _m3((inst.getLowerHalf() >> 13) & 1),
    _sf1((inst.getLowerHalf() >> 5) & 1),
    _sf2((inst.getLowerHalf() >> 6) & 1) { }

COBRFormatInstruction::COBRFormatInstruction(const DecodedInstruction& inst) : GenericFormatInstruction(inst),
    _source1((inst.getLowerHalf() >> 19) & 0b11111),
    _source2((inst.getLowerHalf() >> 14) & 0b11111),
    _m1((inst.getLowerHalf() >> 13) & 1),
    _displacement((inst.getLowerHalf() >> 2) & 0x3FF),
    _t(inst.getLowerHalf() & 2),
    _s2(inst.getLowerHalf() & 1) { }

CTRLFormatInstruction::CTRLFormatInstruction(const DecodedInstruction& inst) : GenericFormatInstruction(inst),
    _displacement((inst.getLowerHalf() >> 2) & 0x1FFFFF),
    _t(inst.getLowerHalf() & 2) { }

MEMFormatInstruction::MEMFormatInstruction(const DecodedInstruction& inst, byte mode) : GenericFormatInstruction(inst),
    _srcDest((inst.getLowerHalf() >> 19) & 0x1F),
    _abase((inst.getLowerHalf() >> 14) & 0x1F),
    _mode(mode) { }

} // end namespace i960
