#ifndef I960_OPCODES_H__
#define I960_OPCODES_H__
namespace i960 {
	enum Opcodes {
		Andnot = 0x582,
		Spanbit = 0x640,
		Scanbyte = 0x5AC,
		Scanbit = 0x641,
		Divi = 0x74B,
		Modi = 0x749,
		Remi = 0x748,
		Muli = 0x741,
		Divo = 0x70B,
		Remo = 0x708,
		Mulo = 0x701,
		Ediv = 0x671,
		Emul = 0x670,
		Syncf = 0x66F,
		Flushreg = 0x66D,
		Fmark = 0x66C,
		Mark = 0x66B,
		Modpc = 0x655,
		Modtc = 0x654,
		Extract = 0x651,
		Modify = 0x650,
		Modac = 0x645,
		Atadd = 0x612,
		Atmod = 0x610,
		Movq = 0x5FC,
		Movt = 0x5EC,
		Movl = 0x5DC,
		Mov = 0x5CC,
		Subc = 0x5B2,
		Addc = 0x5B0,
		Chkbit = 0x5AE,
		Cmpdeci = 0x5A7,
		Cmpdeco = 0x5A6,
		Cmpinci = 0x5A5,
		Cmpinco = 0x5A4,
		Concmpi = 0x5A3,
		Concmpo = 0x5A2,
		Cmpi = 0x5A1,
		Cmpo = 0x5A0,
		Shli = 0x59E,
		Rotate = 0x59D,
		Shlo = 0x59C,
		Shri = 0x59B,
		Shrdi = 0x59A,
		Shro = 0x598,
		Subi = 0x593,
		Subo = 0x592,
		Addi = 0x591,
		Addo = 0x590,
		Alterbit = 0x58F,
		Nand = 0x58E,
		Notor = 0x58D,
		Clrbit = 0x58C,
		Ornot = 0x58B,
		Not = 0x58A,
		Xnor = 0x589,
		Nor = 0x588,
		Or = 0x587,
		Xor = 0x586,
		Notand = 0x584,
		Setbit = 0x583,
		And = 0x581,
		Notbit = 0x580,
		Stis = 0xCA,
		Ldis = 0xC8,
		Stib = 0xC2,
		Ldib = 0xC0,
		Stq = 0xB2,
		Ldq = 0xB0,
		Stt = 0xA2,
		Ldt = 0xA0,
		Stl = 0x9A,
		Ldl = 0x98,
		St = 0x92,
		Ld = 0x90,
		Lda = 0x8C,
		Stos = 0x8A,
		Ldos = 0x88,
		Calix = 0x86,
		Balx = 0x85,
		Bx = 0x84,
		Stob = 0x82,
		Ldob = 0x80,
		Cmpibo = 0x3F,
		Cmpible = 0x3E,
		Cmpibne = 0x3D,
		Cmpibl = 0x3C,
		Cmpibge = 0x3B,
		Cmpibe = 0x3A,
		Cmpibg = 0x39,
		Cmpibno = 0x38,
		Bbs = 0x37,
		Cmpoble = 0x36,
		Cmpobne = 0x35,
		Cmpobl = 0x34,
		Cmpobge = 0x33,
		Cmpobe = 0x32,
		Cmpobg = 0x31,
		Bbc = 0x30,
		Testo = 0x27,
		Testle = 0x26,
		Testne = 0x25,
		Testl = 0x24,
		Testge = 0x23,
		Teste = 0x22,
		Testg = 0x21,
		Testno = 0x20,
		Faulto = 0x1F,
		Faultle = 0x1E,
		Faultne = 0x1D,
		Faultl = 0x1C,
		Faultge = 0x1B,
		Faulte = 0x1A,
		Faultg = 0x19,
		Faultno = 0x18,
		Bo = 0x17,
		Ble = 0x16,
		Bne = 0x15,
		Bl = 0x14,
		Bge = 0x13,
		Be = 0x12,
		Bg = 0x11,
		Bno = 0x10,
		Bal = 0x0B,
		Ret = 0x0A,
		Call = 0x09,
		B = 0x08,
		Calls = 0x660,
#ifdef NUMERICS_ARCHITECTURE
		Addr = 0x78F,
		Subr = 0x78D,
		Mulr = 0x78C,
		Divr = 0x78B,
		Addrl = 0x79F,
		Subrl = 0x79D,
		Mulrl = 0x79C,
		Divrl = 0x79B,
		Movrl = 0x6D9,
		Movr = 0x6C9,
		Cvtzril = 0x6C3,
		Cvtzri = 0x6C2,
		Cvtril = 0x6C1,
		Cvtri = 0x6C0,
		Classrl = 0x69F,
		Tanrl = 0x69E,
		Cosrl = 0x69D,
		Sinrl = 0x69C,
		Roundrl = 0x69B,
		Logbnrl = 0x69A,
		Exprl = 0x699,
		Sqrtrl = 0x698,
		Cmprl = 0x695,
		Cmporl = 0x694,
		Remrl = 0x693,
		Logrl = 0x692,
		Logeprl = 0x691,
		Atanrl = 0x690,
		Classr = 0x68F,
		Tanr = 0x68E,
		Cosr = 0x68D,
		Sinr = 0x68C,
		Roundr = 0x68B,
		Logbnr = 0x68A,
		Expr = 0x689,
		Sqrtr = 0x688,
		Cmpr = 0x685,
		Cmpor = 0x684,
		Remr = 0x683,
		Logr = 0x682,
		Logepr = 0x681,
		Atanr = 0x680,
		Scaler = 0x677,
		Scalerl = 0x676,
		Cvtilr = 0x675,
		Cvtir = 0x674,
		Cpyrsre = 0x6E3,
		Cpysre = 0x6E2,
		Dmovt = 0x644,
		Dsubc = 0x643,
		Daddc = 0x642,
		Movre = 0x6E9,
		Synld = 0x615,
		Synmovq = 0x602,
		Synmovl = 0x601,
		Synmov = 0x600,
#ifdef PROTECTED_ARCHITECTURE
		Movstr = 0x605,
		Movqstr = 0x604,
		Cmpstr = 0x603,
		Signal = 0x66A,
		Wait = 0x669,
		Condwait = 0x668,
		Saveprcs = 0x666,
		Schedprcs = 0x665,
		Resumprcs = 0x664,
		Sendserv = 0x663,
		Send = 0x662,
		Receive = 0x656,
		Condrec = 0x646,
		Fill = 0x617,
		Ldphy = 0x614,
		Inspacc = 0x613,
		Ldtime = 0x673,
#endif // end PROTECTED_ARCHITECTURE
#endif // end NUMERICS_ARCHITECTURE
	};
	//template<Opcodes opcode>
	//constexpr bool isNumericOperation = false;
	//template<Opcodes opcode>
	//constexpr bool isCoreOperation = true;
	//template<Opcodes opcode>
	//constexpr bool isProtectedOperation = isCoreOperation<opcode> && isNumericOperation<opcode>;
} // end namespace i960
#endif // end I960_OPCODES_H__
