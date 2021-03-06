(ns arabic-roman-numerals.core
  (:gen-class :main true))

(def roman-numbers-map '(("M" 1000)
                         ("CM" 900)
                         ("D" 500)
                         ("CD" 400)
                         ("C" 100)
                         ("XC" 90)
                         ("L" 50)
                         ("XL" 40)
                         ("X" 10)
                         ("IX" 9)
                         ("V" 5)
                         ("IV" 4)
                         ("I" 1)))

(defn to-roman [number]
  "Convert integer to Roman numeral"
  (loop [i 0
         number number
         roman nil]
    (if (< i (count roman-numbers-map))
      (let [x (nth roman-numbers-map i)]
        (if (>= number (first (rest x)))
          (recur 0
                 (- number (first (rest x)))
                 (str roman (first x)))
          (recur (inc i)
                 number
                 roman)))
      (if (<= number 0) roman))))

(defn prompt-read [str]
  "Prompt for Integer"
  (print str)
  (flush)
  (Integer/parseInt (read-line)))
  
(defn -main
  [& args]
  (-> (prompt-read "number: ")
      to-roman
      println))
