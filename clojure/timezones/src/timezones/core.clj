(ns timezones.core
  (:gen-class)
  (:use clojure.pprint))

(defn timezones
  []
  (for [timezone-id (java.util.TimeZone/getAvailableIDs)]
    (let [timezone (java.util.TimeZone/getTimeZone timezone-id)
          raw-offset (.getRawOffset timezone)
          hours-offset (.toHours (java.util.concurrent.TimeUnit/MILLISECONDS) raw-offset)
          minutes-offset (.toMinutes (java.util.concurrent.TimeUnit/MILLISECONDS) raw-offset)]
      {:id (.getID timezone)
       :offset {:hours hours-offset
                :minutes (- minutes-offset
                            (.toMinutes (java.util.concurrent.TimeUnit/HOURS) hours-offset))}})))

(defn get-timezones
  [hour]
  (filter #(= hour
              (.get
               (java.util.Calendar/getInstance (java.util.TimeZone/getTimeZone (:id %)))
               java.util.Calendar/HOUR_OF_DAY))
          (timezones)))

(defn -main
  [& args]
  (let [hour (Integer/parseInt (first args))]
    (pprint (get-timezones hour))))
