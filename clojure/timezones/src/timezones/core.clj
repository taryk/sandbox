(ns timezones.core
  (:gen-class)
  (:import [java.util TimeZone Calendar]
           [java.util.concurrent TimeUnit])
  (:use clojure.pprint))

(defn timezones
  "Returns a list of all timezones with hour/minute offset"
  []
  (for [timezone-id (TimeZone/getAvailableIDs)]
    (let [timezone (TimeZone/getTimeZone timezone-id)
          raw-offset (.getRawOffset timezone)
          hours-offset (.toHours (TimeUnit/MILLISECONDS) raw-offset)
          minutes-offset (.toMinutes (TimeUnit/MILLISECONDS) raw-offset)]
      {:id (.getID timezone)
       :offset {:hours hours-offset
                :minutes (- minutes-offset
                            (.toMinutes (TimeUnit/HOURS) hours-offset))}})))

(defn get-timezones
  "Filter timezones by a passed current hour"
  [hour]
  (filter #(= hour
              (.get (Calendar/getInstance (TimeZone/getTimeZone (:id %)))
                    Calendar/HOUR_OF_DAY))
          (timezones)))

(defn -main
  [& args]
  (let [hour (Integer/parseInt (first args))]
    (pprint (get-timezones hour))))
