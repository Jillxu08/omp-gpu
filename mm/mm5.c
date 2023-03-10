;; block mm algorithm with 3 malloc

(%ifndef* NF-TYPE
  (%defconstant NF-TYPE GCC)) ; one of (GCC LW-SC CL-SC XCC XCCCL)
(%include "rule/tcell-setrule.sh")

(c-exp "#include <stdio.h>")
(c-exp "#include <stdlib.h>")
(c-exp "#include <math.h>")
(c-exp "#include <time.h>")


(%defconstant MATRIX_SIZE 8)

(decl (csym::Block_matMul X Y Z m) (fn void (ptr double) (ptr double) (ptr double) int))
(decl (csym::matMul A B C n) (fn void (ptr double) (ptr double) (ptr double) int))
(decl (csym::matAdd A B C m) (fn void (ptr double) (ptr double) (ptr double) int))
(decl (csym::get-wall-time) (fn double void))

(def (task mm-start))

(def (task-body mm-start)
    (defs int N i j)
    (defs double start end)
    (defs double start2 end2)
    (def elapsed double)
    (def elapsed2 double)
    (defs (ptr double) X Y Z W A B C)
    
    (csym::fprintf stderr "mm5's result is as follows:   ~%")
    (csym::fprintf stderr "-------------------------------------------------------------------------~%")

        (= N MATRIX_SIZE)
        (csym::fprintf stderr "Matrix size = %2d    " N) 
        (= X (csym::malloc (* (sizeof double) (* N N))))
        (= Y (csym::malloc (* (sizeof double) (* N N))))
        (= Z (csym::malloc (* (sizeof double) (* N N))))
        (= W (csym::malloc (* (sizeof double) (* N N))))
        (if (or (== X NULL) (or (== Y NULL) (or (== Z NULL) (== W NULL))))
            (begin 
                (csym::fprintf stderr "Out of memory (1)~%")
                (csym::exit 1)
            )
        ) 
        (csym::srand 0)    
       (for ((= i 0) (< i (* N N)) (inc i))
            (= (aref X i) (/ (csym::rand) (+ RAND_MAX 1.0)))
            (= (aref Y i) (/ (csym::rand) (+ RAND_MAX 1.0)))
            )

        (= start (csym::get-wall-time))
        (csym::matMul X Y W N)
        (= end (csym::get-wall-time))
        (= elapsed (- end start))
        (csym::fprintf stderr "naive = %5.4f    " elapsed)

        (= start2 (csym::get-wall-time))
        (csym::Block_matMul X Y Z N)
        (= end2 (csym::get-wall-time))
        (= elapsed2 (- end2 start2))
        (csym::fprintf stderr "Block_matMul = %5.4f  ~%" elapsed2) 

        (def flag int 0)
        (for ((= i 0) (< i N) (inc i))
            (for ((= j 0) (< j N) (inc j))      
                (if (> (csym::fabs(- (aref Z (+ (* i N) j)) (aref W (+ (* i N) j)))) 0.000001)
                    (begin
                        (csym::fprintf stderr "THE ANSWER IS WRONG!~%")
                        (= flag 1)
                        (break))))
            (if (== flag 1) (break)))
        (if (== flag 0)
            (csym::fprintf stderr "THE ANSWER IS RIGHT!~%"))

        (csym::fprintf stderr "------------------- Input Matrices X and Y ---------------------------~%~%")
        (for ((= i 0) (< i N) (inc i))
            (for ((= j 0) (< j N) (inc j))
                (csym::fprintf stderr "%5.4f  " (aref X (+ (* i N) j))))
            (csym::fprintf stderr "~%"))
        (csym::fprintf stderr "~%")  

        (for ((= i 0) (< i N) (inc i))
            (for ((= j 0) (< j N) (inc j))
                (csym::fprintf stderr "%5.4f  " (aref Y (+ (* i N) j))))
            (csym::fprintf stderr "~%"))
        (csym::fprintf stderr "~%")  

        (csym::fprintf stderr "------------------- Output Matrices Z and W ---------------------------~%~%")
        (for ((= i 0) (< i N) (inc i))
            (for ((= j 0) (< j N) (inc j))      
                (csym::fprintf stderr "%5.4f  " (aref Z (+ (* i N) j))))
            (csym::fprintf stderr "~%"))
        (csym::fprintf stderr "~%")      
        
        (for ((= i 0) (< i N) (inc i))
            (for ((= j 0) (< j N) (inc j))      
                (csym::fprintf stderr "%5.4f  " (aref W (+ (* i N) j))))
            (csym::fprintf stderr "~%"))
        (csym::fprintf stderr "~%")  

        (csym::free W)
        (csym::free Z)
        (csym::free Y)
        (csym::free X)

    (csym::fprintf stderr "-------------------------------------------------------------------------~%~%~%"))

(def (csym::Block_matMul X Y Z m)(fn void (ptr double) (ptr double) (ptr double) int)
(def BLOCKSIZE int (/ m 2))
(def i int 0)
(def j int 0)
(def k int 0)

    (if (== m 1)
        (begin
            (= (mref Z) (* (mref X) (mref Y)))
            (return)) 
        // (begin
        //     (for ((= i 0) (< i m) (+= i BLOCKSIZE))
        //         (for (= j 0) (< j m) (+= j BLOCKSIZE)
        //             (for (= k 0) (< k m) (+= k BLOCKSIZE)
        //                 (csym::Block_matMul (aref X (+ (* i m) k)) (aref Y (+ (* k m) j)) (aref Z (+ (* i m) j)) (cast int BLOCKSIZE))
        //             ))))
        //                 ;;  (aref arr y11)
        ))

                



(def (csym::matMul A B C n)(fn void (ptr double) (ptr double) (ptr double) int)
(def row int 0)
(def col int 0)
(def k int 0)
(def i int 0)
(def j int 0)
(def sum double)

(for ((= i 0) (< i n) (inc i))
        (for ((= j 0) (< j n) (inc j))
            (= sum 0.0)
            (for ((= k 0) (< k n) (inc k))
                (+= sum (* (aref A (+ (* i n) k)) (aref B (+ (* k n) j))))
            )
            (= (aref C (+ (* i n) j)) sum)
        )))            

;; (def (csym::matAdd A B C m) (fn void (ptr double) (ptr double) (ptr double) int)
;; (def row int 0)
;; (def col int 0)

;; (for ((= row 0) (< row m) (inc row))
;;         (for ((= col 0) (< col m) (inc col))
;;             (= (aref C (+ (* row m) col)) (+ (aref A (+ (* row m) col)) (aref B (+ (* row m) col))))
;;         )))

(def (csym::get-wall-time) (fn double void)
  (decl time (struct timeval))
  (if (csym::gettimeofday (ptr time) NULL) (begin (return 0)))
  (return (+ (cast double (fref time tv-sec)) (* (cast double (fref time tv-usec)) 0.000001))))