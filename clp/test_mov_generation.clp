(defgeneric generate-move)
(defgeneric generate-movel)
(defgeneric generate-movet)
(defgeneric generate-moveq)
(defmethod generate-move ((?src SYMBOL) (?dest SYMBOL)) (format nil "mov %s, %s" ?src ?dest))
(defmethod generate-move ((?src INTEGER) (?dest SYMBOL)) (format nil "mov %d, %s" ?src ?dest))
(defmethod generate-movel ((?src SYMBOL) (?dest SYMBOL)) (format nil "movl %s, %s" ?src ?dest))
(defmethod generate-movel ((?src INTEGER) (?dest SYMBOL)) (format nil "movl %d, %s" ?src ?dest))
(defmethod generate-movet ((?src SYMBOL) (?dest SYMBOL)) (format nil "movt %s, %s" ?src ?dest))
(defmethod generate-movet ((?src INTEGER) (?dest SYMBOL)) (format nil "movt %d, %s" ?src ?dest))
(defmethod generate-moveq ((?src SYMBOL) (?dest SYMBOL)) (format nil "movq %s, %s" ?src ?dest))
(defmethod generate-moveq ((?src INTEGER) (?dest SYMBOL)) (format nil "movq %d, %s" ?src ?dest))
(defglobal MAIN
           ?*registers* = (create$ pfp sp rip r3 r4 r5 r6 r7 r8 r9 r10 r11 r12 r13 r14 r15
                                   g0 g1 g2 g3 g4 g5 g6 g7 g8 g9 g10 g11 g12 g13 g14 fp)
           ?*long-registers* = (create$ pfp rip r4 r6 r8 r10 r12 r14
                                   g0 g2 g4 g6 g8 g10 g12 g14)
           ?*quad-registers* = (create$ pfp r4 r8 r12 
                                   g0 g4 g8 g12))
(deffunction generate-moves
             (?fn ?list)
             (progn$ (?src ?list)
                     (progn$ (?dest ?list)
                             (printout t (funcall ?fn ?src ?dest) crlf)))
             (loop-for-count (?src 0 31) do
                     (progn$ (?dest ?list)
                             (printout t (funcall ?fn ?src ?dest) crlf))))

(generate-moves generate-move ?*registers*)
(generate-moves generate-movel ?*long-registers*)
(generate-moves generate-movet ?*quad-registers*)
(generate-moves generate-moveq ?*quad-registers*)


(exit)

