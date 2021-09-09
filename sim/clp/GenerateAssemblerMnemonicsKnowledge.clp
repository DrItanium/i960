(defrule open-output-file
         (declare (salience 10000))
         =>
         (open "MnemonicsKnowledge.clp" file "w")
         (printout file 
                   "(deffacts assembler-mnemonics" crlf))

(defrule construct-mnemonic
         ?f <- (construct mnemonics ?item $?rest)
         =>
         (retract ?f)
         (assert (construct mnemonics $?rest)
                 (mnemonic (explode$ ?item))))

(defrule printout-mnemonic
         ?f <- (mnemonic ?name ?value)
         =>
         (retract ?f)
         (printout file
                   "(mnemonic " ?name " 0x" ?value ")" crlf))

(defrule finished-knowledge-construction
         (declare (salience -10000))
         =>
         (printout file
                   ")" crlf)
         (close file))


(reset)
(run)
(exit)
