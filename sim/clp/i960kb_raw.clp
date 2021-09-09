(deffacts raw_i960_kb_entries
          (ops 0xOA CTRL ret 
               0xOB CTRL bal 
               0x10 CTRL bno 
               0x11 CTRL bg 
               0x12 CTRL be 
               0x13 CTRL bge 
               0x14 CTRL bl 
               0x15 CTRL bne 
               0x16 CTRL ble 
               0x17 CTRL bo 
               0x18 CTRL faultno 
               0x19 CTRL faultg 
               0x1A CTRL faulte 
               0x1B CTRL faultge 
               0x1C CTRL faultl 
               0x10 CTRL faultne 
               0x1E CTRL faultle 
               0x1F CTRL faulto 
               0x20 COBR testno 
               0x21 COBR testg 
               0x22 COBR teste 
               0x23 COBR testge 
               0x24 COBR testl 
               0x25 COBR testne 
               0x26 COBR testle 
               0x27 COBR testo 
               0x30 COBR bbc 
               0x31 COBR cmpobg 
               0x32 COBR cmpobe 
               0x33 COBR cmpobge 
               0x34 COBR cmpobl 
               0x35 COBR cmpobne 
               0x36 COBR cmpoble 
               0x37 COBR bbs 
               0x38 COBR cmpibno 
               0x39 COBR cmpibg 
               0x3A COBR cmpibe 
               0x3B COBR cmpibge 
               0x3C COBR cmpibl 
               0x3D COBR cmpibne 
               0x3E COBR cmpible 
               0x3F COBR cmpibo 
               0x80 MEM Idob 
               0x84 MEM bx 
               0x85 MEM balx 
               0x86 MEM calix 
               0x88 MEM ldos 
               0x8A MEM stos 
               0x8C MEM lda 
               0x90 MEM ld 
               0x92 MEM st 
               0x98 MEM Idl 
               0x9A MEM stl 
               0xA0 MEM Idt 
               0xA2 MEM stt 
               0xB0 MEM Idq 
               0xB2 MEM stq 
               0xC0 MEM Idib 
               0xC2 MEM stib 
               0xC8 MEM ldis 
               0xCA MEM stis 
               0x580 REG notbit 
               0x581 REG and 
               0x582 REG andnot 
               0x583 REG setbit 
               0x584 REG notand 
               0x586 REG xor 
               0x587 REG or 
               0x588 REG nor 
               0x589 REG xnor 
               0x58A REG not 
               0x58B REG ornot 
               0x58C REG clrbit 
               0x58D REG notor 
               0x58E REG nand 
               0x58F REG alterbit 
               0x590 REG addo 
               0x591 REG addi 
               0x592 REG subo 
               0x593 REG subi 
               0x598 REG shro 
               0x59A REG shrdi 
               0x59B REG shri 
               0x59C REG shlo 
               0x59D REG rotate 
               0x59E REG shli 
               0x5A0 REG cmpo 
               0x5A1 REG cmpi 
               0x5A2 REG concmpo 
               0x5A3 REG concmpi
               0x5A4 REG cmpinco
               0x5A5 REG cmpinci 
               0x5A6 REG cmpdeco 
               0x5A7 REG cmpdeci 
               0x5AC REG scanbyte 
               0x5AE REG chkbit 
               0x5BO REG addc 
               0x5B2 REG subc 
               0x5CC REG mov 
               0x5DC REG movl 
               0x5EC REG movt 
               0x5FC REG movq 
               0x600 REG synmov 
               0x601 REG synmovi 
               0x602 REG synmovq 
               0x610 REG atmod 
               0x612 REG atadd 
               0x615 REG synld 
               0x640 REG spanbit 
               0x641 REG scanbit 
               0x642 REG daddc 
               0x643 REG dsubc 
               0x644 REG dmovt 
               0x645 REG modac 
               0x650 REG modify 
               0x651 REG extract 
               0x654 REG modtc 
               0x655 REG modpc 
               0x660 REG calls 
               0x66B REG mark 
               0x66C REG fmark 
               0x66D REG flushreg 
               0x66F REG syncf 
               0x670 REG emul 
               0x671 REG ediv 
               0x674 REG cvtir 
               0x675 REG cvtilr 
               0x676 REG scalerl 
               0x677 REG scaler 
               0x680 REG atanr 
               0x681 REG logepr 
               0x682 REG logr 
               0x683 REG remr 
               0x684 REG cmpor 
               0x685 REG cmpr 
               0x688 REG sqrtr 
               0x68A REG logbnr 
               0x68B REG roundr 
               0x68C REG sinr 
               0x68D REG cosr 
               0x68E REG tanr 
               0x68F REG classr 
               0x690 REG atanrl 
               0x691 REG logeprl 
               0x692 REG logrl 
               0x693 REG remrl 
               0x694 REG cmporl 
               0x695 REG cmprl 
               0x698 REG sqrtrl 
               0x699 REG exprl 
               0x69A REG logbnrl 
               0x69B REG roundrl 
               0x69C REG sinrl 
               0x69D REG cosrl 
               0x69E REG tanrl 
               0x69F REG classrl 
               0x6C0 REG cvtri 
               0x6C1 REG cvtril 
               0x6C2 REG cvtzri 
               0x6C3 REG cvtzril 
               0x6C9 REG movr 
               0x6D9 REG movrl 
               0x6E2 REG cpysre 
               0x6E3 REG cpyrsre 
               0x6E9 REG movre 
               0x701 REG mulo 
               0x708 REG remo 
               0x70B REG divo 
               0x741 REG muli 
               0x748 REG remi 
               0x749 REG modi 
               0x74B REG divi 
               0x78B REG divr 
               0x78C REG mulr 
               0x78D REG subr 
               0x78F REG addr 
               0x79B REG divrl 
               0x79C REG mulrl 
               0x79D REG subrl 
               0x79F REG addrl 
               ))
(defrule open-facts-file
         (declare (salience 10000))
         =>
         (open i960kb_ops.clp file "w")
         (printout file 
                   "(deftemplate opcode" crlf 
                   "(slot opcode (type LEXEME) (default ?NONE))" crlf
                   "(slot class (type LEXEME) (default ?NONE))" crlf
                   "(slot id (type LEXEME) (default ?NONE))" crlf
                   ")" crlf
                   "(deffacts i960kb-ops" crlf))
(defrule close-facts-file 
         (declare (salience -10000))
         =>
         (printout file
                   ")" crlf))
(defrule make-entry
         ?f <- (ops ?code ?class ?name $?rest)
         =>
         (retract ?f)
         (assert (ops $?rest)
                 (defopcode ?code ?class ?name)))
(defrule emit-entry
         ?f <- (defopcode ?code ?class ?name)
         =>
         (retract ?f)
         (printout file
                   "(opcode (opcode " ?code ") (class " ?class ") (id " ?name "))" crlf))