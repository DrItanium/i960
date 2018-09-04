(load* "MnemonicsWithCodesAndClass.clp")
(load* "cmacro.clp")

(defrule open-header
         (declare (salience 100))
         =>
         (open "../opcodes.h"
               file
               "w")
         (def-c-header file
                       I960_OPCODES_H__)
         (printout file
                   (namespace{ i960) crlf
                   tab "enum Opcodes {" crlf))

(defrule close-header
         (declare (salience -100))
         =>
         (printout file 
                   tab "};" crlf
                   (}namespace i960) crlf)
         (def-c-footer file
                       I960_OPCODES_H__)
         (close file))

(defrule emit-enum-entry
         (instruction (opcode ?code)
                      (mnemonic ?name))
         =>
         (printout file 
                   tab tab "_" ?name " = " ?code "," crlf))

(reset)
(run)
(exit)
