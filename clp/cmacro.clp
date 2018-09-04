; c macro code generation functions
(defgeneric #define)
(deffunction single-line-comment
             ($?contents)
             (format nil "// %s" (implode$ ?contents)))

(deffunction #ifndef 
             (?x)
             (format nil 
                     "#ifndef %s" 
                     ?x))

(defmethod #define 
  ((?x LEXEME))
  (format nil 
          "#define %s"
          ?x))

(defmethod #define
  ((?x LEXEME)
   ?value)
  (format nil 
          "%s %s" 
          (#define ?x)
          (str-cat ?value)))

(deffunction #endif
             ()
             "#endif")


(deffunction def-c-header
             (?router ?name)
             (printout ?router
                       (#ifndef ?name) crlf
                       (#define ?name) crlf))

(deffunction def-c-footer
             (?router ?name)
             (printout ?router
                       (#endif) " " (single-line-comment end ?name) crlf))
(deffunction namespace{ 
             (?name)
             (format nil "namespace %s {" ?name))
(deffunction }namespace
             (?name)
             (str-cat "} " 
                      (single-line-comment end namespace ?name)))

