(deftemplate instruction
	(slot opcode (type LEXEME) (default ?NONE))
	(slot class  (type LEXEME) (default ?NONE))
	(slot mnemonic (type LEXEME) (default ?NONE))
)

(deffacts instructions
(instruction (opcode 0x08) (class CTRL) (mnemonic b))
(instruction (opcode 0x09) (class CTRL) (mnemonic call))
(instruction (opcode 0xOA) (class CTRL) (mnemonic ret))
(instruction (opcode 0xOB) (class CTRL) (mnemonic bal))
(instruction (opcode 0x10) (class CTRL) (mnemonic bno))
(instruction (opcode 0x11) (class CTRL) (mnemonic bg))
(instruction (opcode 0x12) (class CTRL) (mnemonic be))
(instruction (opcode 0x13) (class CTRL) (mnemonic bge))
(instruction (opcode 0x14) (class CTRL) (mnemonic bl))
(instruction (opcode 0x15) (class CTRL) (mnemonic bne))
(instruction (opcode 0x16) (class CTRL) (mnemonic ble))
(instruction (opcode 0x17) (class CTRL) (mnemonic bo))
(instruction (opcode 0x18) (class CTRL) (mnemonic faultno))
(instruction (opcode 0x19) (class CTRL) (mnemonic faultg))
(instruction (opcode 0xlA) (class CTRL) (mnemonic faulte))
(instruction (opcode 0x1B) (class CTRL) (mnemonic faultge))
(instruction (opcode 0xlC) (class CTRL) (mnemonic faultl))
(instruction (opcode 0x1D) (class CTRL) (mnemonic faultne))
(instruction (opcode 0xIE) (class CTRL) (mnemonic faultle))
(instruction (opcode 0xIF) (class CTRL) (mnemonic faulto))
(instruction (opcode 0x20) (class COBR) (mnemonic testno))
(instruction (opcode 0x21) (class COBR) (mnemonic testg))
(instruction (opcode 0x22) (class COBR) (mnemonic teste))
(instruction (opcode 0x23) (class COBR) (mnemonic testge))
(instruction (opcode 0x24) (class COBR) (mnemonic testl))
(instruction (opcode 0x25) (class COBR) (mnemonic testne))
(instruction (opcode 0x26) (class COBR) (mnemonic testle))
(instruction (opcode 0x27) (class COBR) (mnemonic testo))
(instruction (opcode 0x30) (class COBR) (mnemonic bbc))
(instruction (opcode 0x31) (class COBR) (mnemonic cmpobg))
(instruction (opcode 0x32) (class COBR) (mnemonic cmpobe))
(instruction (opcode 0x33) (class COBR) (mnemonic cmpobge))
(instruction (opcode 0x34) (class COBR) (mnemonic cmpobl))
(instruction (opcode 0x35) (class COBR) (mnemonic cmpobne))
(instruction (opcode 0x36) (class COBR) (mnemonic cmpoble))
(instruction (opcode 0x37) (class COBR) (mnemonic bbs))
(instruction (opcode 0x38) (class COBR) (mnemonic cmpibno))
(instruction (opcode 0x39) (class COBR) (mnemonic cmpibg))
(instruction (opcode 0x3A) (class COBR) (mnemonic cmpibe))
(instruction (opcode 0x3B) (class COBR) (mnemonic cmpibge))
(instruction (opcode 0x3C) (class COBR) (mnemonic cmpibl))
(instruction (opcode 0x3D) (class COBR) (mnemonic cmpibne))
(instruction (opcode 0x3E) (class COBR) (mnemonic cmpible))
(instruction (opcode 0x3F) (class COBR) (mnemonic cmpibo))
(instruction (opcode 0x80) (class MEM) (mnemonic Idob))
(instruction (opcode 0x82) (class MEM) (mnemonic stob))
(instruction (opcode 0x84) (class MEM) (mnemonic bx))
(instruction (opcode 0x85) (class MEM) (mnemonic balx))
(instruction (opcode 0x86) (class MEM) (mnemonic calix))
(instruction (opcode 0x88) (class MEM) (mnemonic Idos))
(instruction (opcode 0x8A) (class MEM) (mnemonic stos))
(instruction (opcode 0x8C) (class MEM) (mnemonic Ida))
(instruction (opcode 0x90) (class MEM) (mnemonic Id))
(instruction (opcode 0x92) (class MEM) (mnemonic st))
(instruction (opcode 0x98) (class MEM) (mnemonic Idl))
(instruction (opcode 0x9A) (class MEM) (mnemonic stl))
(instruction (opcode 0xAO) (class MEM) (mnemonic Idt))
(instruction (opcode 0xA2) (class MEM) (mnemonic stt))
(instruction (opcode 0xBO) (class MEM) (mnemonic Idq))
(instruction (opcode 0xB2) (class MEM) (mnemonic stq))
(instruction (opcode 0xCO) (class MEM) (mnemonic Idib))
(instruction (opcode 0xC2) (class MEM) (mnemonic stib))
(instruction (opcode 0xC8) (class MEM) (mnemonic Idis))
(instruction (opcode 0xCA) (class MEM) (mnemonic stis))
(instruction (opcode 0x580) (class REG) (mnemonic notbit))
(instruction (opcode 0x581) (class REG) (mnemonic and))
(instruction (opcode 0x583) (class REG) (mnemonic setbit))
(instruction (opcode 0x584) (class REG) (mnemonic notand))
(instruction (opcode 0x586) (class REG) (mnemonic xor))
(instruction (opcode 0x587) (class REG) (mnemonic or))
(instruction (opcode 0x588) (class REG) (mnemonic nor))
(instruction (opcode 0x589) (class REG) (mnemonic xnor))
(instruction (opcode 0x58A) (class REG) (mnemonic not))
(instruction (opcode 0x58B) (class REG) (mnemonic ornot))
(instruction (opcode 0x58C) (class REG) (mnemonic c1rbit))
(instruction (opcode 0x58D) (class REG) (mnemonic notor))
(instruction (opcode 0x58E) (class REG) (mnemonic nand))
(instruction (opcode 0x58F) (class REG) (mnemonic alterbit))
(instruction (opcode 0x590) (class REG) (mnemonic addo))
(instruction (opcode 0x591) (class REG) (mnemonic addi))
(instruction (opcode 0x592) (class REG) (mnemonic subo))
(instruction (opcode 0x593) (class REG) (mnemonic subi))
(instruction (opcode 0x598) (class REG) (mnemonic shro))
(instruction (opcode 0x59A) (class REG) (mnemonic shrdi))
(instruction (opcode 0x59B) (class REG) (mnemonic shri))
(instruction (opcode 0x59C) (class REG) (mnemonic shlo))
(instruction (opcode 0x59D) (class REG) (mnemonic rotate))
(instruction (opcode 0x59E) (class REG) (mnemonic shli))
(instruction (opcode 0x5AO) (class REG) (mnemonic cmpo))
(instruction (opcode 0x5A1) (class REG) (mnemonic cmpi))
(instruction (opcode 0x5A2) (class REG) (mnemonic concmpo))
(instruction (opcode 0x5A3) (class REG) (mnemonic concmpi))
(instruction (opcode 0x5A4) (class REG) (mnemonic cmpinco))
(instruction (opcode 0x5A5) (class REG) (mnemonic cmpinci))
(instruction (opcode 0x5A6) (class REG) (mnemonic cmpdeco))
(instruction (opcode 0x5A7) (class REG) (mnemonic cmpdeci))
(instruction (opcode 0x5AE) (class REG) (mnemonic chkbit))
(instruction (opcode 0x5BO) (class REG) (mnemonic addc))
(instruction (opcode 0x5B2) (class REG) (mnemonic subc))
(instruction (opcode 0x5CC) (class REG) (mnemonic mov))
(instruction (opcode 0x5DC) (class REG) (mnemonic movl))
(instruction (opcode 0x5EC) (class REG) (mnemonic movt))
(instruction (opcode 0x5FC) (class REG) (mnemonic movq))
(instruction (opcode 0x600) (class REG) (mnemonic synmov))
(instruction (opcode 0x601) (class REG) (mnemonic synmovl))
(instruction (opcode 0x602) (class REG) (mnemonic synmovq))
(instruction (opcode 0x603) (class REG) (mnemonic cmpstr))
(instruction (opcode 0x604) (class REG) (mnemonic movqstr))
(instruction (opcode 0x605) (class REG) (mnemonic movstr))
(instruction (opcode 0x610) (class REG) (mnemonic atmod))
(instruction (opcode 0x612) (class REG) (mnemonic atadd))
(instruction (opcode 0x613) (class REG) (mnemonic inspacc))
(instruction (opcode 0x614) (class REG) (mnemonic ldphy))
(instruction (opcode 0x615) (class REG) (mnemonic synld))
(instruction (opcode 0x617) (class REG) (mnemonic fill))
(instruction (opcode 0x642) (class REG) (mnemonic daddc))
(instruction (opcode 0x643) (class REG) (mnemonic dsubc))
(instruction (opcode 0x644) (class REG) (mnemonic dmovt))
(instruction (opcode 0x645) (class REG) (mnemonic modac))
(instruction (opcode 0x646) (class REG) (mnemonic condrec))
(instruction (opcode 0x650) (class REG) (mnemonic modify))
(instruction (opcode 0x651) (class REG) (mnemonic extract))
(instruction (opcode 0x654) (class REG) (mnemonic modtc))
(instruction (opcode 0x655) (class REG) (mnemonic modpc))
(instruction (opcode 0x656) (class REG) (mnemonic receive))
(instruction (opcode 0x660) (class REG) (mnemonic calls))
(instruction (opcode 0x662) (class REG) (mnemonic send))
(instruction (opcode 0x663) (class REG) (mnemonic sendserv))
(instruction (opcode 0x664) (class REG) (mnemonic resumprcs))
(instruction (opcode 0x665) (class REG) (mnemonic schedprcs))
(instruction (opcode 0x666) (class REG) (mnemonic saveprcs))
(instruction (opcode 0x668) (class REG) (mnemonic condwait))
(instruction (opcode 0x669) (class REG) (mnemonic wait))
(instruction (opcode 0x66A) (class REG) (mnemonic signal))
(instruction (opcode 0x66B) (class REG) (mnemonic mark))
(instruction (opcode 0x66C) (class REG) (mnemonic fmark))
(instruction (opcode 0x66D) (class REG) (mnemonic flushreg))
(instruction (opcode 0x66F) (class REG) (mnemonic syncf))
(instruction (opcode 0x670) (class REG) (mnemonic ernul))
(instruction (opcode 0x671) (class REG) (mnemonic ediv))
(instruction (opcode 0x673) (class REG) (mnemonic Idtime))
(instruction (opcode 0x674) (class REG) (mnemonic cvtir))
(instruction (opcode 0x675) (class REG) (mnemonic cvtilr))
(instruction (opcode 0x676) (class REG) (mnemonic scalerl))
(instruction (opcode 0x677) (class REG) (mnemonic scaler))
(instruction (opcode 0x680) (class REG) (mnemonic atanr))
(instruction (opcode 0x681) (class REG) (mnemonic logepr))
(instruction (opcode 0x682) (class REG) (mnemonic logr))
(instruction (opcode 0x683) (class REG) (mnemonic remr))
(instruction (opcode 0x684) (class REG) (mnemonic cmpor))
(instruction (opcode 0x685) (class REG) (mnemonic cmpr))
(instruction (opcode 0x688) (class REG) (mnemonic sqrtr))
(instruction (opcode 0x689) (class REG) (mnemonic expr))
(instruction (opcode 0x68A) (class REG) (mnemonic logbnr))
(instruction (opcode 0x68B) (class REG) (mnemonic roundr))
(instruction (opcode 0x68C) (class REG) (mnemonic sinr))
(instruction (opcode 0x68D) (class REG) (mnemonic cosr))
(instruction (opcode 0x68E) (class REG) (mnemonic tanr))
(instruction (opcode 0x68F) (class REG) (mnemonic c1assr))
(instruction (opcode 0x690) (class REG) (mnemonic atanrl))
(instruction (opcode 0x691) (class REG) (mnemonic logeprl))
(instruction (opcode 0x692) (class REG) (mnemonic logrl))
(instruction (opcode 0x693) (class REG) (mnemonic remrl))
(instruction (opcode 0x694) (class REG) (mnemonic cmporl))
(instruction (opcode 0x695) (class REG) (mnemonic cmprl))
(instruction (opcode 0x698) (class REG) (mnemonic sqrtrl))
(instruction (opcode 0x699) (class REG) (mnemonic exprl))
(instruction (opcode 0x69A) (class REG) (mnemonic logbnrl))
(instruction (opcode 0x69B) (class REG) (mnemonic roundrl))
(instruction (opcode 0x69C) (class REG) (mnemonic sinrl))
(instruction (opcode 0x69D) (class REG) (mnemonic cosrl))
(instruction (opcode 0x69E) (class REG) (mnemonic tanrl))
(instruction (opcode 0x69F) (class REG) (mnemonic c1assrl))
(instruction (opcode 0x6CO) (class REG) (mnemonic cvtri))
(instruction (opcode 0x6Cl) (class REG) (mnemonic cvtril))
(instruction (opcode 0x6C2) (class REG) (mnemonic cvtzri))
(instruction (opcode 0x6C3) (class REG) (mnemonic cvtzril))
(instruction (opcode 0x6C9) (class REG) (mnemonic movr))
(instruction (opcode 0x6D9) (class REG) (mnemonic movrl))
(instruction (opcode 0x6E2) (class REG) (mnemonic cpysre))
(instruction (opcode 0x6E3) (class REG) (mnemonic cpyrsre))
(instruction (opcode 0x6E9) (class REG) (mnemonic movre))
(instruction (opcode 0x701) (class REG) (mnemonic mulo))
(instruction (opcode 0x708) (class REG) (mnemonic remo))
(instruction (opcode 0x70B) (class REG) (mnemonic divo))
(instruction (opcode 0x741) (class REG) (mnemonic muli))
(instruction (opcode 0x748) (class REG) (mnemonic remi))
(instruction (opcode 0x749) (class REG) (mnemonic modi))
(instruction (opcode 0x74B) (class REG) (mnemonic divi))
(instruction (opcode 0x78B) (class REG) (mnemonic divr))
(instruction (opcode 0x78C) (class REG) (mnemonic muir))
(instruction (opcode 0x78D) (class REG) (mnemonic subr))
(instruction (opcode 0x78F) (class REG) (mnemonic addr))
(instruction (opcode 0x79B) (class REG) (mnemonic divrl))
(instruction (opcode 0x79C) (class REG) (mnemonic mulrl))
(instruction (opcode 0x79D) (class REG) (mnemonic subrl))
(instruction (opcode 0x79F) (class REG) (mnemonic addrl))
)
