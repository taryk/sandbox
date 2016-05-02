(ns luhn-checksum.core
  (:gen-class :main true))

(defn get-luhn-checksum
  [digits]
  (let [[even-nums odd-nums]
        (->> digits
             (map #(list %1 %2) (iterate inc 0))
             (group-by #(even? (first %)))
             vals)]
    (+ (apply + (map #(let [x (* (nth % 1) 2)] (if (> x 9) (- x 9) x)) even-nums))
       (apply + (map #(nth % 1) odd-nums)))))

(defn valid?
  [checksum]
  (= (rem checksum 10) 0))

(defn read-number [str]
  (print str)
  (flush)
  (read-line))

(defn -main
  [& args]
  (println "Card number is"
           (if (->> (read-number "Card number: ")
                    seq
                    (map #(read-string (str %)))
                    get-luhn-checksum
                    valid?)
             "OK"
             "wrong")))
