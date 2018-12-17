#ifndef I960_OPCODES_H__
#define I960_OPCODES_H__
#include "types.h"
namespace i960 {
    enum class Opcode : Ordinal {
#define o(name, code) name = code
#define c(baseName, baseAddress) \
		o(baseName ## e, (baseAddress | 0x0a0)), \
		o(baseName ## g, (baseAddress | 0x090)), \
		o(baseName ## ge, (baseAddress | 0x0b0)), \
		o(baseName ## l, (baseAddress | 0x0c0)), \
		o(baseName ## le, (baseAddress | 0x0e0)), \
		o(baseName ## ne, (baseAddress | 0x0d0)), \
		o(baseName ## no, (baseAddress | 0x080)), \
		o(baseName ## o, (baseAddress | 0x0f0))
		o(addc, 0x5b0),
		o(addi, 0x591),
		c(addi, 0x701),
		o(addo, 0x590),
		c(addo, 0x700),
		o(alterbit, 0x58f),
		o(opand, 0x581),
		o(andnot, 0x582),
		o(atadd, 0x612),
		o(atmod, 0x610),
		o(b, 0x08),
		o(bal, 0x0B),
		o(balx, 0x85),
		o(bbc, 0x30),
		o(bbs, 0x37),
		o(be, 0x12),
		o(bg, 0x11),
		o(bge, 0x13),
		o(bl, 0x14),
		o(ble, 0x16),
		o(bne, 0x15),
		o(bno, 0x10),
		o(bo, 0x17),
		o(bswap, 0x5ad),
		o(bx, 0x84),
		o(call, 0x09),
		o(calls, 0x660),
		o(callx, 0x86),
		o(chkbit, 0x5ae),
		o(clrbit, 0x58c),
		o(cmpdeci, 0x5a7),
		o(cmpdeco, 0x5a6),
		o(cmpi, 0x5a1),
		o(cmpib, 0x595),
		o(cmpibe, 0x3a),
		o(cmpibg, 0x39),
		o(cmpibge, 0x3b),
		o(cmpibl, 0x3c),
		o(cmpible, 0x3e),
		o(cmpibne, 0x3d),
		o(cmpibno, 0x38),
		o(cmpibo, 0x3f),
		o(cmpinci, 0x5a5),
		o(cmpinco, 0x5a4),
		o(cmpis, 0x597),
		o(cmpo, 0x5a0),
		o(cmpob, 0x594),
		o(cmpobe, 0x32),
		o(cmpobge, 0x33),
		o(cmpobl, 0x34),
		o(cmpoble, 0x36),
		o(cmpobne, 0x35),
		o(cmpos, 0x596),
		o(concmpi, 0x5a3),
		o(concmpo, 0x5a2),
		o(dcctl, 0x65c),
		o(divi, 0x74b),
		o(divo, 0x70b),
		o(ediv, 0x671),
		o(emul, 0x670),
		o(eshro, 0x5D8), // extended shift right ordinal
		o(extract, 0x651),
		o(faulte, 0x1a),
		o(faultg, 0x19),
		o(faultge, 0x1b),
		o(faultl, 0x1c),
		o(faultle, 0x1e),
		o(faultne, 0x1d),
		o(faultno, 0x18),
		o(faulto, 0x1f),
		o(flushreg, 0x66d),
		o(fmark, 0x66c),
		o(halt, 0x65d),
		o(icctl, 0x65b), // instruction cache control
		o(intctl, 0x658), // interrupt global enable and disable
		o(intdis, 0x5b4), // global interrupt disable
		o(inten, 0x5b5), // global interrupt enable
		o(ld, 0x90),
		o(lda, 0x8c),
		o(ldib, 0xc0),
		o(ldis, 0xc8),
		o(ldl, 0x98),
		o(ldob, 0x80),
		o(ldos, 0x88),
		o(ldq, 0xb0),
		o(ldt, 0xa0),
		o(mark, 0x66b),
		o(modac, 0x645),
		o(modi, 0x749),
		o(modify, 0x650),
		o(modpc, 0x655),
		o(modtc, 0x654),
		o(mov, 0x5cc),
		o(movl, 0x5dc), 
		o(movq, 0x5fc),
		o(movt, 0x5ec),
		o(muli, 0x741),
		o(mulo, 0x701),
		o(nand, 0x58e),
		o(nor, 0x588),
		o(opnot, 0x58a),
		o(notand, 0x584),
		o(notbit, 0x580),
		o(notor, 0x58d),
		o(opor, 0x587),
		o(ornot, 0x58b),
		o(remi, 0x748),
		o(remo, 0x708),
		o(ret, 0x0a),
		o(rotate, 0x59d),
		o(scanbit, 0x641),
		o(scanbyte, 0x5ac),
		c(sel, 0x704), // select based on ...
		o(setbit, 0x583),
		o(shli, 0x59e),
		o(shlo, 0x59c),
		o(shrdi, 0x59a),
		o(shri, 0x59b),
		o(shro, 0x598),
		o(spanbit, 0x640),
		o(st, 0x92),
		o(stib, 0xc2),
		o(stis, 0xcA),
		o(stl, 0x9a),
		o(stob, 0x82),
		o(stos, 0x8a),
		o(stq, 0xb2),
		o(stt, 0xa2),
		o(subc, 0x5b2),
		o(subi, 0x593),
		c(subi, 0x703),
		o(subo, 0x592),
		c(subo, 0x702),
		o(syncf, 0x66f),
		o(sysctl, 0x659),
		o(teste, 0x22),
		o(testg, 0x21),
		o(testge, 0x23),
		o(testl, 0x24),
		o(testle, 0x26),
		o(testne, 0x25),
		o(testno, 0x20),
		o(testo, 0x27),
		o(xnor, 0x589),
		o(opxor, 0x586),
#undef c
#undef o
    };
	constexpr Ordinal decode(Opcode code) noexcept {
		return static_cast<Ordinal>(code);
	}
} // end namespace i960
#endif // end I960_OPCODES_H__
