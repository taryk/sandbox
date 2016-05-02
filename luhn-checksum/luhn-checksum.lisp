;; Checks the validity of an input card number using the Luhn algorithm.
;;
;; Examples:
;;
;; $ sbcl --script luhn-checksum.lisp
;; Card number: 4561261212345467
;; Card number is: OK
;;
;; or
;;
;; $ clisp luhn-checksum.lisp
;; Card number: 5610591081018250
;; Card number is: OK

(defun get-luhn-checksum (number-str)
  (let ((n (if (= (rem (length number-str) 2) 0) -1 0)))
    (apply #'+
           (map 'list #'(lambda(char)
                          (let ((d (parse-integer (string char))))
                            (if (= (rem (incf n) 2) 0)
                                (let ((d2 (* d 2)))
                                  (if (> d2 9) (- d2 9) d2))
                                d)))
                (coerce number-str 'list)))))

(defun is-valid (checksum)
  (= (rem checksum 10) 0))

(defun prompt-for-number ()
  (format *query-io* "Card number: ")
  (force-output *query-io*)
  (read-line *query-io*))

(format *query-io* "Card numer is: ~a~C"
        (if (is-valid (get-luhn-checksum (prompt-for-number))) "OK" "Wrong") 
        #\linefeed)
