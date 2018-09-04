(deftemplate kbopcode
(slot opcode (type LEXEME) (default ?NONE))
(slot class (type LEXEME) (default ?NONE))
(slot id (type LEXEME) (default ?NONE))
)
(deffacts i960kb-ops
(kbopcode (opcode 0xOA) (class CTRL) (id ret))
(kbopcode (opcode 0xOB) (class CTRL) (id bal))
(kbopcode (opcode 0x10) (class CTRL) (id bno))
(kbopcode (opcode 0x11) (class CTRL) (id bg))
(kbopcode (opcode 0x12) (class CTRL) (id be))
(kbopcode (opcode 0x13) (class CTRL) (id bge))
(kbopcode (opcode 0x14) (class CTRL) (id bl))
(kbopcode (opcode 0x15) (class CTRL) (id bne))
(kbopcode (opcode 0x16) (class CTRL) (id ble))
(kbopcode (opcode 0x17) (class CTRL) (id bo))
(kbopcode (opcode 0x18) (class CTRL) (id faultno))
(kbopcode (opcode 0x19) (class CTRL) (id faultg))
(kbopcode (opcode 0x1A) (class CTRL) (id faulte))
(kbopcode (opcode 0x1B) (class CTRL) (id faultge))
(kbopcode (opcode 0x1C) (class CTRL) (id faultl))
(kbopcode (opcode 0x10) (class CTRL) (id faultne))
(kbopcode (opcode 0x1E) (class CTRL) (id faultle))
(kbopcode (opcode 0x1F) (class CTRL) (id faulto))
(kbopcode (opcode 0x20) (class COBR) (id testno))
(kbopcode (opcode 0x21) (class COBR) (id testg))
(kbopcode (opcode 0x22) (class COBR) (id teste))
(kbopcode (opcode 0x23) (class COBR) (id testge))
(kbopcode (opcode 0x24) (class COBR) (id testl))
(kbopcode (opcode 0x25) (class COBR) (id testne))
(kbopcode (opcode 0x26) (class COBR) (id testle))
(kbopcode (opcode 0x27) (class COBR) (id testo))
(kbopcode (opcode 0x30) (class COBR) (id bbc))
(kbopcode (opcode 0x31) (class COBR) (id cmpobg))
(kbopcode (opcode 0x32) (class COBR) (id cmpobe))
(kbopcode (opcode 0x33) (class COBR) (id cmpobge))
(kbopcode (opcode 0x34) (class COBR) (id cmpobl))
(kbopcode (opcode 0x35) (class COBR) (id cmpobne))
(kbopcode (opcode 0x36) (class COBR) (id cmpoble))
(kbopcode (opcode 0x37) (class COBR) (id bbs))
(kbopcode (opcode 0x38) (class COBR) (id cmpibno))
(kbopcode (opcode 0x39) (class COBR) (id cmpibg))
(kbopcode (opcode 0x3A) (class COBR) (id cmpibe))
(kbopcode (opcode 0x3B) (class COBR) (id cmpibge))
(kbopcode (opcode 0x3C) (class COBR) (id cmpibl))
(kbopcode (opcode 0x3D) (class COBR) (id cmpibne))
(kbopcode (opcode 0x3E) (class COBR) (id cmpible))
(kbopcode (opcode 0x3F) (class COBR) (id cmpibo))
(kbopcode (opcode 0x80) (class MEM) (id Idob))
(kbopcode (opcode 0x84) (class MEM) (id bx))
(kbopcode (opcode 0x85) (class MEM) (id balx))
(kbopcode (opcode 0x86) (class MEM) (id calix))
(kbopcode (opcode 0x88) (class MEM) (id ldos))
(kbopcode (opcode 0x8A) (class MEM) (id stos))
(kbopcode (opcode 0x8C) (class MEM) (id lda))
(kbopcode (opcode 0x90) (class MEM) (id ld))
(kbopcode (opcode 0x92) (class MEM) (id st))
(kbopcode (opcode 0x98) (class MEM) (id Idl))
(kbopcode (opcode 0x9A) (class MEM) (id stl))
(kbopcode (opcode 0xA0) (class MEM) (id Idt))
(kbopcode (opcode 0xA2) (class MEM) (id stt))
(kbopcode (opcode 0xB0) (class MEM) (id Idq))
(kbopcode (opcode 0xB2) (class MEM) (id stq))
(kbopcode (opcode 0xC0) (class MEM) (id Idib))
(kbopcode (opcode 0xC2) (class MEM) (id stib))
(kbopcode (opcode 0xC8) (class MEM) (id ldis))
(kbopcode (opcode 0xCA) (class MEM) (id stis))
(kbopcode (opcode 0x580) (class REG) (id notbit))
(kbopcode (opcode 0x581) (class REG) (id and))
(kbopcode (opcode 0x582) (class REG) (id andnot))
(kbopcode (opcode 0x583) (class REG) (id setbit))
(kbopcode (opcode 0x584) (class REG) (id notand))
(kbopcode (opcode 0x586) (class REG) (id xor))
(kbopcode (opcode 0x587) (class REG) (id or))
(kbopcode (opcode 0x588) (class REG) (id nor))
(kbopcode (opcode 0x589) (class REG) (id xnor))
(kbopcode (opcode 0x58A) (class REG) (id not))
(kbopcode (opcode 0x58B) (class REG) (id ornot))
(kbopcode (opcode 0x58C) (class REG) (id clrbit))
(kbopcode (opcode 0x58D) (class REG) (id notor))
(kbopcode (opcode 0x58E) (class REG) (id nand))
(kbopcode (opcode 0x58F) (class REG) (id alterbit))
(kbopcode (opcode 0x590) (class REG) (id addo))
(kbopcode (opcode 0x591) (class REG) (id addi))
(kbopcode (opcode 0x592) (class REG) (id subo))
(kbopcode (opcode 0x593) (class REG) (id subi))
(kbopcode (opcode 0x598) (class REG) (id shro))
(kbopcode (opcode 0x59A) (class REG) (id shrdi))
(kbopcode (opcode 0x59B) (class REG) (id shri))
(kbopcode (opcode 0x59C) (class REG) (id shlo))
(kbopcode (opcode 0x59D) (class REG) (id rotate))
(kbopcode (opcode 0x59E) (class REG) (id shli))
(kbopcode (opcode 0x5A0) (class REG) (id cmpo))
(kbopcode (opcode 0x5A1) (class REG) (id cmpi))
(kbopcode (opcode 0x5A2) (class REG) (id concmpo))
(kbopcode (opcode 0x5A3) (class REG) (id concmpi))
(kbopcode (opcode 0x5A4) (class REG) (id cmpinco))
(kbopcode (opcode 0x5A5) (class REG) (id cmpinci))
(kbopcode (opcode 0x5A6) (class REG) (id cmpdeco))
(kbopcode (opcode 0x5A7) (class REG) (id cmpdeci))
(kbopcode (opcode 0x5AC) (class REG) (id scanbyte))
(kbopcode (opcode 0x5AE) (class REG) (id chkbit))
(kbopcode (opcode 0x5BO) (class REG) (id addc))
(kbopcode (opcode 0x5B2) (class REG) (id subc))
(kbopcode (opcode 0x5CC) (class REG) (id mov))
(kbopcode (opcode 0x5DC) (class REG) (id movl))
(kbopcode (opcode 0x5EC) (class REG) (id movt))
(kbopcode (opcode 0x5FC) (class REG) (id movq))
(kbopcode (opcode 0x600) (class REG) (id synmov))
(kbopcode (opcode 0x601) (class REG) (id synmovi))
(kbopcode (opcode 0x602) (class REG) (id synmovq))
(kbopcode (opcode 0x610) (class REG) (id atmod))
(kbopcode (opcode 0x612) (class REG) (id atadd))
(kbopcode (opcode 0x615) (class REG) (id synld))
(kbopcode (opcode 0x640) (class REG) (id spanbit))
(kbopcode (opcode 0x641) (class REG) (id scanbit))
(kbopcode (opcode 0x642) (class REG) (id daddc))
(kbopcode (opcode 0x643) (class REG) (id dsubc))
(kbopcode (opcode 0x644) (class REG) (id dmovt))
(kbopcode (opcode 0x645) (class REG) (id modac))
(kbopcode (opcode 0x650) (class REG) (id modify))
(kbopcode (opcode 0x651) (class REG) (id extract))
(kbopcode (opcode 0x654) (class REG) (id modtc))
(kbopcode (opcode 0x655) (class REG) (id modpc))
(kbopcode (opcode 0x660) (class REG) (id calls))
(kbopcode (opcode 0x66B) (class REG) (id mark))
(kbopcode (opcode 0x66C) (class REG) (id fmark))
(kbopcode (opcode 0x66D) (class REG) (id flushreg))
(kbopcode (opcode 0x66F) (class REG) (id syncf))
(kbopcode (opcode 0x670) (class REG) (id emul))
(kbopcode (opcode 0x671) (class REG) (id ediv))
(kbopcode (opcode 0x674) (class REG) (id cvtir))
(kbopcode (opcode 0x675) (class REG) (id cvtilr))
(kbopcode (opcode 0x676) (class REG) (id scalerl))
(kbopcode (opcode 0x677) (class REG) (id scaler))
(kbopcode (opcode 0x680) (class REG) (id atanr))
(kbopcode (opcode 0x681) (class REG) (id logepr))
(kbopcode (opcode 0x682) (class REG) (id logr))
(kbopcode (opcode 0x683) (class REG) (id remr))
(kbopcode (opcode 0x684) (class REG) (id cmpor))
(kbopcode (opcode 0x685) (class REG) (id cmpr))
(kbopcode (opcode 0x688) (class REG) (id sqrtr))
(kbopcode (opcode 0x68A) (class REG) (id logbnr))
(kbopcode (opcode 0x68B) (class REG) (id roundr))
(kbopcode (opcode 0x68C) (class REG) (id sinr))
(kbopcode (opcode 0x68D) (class REG) (id cosr))
(kbopcode (opcode 0x68E) (class REG) (id tanr))
(kbopcode (opcode 0x68F) (class REG) (id classr))
(kbopcode (opcode 0x690) (class REG) (id atanrl))
(kbopcode (opcode 0x691) (class REG) (id logeprl))
(kbopcode (opcode 0x692) (class REG) (id logrl))
(kbopcode (opcode 0x693) (class REG) (id remrl))
(kbopcode (opcode 0x694) (class REG) (id cmporl))
(kbopcode (opcode 0x695) (class REG) (id cmprl))
(kbopcode (opcode 0x698) (class REG) (id sqrtrl))
(kbopcode (opcode 0x699) (class REG) (id exprl))
(kbopcode (opcode 0x69A) (class REG) (id logbnrl))
(kbopcode (opcode 0x69B) (class REG) (id roundrl))
(kbopcode (opcode 0x69C) (class REG) (id sinrl))
(kbopcode (opcode 0x69D) (class REG) (id cosrl))
(kbopcode (opcode 0x69E) (class REG) (id tanrl))
(kbopcode (opcode 0x69F) (class REG) (id classrl))
(kbopcode (opcode 0x6C0) (class REG) (id cvtri))
(kbopcode (opcode 0x6C1) (class REG) (id cvtril))
(kbopcode (opcode 0x6C2) (class REG) (id cvtzri))
(kbopcode (opcode 0x6C3) (class REG) (id cvtzril))
(kbopcode (opcode 0x6C9) (class REG) (id movr))
(kbopcode (opcode 0x6D9) (class REG) (id movrl))
(kbopcode (opcode 0x6E2) (class REG) (id cpysre))
(kbopcode (opcode 0x6E3) (class REG) (id cpyrsre))
(kbopcode (opcode 0x6E9) (class REG) (id movre))
(kbopcode (opcode 0x701) (class REG) (id mulo))
(kbopcode (opcode 0x708) (class REG) (id remo))
(kbopcode (opcode 0x70B) (class REG) (id divo))
(kbopcode (opcode 0x741) (class REG) (id muli))
(kbopcode (opcode 0x748) (class REG) (id remi))
(kbopcode (opcode 0x749) (class REG) (id modi))
(kbopcode (opcode 0x74B) (class REG) (id divi))
(kbopcode (opcode 0x78B) (class REG) (id divr))
(kbopcode (opcode 0x78C) (class REG) (id mulr))
(kbopcode (opcode 0x78D) (class REG) (id subr))
(kbopcode (opcode 0x78F) (class REG) (id addr))
(kbopcode (opcode 0x79B) (class REG) (id divrl))
(kbopcode (opcode 0x79C) (class REG) (id mulrl))
(kbopcode (opcode 0x79D) (class REG) (id subrl))
(kbopcode (opcode 0x79F) (class REG) (id addrl))
)