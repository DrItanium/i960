(deftemplate opcode
(slot opcode (type LEXEME) (default ?NONE))
(slot class (type LEXEME) (default ?NONE))
(slot id (type LEXEME) (default ?NONE))
)
(deffacts i960kb-ops
(opcode (opcode 0xOA) (class CTRL) (id ret))
(opcode (opcode 0xOB) (class CTRL) (id bal))
(opcode (opcode 0x10) (class CTRL) (id bno))
(opcode (opcode 0x11) (class CTRL) (id bg))
(opcode (opcode 0x12) (class CTRL) (id be))
(opcode (opcode 0x13) (class CTRL) (id bge))
(opcode (opcode 0x14) (class CTRL) (id bl))
(opcode (opcode 0x15) (class CTRL) (id bne))
(opcode (opcode 0x16) (class CTRL) (id ble))
(opcode (opcode 0x17) (class CTRL) (id bo))
(opcode (opcode 0x18) (class CTRL) (id faultno))
(opcode (opcode 0x19) (class CTRL) (id faultg))
(opcode (opcode 0x1A) (class CTRL) (id faulte))
(opcode (opcode 0x1B) (class CTRL) (id faultge))
(opcode (opcode 0x1C) (class CTRL) (id faultl))
(opcode (opcode 0x10) (class CTRL) (id faultne))
(opcode (opcode 0x1E) (class CTRL) (id faultle))
(opcode (opcode 0x1F) (class CTRL) (id faulto))
(opcode (opcode 0x20) (class COBR) (id testno))
(opcode (opcode 0x21) (class COBR) (id testg))
(opcode (opcode 0x22) (class COBR) (id teste))
(opcode (opcode 0x23) (class COBR) (id testge))
(opcode (opcode 0x24) (class COBR) (id testl))
(opcode (opcode 0x25) (class COBR) (id testne))
(opcode (opcode 0x26) (class COBR) (id testle))
(opcode (opcode 0x27) (class COBR) (id testo))
(opcode (opcode 0x30) (class COBR) (id bbc))
(opcode (opcode 0x31) (class COBR) (id cmpobg))
(opcode (opcode 0x32) (class COBR) (id cmpobe))
(opcode (opcode 0x33) (class COBR) (id cmpobge))
(opcode (opcode 0x34) (class COBR) (id cmpobl))
(opcode (opcode 0x35) (class COBR) (id cmpobne))
(opcode (opcode 0x36) (class COBR) (id cmpoble))
(opcode (opcode 0x37) (class COBR) (id bbs))
(opcode (opcode 0x38) (class COBR) (id cmpibno))
(opcode (opcode 0x39) (class COBR) (id cmpibg))
(opcode (opcode 0x3A) (class COBR) (id cmpibe))
(opcode (opcode 0x3B) (class COBR) (id cmpibge))
(opcode (opcode 0x3C) (class COBR) (id cmpibl))
(opcode (opcode 0x3D) (class COBR) (id cmpibne))
(opcode (opcode 0x3E) (class COBR) (id cmpible))
(opcode (opcode 0x3F) (class COBR) (id cmpibo))
(opcode (opcode 0x80) (class MEM) (id Idob))
(opcode (opcode 0x84) (class MEM) (id bx))
(opcode (opcode 0x85) (class MEM) (id balx))
(opcode (opcode 0x86) (class MEM) (id calix))
(opcode (opcode 0x88) (class MEM) (id ldos))
(opcode (opcode 0x8A) (class MEM) (id stos))
(opcode (opcode 0x8C) (class MEM) (id lda))
(opcode (opcode 0x90) (class MEM) (id ld))
(opcode (opcode 0x92) (class MEM) (id st))
(opcode (opcode 0x98) (class MEM) (id Idl))
(opcode (opcode 0x9A) (class MEM) (id stl))
(opcode (opcode 0xA0) (class MEM) (id Idt))
(opcode (opcode 0xA2) (class MEM) (id stt))
(opcode (opcode 0xB0) (class MEM) (id Idq))
(opcode (opcode 0xB2) (class MEM) (id stq))
(opcode (opcode 0xC0) (class MEM) (id Idib))
(opcode (opcode 0xC2) (class MEM) (id stib))
(opcode (opcode 0xC8) (class MEM) (id ldis))
(opcode (opcode 0xCA) (class MEM) (id stis))
(opcode (opcode 0x580) (class REG) (id notbit))
(opcode (opcode 0x581) (class REG) (id and))
(opcode (opcode 0x582) (class REG) (id andnot))
(opcode (opcode 0x583) (class REG) (id setbit))
(opcode (opcode 0x584) (class REG) (id notand))
(opcode (opcode 0x586) (class REG) (id xor))
(opcode (opcode 0x587) (class REG) (id or))
(opcode (opcode 0x588) (class REG) (id nor))
(opcode (opcode 0x589) (class REG) (id xnor))
(opcode (opcode 0x58A) (class REG) (id not))
(opcode (opcode 0x58B) (class REG) (id ornot))
(opcode (opcode 0x58C) (class REG) (id clrbit))
(opcode (opcode 0x58D) (class REG) (id notor))
(opcode (opcode 0x58E) (class REG) (id nand))
(opcode (opcode 0x58F) (class REG) (id alterbit))
(opcode (opcode 0x590) (class REG) (id addo))
(opcode (opcode 0x591) (class REG) (id addi))
(opcode (opcode 0x592) (class REG) (id subo))
(opcode (opcode 0x593) (class REG) (id subi))
(opcode (opcode 0x598) (class REG) (id shro))
(opcode (opcode 0x59A) (class REG) (id shrdi))
(opcode (opcode 0x59B) (class REG) (id shri))
(opcode (opcode 0x59C) (class REG) (id shlo))
(opcode (opcode 0x59D) (class REG) (id rotate))
(opcode (opcode 0x59E) (class REG) (id shli))
(opcode (opcode 0x5A0) (class REG) (id cmpo))
(opcode (opcode 0x5A1) (class REG) (id cmpi))
(opcode (opcode 0x5A2) (class REG) (id concmpo))
(opcode (opcode 0x5A3) (class REG) (id concmpi))
(opcode (opcode 0x5A4) (class REG) (id cmpinco))
(opcode (opcode 0x5A5) (class REG) (id cmpinci))
(opcode (opcode 0x5A6) (class REG) (id cmpdeco))
(opcode (opcode 0x5A7) (class REG) (id cmpdeci))
(opcode (opcode 0x5AC) (class REG) (id scanbyte))
(opcode (opcode 0x5AE) (class REG) (id chkbit))
(opcode (opcode 0x5BO) (class REG) (id addc))
(opcode (opcode 0x5B2) (class REG) (id subc))
(opcode (opcode 0x5CC) (class REG) (id mov))
(opcode (opcode 0x5DC) (class REG) (id movl))
(opcode (opcode 0x5EC) (class REG) (id movt))
(opcode (opcode 0x5FC) (class REG) (id movq))
(opcode (opcode 0x600) (class REG) (id synmov))
(opcode (opcode 0x601) (class REG) (id synmovi))
(opcode (opcode 0x602) (class REG) (id synmovq))
(opcode (opcode 0x610) (class REG) (id atmod))
(opcode (opcode 0x612) (class REG) (id atadd))
(opcode (opcode 0x615) (class REG) (id synld))
(opcode (opcode 0x640) (class REG) (id spanbit))
(opcode (opcode 0x641) (class REG) (id scanbit))
(opcode (opcode 0x642) (class REG) (id daddc))
(opcode (opcode 0x643) (class REG) (id dsubc))
(opcode (opcode 0x644) (class REG) (id dmovt))
(opcode (opcode 0x645) (class REG) (id modac))
(opcode (opcode 0x650) (class REG) (id modify))
(opcode (opcode 0x651) (class REG) (id extract))
(opcode (opcode 0x654) (class REG) (id modtc))
(opcode (opcode 0x655) (class REG) (id modpc))
(opcode (opcode 0x660) (class REG) (id calls))
(opcode (opcode 0x66B) (class REG) (id mark))
(opcode (opcode 0x66C) (class REG) (id fmark))
(opcode (opcode 0x66D) (class REG) (id flushreg))
(opcode (opcode 0x66F) (class REG) (id syncf))
(opcode (opcode 0x670) (class REG) (id emul))
(opcode (opcode 0x671) (class REG) (id ediv))
(opcode (opcode 0x674) (class REG) (id cvtir))
(opcode (opcode 0x675) (class REG) (id cvtilr))
(opcode (opcode 0x676) (class REG) (id scalerl))
(opcode (opcode 0x677) (class REG) (id scaler))
(opcode (opcode 0x680) (class REG) (id atanr))
(opcode (opcode 0x681) (class REG) (id logepr))
(opcode (opcode 0x682) (class REG) (id logr))
(opcode (opcode 0x683) (class REG) (id remr))
(opcode (opcode 0x684) (class REG) (id cmpor))
(opcode (opcode 0x685) (class REG) (id cmpr))
(opcode (opcode 0x688) (class REG) (id sqrtr))
(opcode (opcode 0x68A) (class REG) (id logbnr))
(opcode (opcode 0x68B) (class REG) (id roundr))
(opcode (opcode 0x68C) (class REG) (id sinr))
(opcode (opcode 0x68D) (class REG) (id cosr))
(opcode (opcode 0x68E) (class REG) (id tanr))
(opcode (opcode 0x68F) (class REG) (id classr))
(opcode (opcode 0x690) (class REG) (id atanrl))
(opcode (opcode 0x691) (class REG) (id logeprl))
(opcode (opcode 0x692) (class REG) (id logrl))
(opcode (opcode 0x693) (class REG) (id remrl))
(opcode (opcode 0x694) (class REG) (id cmporl))
(opcode (opcode 0x695) (class REG) (id cmprl))
(opcode (opcode 0x698) (class REG) (id sqrtrl))
(opcode (opcode 0x699) (class REG) (id exprl))
(opcode (opcode 0x69A) (class REG) (id logbnrl))
(opcode (opcode 0x69B) (class REG) (id roundrl))
(opcode (opcode 0x69C) (class REG) (id sinrl))
(opcode (opcode 0x69D) (class REG) (id cosrl))
(opcode (opcode 0x69E) (class REG) (id tanrl))
(opcode (opcode 0x69F) (class REG) (id classrl))
(opcode (opcode 0x6C0) (class REG) (id cvtri))
(opcode (opcode 0x6C1) (class REG) (id cvtril))
(opcode (opcode 0x6C2) (class REG) (id cvtzri))
(opcode (opcode 0x6C3) (class REG) (id cvtzril))
(opcode (opcode 0x6C9) (class REG) (id movr))
(opcode (opcode 0x6D9) (class REG) (id movrl))
(opcode (opcode 0x6E2) (class REG) (id cpysre))
(opcode (opcode 0x6E3) (class REG) (id cpyrsre))
(opcode (opcode 0x6E9) (class REG) (id movre))
(opcode (opcode 0x701) (class REG) (id mulo))
(opcode (opcode 0x708) (class REG) (id remo))
(opcode (opcode 0x70B) (class REG) (id divo))
(opcode (opcode 0x741) (class REG) (id muli))
(opcode (opcode 0x748) (class REG) (id remi))
(opcode (opcode 0x749) (class REG) (id modi))
(opcode (opcode 0x74B) (class REG) (id divi))
(opcode (opcode 0x78B) (class REG) (id divr))
(opcode (opcode 0x78C) (class REG) (id mulr))
(opcode (opcode 0x78D) (class REG) (id subr))
(opcode (opcode 0x78F) (class REG) (id addr))
(opcode (opcode 0x79B) (class REG) (id divrl))
(opcode (opcode 0x79C) (class REG) (id mulrl))
(opcode (opcode 0x79D) (class REG) (id subrl))
(opcode (opcode 0x79F) (class REG) (id addrl))
)
