(load* "i960kb_ops.clp")
(load* "i960mc_ops.clp")
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
(defrule emit-enum-entry:protected-unique
         (mcopcode (opcode ?code)
                   (id ?name))
         (not (kbopcode (opcode ?code)
                        (id ?name)))
         =>
         (printout file 
                   (#ifdef PROTECTED_ARCHITECTURE) crlf
                   tab tab "_" ?name " = " ?code "," crlf
                   (#endif) " " (single-line-comment end PROTECTED_ARCHITECTURE) crlf))
(defrule emit-enum-entry:in-numerics-as-well
         (mcopcode (opcode ?code)
                   (id ?name))
         (kbopcode (opcode ?code)
                   (id ?name))
         =>
         (printout file 
                   tab tab "_" ?name " = " ?code "," crlf))

         

(reset)
(run)
(exit)
