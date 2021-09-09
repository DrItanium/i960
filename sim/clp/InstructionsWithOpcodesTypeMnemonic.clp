(deffacts gen-facts
          (lines "0x08 CTRL b "
                 "0x09 CTRL call "
                 "0xOA CTRL ret "
                 "0xOB CTRL bal "
                 "0x10 CTRL bno "
                 "0x11 CTRL bg "
                 "0x12 CTRL be "
                 "0x13 CTRL bge "
                 "0x14 CTRL bl "
                 "0x15 CTRL bne "
                 "0x16 CTRL ble "
                 "0x17 CTRL bo "
                 "0x18 CTRL faultno "
                 "0x19 CTRL faultg "
                 "0xlA CTRL faulte "
                 "0x1B CTRL faultge "
                 "0xlC CTRL faultl "
                 "0x1D CTRL faultne "
                 "0xIE CTRL faultle "
                 "0xIF CTRL faulto "
                 "0x20 COBR testno "
                 "0x21 COBR testg "
                 "0x22 COBR teste "
                 "0x23 COBR testge "
                 "0x24 COBR testl "
                 "0x25 COBR testne "
                 "0x26 COBR testle "
                 "0x27 COBR testo "
                 "0x30 COBR bbc "
                 "0x31 COBR cmpobg "
                 "0x32 COBR cmpobe "
                 "0x33 COBR cmpobge "
                 "0x34 COBR cmpobl "
                 "0x35 COBR cmpobne "
                 "0x36 COBR cmpoble "
                 "0x37 COBR bbs "
                 "0x38 COBR cmpibno "
                 "0x39 COBR cmpibg "
                 "0x3A COBR cmpibe "
                 "0x3B COBR cmpibge "
                 "0x3C COBR cmpibl "
                 "0x3D COBR cmpibne "
                 "0x3E COBR cmpible "
                 "0x3F COBR cmpibo "
                 "0x80 MEM Idob "
                 "0x82 MEM stob "
                 "0x84 MEM bx "
                 "0x85 MEM balx "
                 "0x86 MEM calix "
                 "0x88 MEM Idos "
                 "0x8A MEM stos "
                 "0x8C MEM Ida "
                 "0x90 MEM Id "
                 "0x92 MEM st "
                 "0x98 MEM Idl "
                 "0x9A MEM stl "
                 "0xAO MEM Idt "
                 "0xA2 MEM stt "
                 "0xBO MEM Idq "
                 "0xB2 MEM stq "
                 "0xCO MEM Idib "
                 "0xC2 MEM stib "
                 "0xC8 MEM Idis "
                 "0xCA MEM stis "
                 "0x580 REG notbit "
                 "0x581 REG and "
                 "0x582 REG and not "
                 "0x583 REG setbit "
                 "0x584 REG notand "
                 "0x586 REG xor "
                 "0x587 REG or "
                 "0x588 REG nor "
                 "0x589 REG xnor "
                 "0x58A REG not "
                 "0x58B REG ornot "
                 "0x58C REG c1rbit "
                 "0x58D REG notor "
                 "0x58E REG nand "
                 "0x58F REG alterbit "
                 "0x590 REG addo "
                 "0x591 REG addi "
                 "0x592 REG subo "
                 "0x593 REG subi "
                 "0x598 REG shro "
                 "0x59A REG shrdi "
                 "0x59B REG shri "
                 "0x59C REG shlo "
                 "0x59D REG rotate "
                 "0x59E REG shli "
                 "0x5AO REG cmpo "
                 "0x5A1 REG cmpi "
                 "0x5A2 REG concmpo "
                 "0x5A3 REG concmpi "
                 "0x5A4 REG cmpinco "
                 "0x5A5 REG cmpinci "
                 "0x5A6 REG cmpdeco "
                 "0x5A7 REG cmpdeci "
                 "0x5AC REG scan byte "
                 "0x5AE REG chkbit "
                 "0x5BO REG addc "
                 "0x5B2 REG subc "
                 "0x5CC REG mov "
                 "0x5DC REG movl "
                 "0x5EC REG movt "
                 "0x5FC REG movq "
                 "0x600 REG synmov "
                 "0x601 REG synmovl "
                 "0x602 REG synmovq "
                 "0x603 REG cmpstr "
                 "0x604 REG movqstr "
                 "0x605 REG movstr "
                 "0x610 REG atmod "
                 "0x612 REG atadd "
                 "0x613 REG inspacc "
                 "0x614 REG ldphy "
                 "0x615 REG synld "
                 "0x617 REG fill "
                 "0x640 REG span bit "
                 "0x641 REG scan bit "
                 "0x642 REG daddc "
                 "0x643 REG dsubc "
                 "0x644 REG dmovt "
                 "0x645 REG modac "
                 "0x646 REG condrec "
                 "0x650 REG modify "
                 "0x651 REG extract "
                 "0x654 REG modtc "
                 "0x655 REG modpc "
                 "0x656 REG receive "
                 "0x660 REG calls "
                 "0x662 REG send "
                 "0x663 REG sendserv "
                 "0x664 REG resumprcs "
                 "0x665 REG schedprcs "
                 "0x666 REG saveprcs "
                 "0x668 REG condwait "
                 "0x669 REG wait "
                 "0x66A REG signal "
                 "0x66B REG mark "
                 "0x66C REG fmark "
                 "0x66D REG flushreg "
                 "0x66F REG syncf "
                 "0x670 REG ernul "
                 "0x671 REG ediv "
                 "0x673 REG Idtime "
                 "0x674 REG cvtir "
                 "0x675 REG cvtilr "
                 "0x676 REG scalerl "
                 "0x677 REG scaler "
                 "0x680 REG atanr "
                 "0x681 REG logepr "
                 "0x682 REG logr "
                 "0x683 REG remr "
                 "0x684 REG cmpor "
                 "0x685 REG cmpr "
                 "0x688 REG sqrtr "
                 "0x689 REG expr "
                 "0x68A REG logbnr "
                 "0x68B REG roundr "
                 "0x68C REG sinr "
                 "0x68D REG cosr "
                 "0x68E REG tanr "
                 "0x68F REG c1assr "
                 "0x690 REG atanrl "
                 "0x691 REG logeprl "
                 "0x692 REG logrl "
                 "0x693 REG remrl "
                 "0x694 REG cmporl "
                 "0x695 REG cmprl "
                 "0x698 REG sqrtrl "
                 "0x699 REG exprl "
                 "0x69A REG logbnrl "
                 "0x69B REG roundrl "
                 "0x69C REG sinrl "
                 "0x69D REG cosrl "
                 "0x69E REG tanrl "
                 "0x69F REG c1assrl "
                 "0x6CO REG cvtri "
                 "0x6Cl REG cvtril "
                 "0x6C2 REG cvtzri "
                 "0x6C3 REG cvtzril "
                 "0x6C9 REG movr "
                 "0x6D9 REG movrl "
                 "0x6E2 REG cpysre "
                 "0x6E3 REG cpyrsre "
                 "0x6E9 REG movre "
                 "0x701 REG mulo "
                 "0x708 REG remo "
                 "0x70B REG divo "
                 "0x741 REG muli "
                 "0x748 REG remi "
                 "0x749 REG modi "
                 "0x74B REG divi "
                 "0x78B REG divr "
                 "0x78C REG muir "
                 "0x78D REG subr "
                 "0x78F REG addr "
                 "0x79B REG divrl "
                 "0x79C REG mulrl "
                 "0x79D REG subrl "
                 "0x79F REG addrl "
                 ))


(defrule open-fact-construct
         (declare (salience 10000))
         =>
         (open "MnemonicsWithCodesAndClass.clp"
               file
               "w")
         (printout file
                   "(deftemplate instruction" crlf
                   tab "(slot opcode (type LEXEME) (default ?NONE))" crlf
                   tab "(slot class  (type LEXEME) (default ?NONE))" crlf
                   tab "(slot mnemonic (type LEXEME) (default ?NONE))" crlf
                   ")" crlf crlf
                   "(deffacts instructions" crlf))
(deftemplate instruction
             (slot opcode 
                   (type LEXEME)
                   (default ?NONE))
             (slot class
                   (type LEXEME)
                   (default ?NONE))
             (slot mnemonic 
                   (type LEXEME)
                   (default ?NONE)))

(defrule generate-fact
         ?f <- (lines ?line $?rest)
         =>
         (retract ?f)
         (assert (lines $?rest)
                 (make instruction (explode$ ?line))))
(defrule generate-instruction
         ?f <- (make instruction ?opcode ?class ?mnemonic)
         =>
         (retract ?f)
         (assert (instruction (opcode ?opcode)
                              (class ?class)
                              (mnemonic ?mnemonic))))


(defrule print-rule-representation
         ?f <- (instruction (opcode ?o)
                            (class ?c)
                            (mnemonic ?m))
         =>
         (retract ?f)
         (printout file
                   "(instruction (opcode " ?o ") (class " ?c ") (mnemonic " ?m "))" crlf))

(defrule close-fact-construct
         (declare (salience -10000))
         =>
         (printout file ")" crlf)
         (close file))

(reset)
(run)
(exit)

