(load* "MnemonicsKnowledge.clp")
(defrule construct-enumeration-header
         (declare (salience 10000))
         =>
         (open "opcodes.h" 
               file
               "w")
         (printout file
                   "#ifndef I960MC_OPCODES_H__" crlf
                   "#define I960MC_OPCODES_H__" crlf
                   crlf
                   "namespace i960 {" crlf
                   tab "enum Opcodes {" crlf))

(defrule build-opcode-entry
         ?f <- (mnemonic ?name ?value)
         =>
         (retract ?f)
         (printout file
                   tab tab "_" ?name " = " ?value "," crlf))
(defrule construct-enumeration-footer
         (declare (salience -10000))
         =>
         (printout file
                   tab "};" crlf
                   "} // end namespace i960" crlf
                   "#endif // end I960MC_OPCODES_H__" crlf)
         (close file))
(reset)
(run)
(exit)
