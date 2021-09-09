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
(defmethod addo ((?src1 SYMBOL INTEGER) (?src2 SYMBOL INTEGER) (?dest SYMBOL)) (format nil "addo %s, %s, %s" (str-cat ?src1) (str-cat ?src2) (str-cat ?dest)))
(defmethod addi ((?src1 SYMBOL INTEGER) (?src2 SYMBOL INTEGER) (?dest SYMBOL)) (format nil "addi %s, %s, %s" (str-cat ?src1) (str-cat ?src2) (str-cat ?dest)))
(defmethod addc ((?src1 SYMBOL INTEGER) (?src2 SYMBOL INTEGER) (?dest SYMBOL)) (format nil "addc %s, %s, %s" (str-cat ?src1) (str-cat ?src2) (str-cat ?dest)))
(defmethod alterbit ((?src1 SYMBOL INTEGER) (?src2 SYMBOL INTEGER) (?dest SYMBOL)) (format nil "alterbit %s, %s, %s" (str-cat ?src1) (str-cat ?src2) (str-cat ?dest)))
(defmethod andop ((?src1 SYMBOL INTEGER) (?src2 SYMBOL INTEGER) (?dest SYMBOL)) (format nil "and %s, %s, %s" (str-cat ?src1) (str-cat ?src2) (str-cat ?dest)))
(defmethod andnot ((?src1 SYMBOL INTEGER) (?src2 SYMBOL INTEGER) (?dest SYMBOL)) (format nil "andnot %s, %s, %s" (str-cat ?src1) (str-cat ?src2) (str-cat ?dest)))
(defmethod clrbit ((?src1 SYMBOL INTEGER) (?src2 SYMBOL INTEGER) (?dest SYMBOL)) (format nil "clrbit %s, %s, %s" (str-cat ?src1) (str-cat ?src2) (str-cat ?dest)))

(defglobal MAIN
           ?*registers* = (create$ pfp sp rip r3 r4 r5 r6 r7 r8 r9 r10 r11 r12 r13 r14 r15
                                   g0 g1 g2 g3 g4 g5 g6 g7 g8 g9 g10 g11 g12 g13 g14 fp)
           ?*long-registers* = (create$ pfp rip r4 r6 r8 r10 r12 r14
                                        g0 g2 g4 g6 g8 g10 g12 g14)
           ?*quad-registers* = (create$ pfp r4 r8 r12 
                                        g0 g4 g8 g12)
           ?*registers-with-literals* = (create$ ?*registers* 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31)
           )
(deffunction generate-moves2
             (?fn ?list)
             (progn$ (?src ?list)
                     (progn$ (?dest ?list)
                             (printout t (funcall ?fn ?src ?dest) crlf)))
             (loop-for-count (?src 0 31) do
                             (progn$ (?dest ?list)
                                     (printout t (funcall ?fn ?src ?dest) crlf))))
(deffunction generate-moves3
             (?fn ?srcr ?destr)
             (progn$ (?src1 ?srcr)
                     (progn$ (?src2 ?srcr)
                             (progn$ (?dest ?destr)
                                     (printout t (funcall ?fn
                                                          ?src1
                                                          ?src2
                                                          ?dest) crlf)))))

(generate-moves2 generate-move ?*registers*)
(generate-moves2 generate-movel ?*long-registers*)
(generate-moves2 generate-movet ?*quad-registers*)
(generate-moves2 generate-moveq ?*quad-registers*)
(generate-moves3 addo ?*registers-with-literals* ?*registers*)
(generate-moves3 addi ?*registers-with-literals* ?*registers*)
(generate-moves3 addc ?*registers-with-literals* ?*registers*)
(generate-moves3 alterbit ?*registers-with-literals* ?*registers*)
(generate-moves3 andop ?*registers-with-literals* ?*registers*)
(generate-moves3 andnot ?*registers-with-literals* ?*registers*)
(generate-moves3 clrbit ?*registers-with-literals* ?*registers*)



(exit)

