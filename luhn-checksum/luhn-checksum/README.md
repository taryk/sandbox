# luhn-checksum

The program checks the validity of an input card number using the [Luhn algorithm](http://en.wikipedia.org/wiki/Luhn_algorithm)

Just run

    $ lein run
    Card number: 4561261212345467
    Card number is OK

To build a standalone jar and run it

    $ lein uberjar
    ...
    $ java -jar target/luhn-checksum-0.1.0-SNAPSHOT-standalone.jar
    Card number: 4561261212345467
    Card number is OK

You can use [LUHN-10 Error Detection Tool](http://www.ee.unb.ca/cgi-bin/tervo/luhn.pl) to check the card number online.

## License

Copyright © 2012 Taras Yagniuk <mrtaryk@gmail.com>

Distributed under the Eclipse Public License, the same as Clojure.
