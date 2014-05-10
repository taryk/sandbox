(defproject timezones "0.1.0-SNAPSHOT"
  :description "Returns a list of timezones where passed value is current hour."
  :license {:name "Eclipse Public License"
            :url "http://www.eclipse.org/legal/epl-v10.html"}
  :dependencies [[org.clojure/clojure "1.5.1"]]
  :main ^:skip-aot timezones.core
  :target-path "target/%s"
  :profiles {:uberjar {:aot :all}})
