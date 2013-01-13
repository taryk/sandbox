# luhn-checksum

The program checks the validity of an input card number using the [Luhn algorithm](http://en.wikipedia.org/wiki/Luhn_algorithm)

Just run ( sbcl )

    $ sbcl --script luhn-checksum.lisp
    Card number: 4561261212345467
    Card number is: OK

or using clisp

    $ clisp luhn-checksum.lisp
    Card number: 5610591081018250
    Card number is: OK    

You can use [LUHN-10 Error Detection Tool](http://www.ee.unb.ca/cgi-bin/tervo/luhn.pl) to check the card number online.

## License

Copyright © 2013 Taras Yagniuk <mrtaryk@gmail.com>
