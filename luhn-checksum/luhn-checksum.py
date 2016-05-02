#!/usr/bin/env python
#
# Checks the validity of an input card number using the Luhn algorithm.
#
# Examples:
# $ python luhn-checksum.py
# Card number: 4561261212345467
# Card number is OK

def split_d(strn):
    return list(map(int,strn))

def get_luhn_checksum(digits):
    odd_indexes = [x for i, x in enumerate(digits) if i % 2 != 0]
    even_indexes = [x*2 for i, x in enumerate(digits) if i % 2 == 0]
    return sum(odd_indexes) + sum(map(lambda x: x - 9 if x > 9 else x, even_indexes))

# checksum must be a multiple of 10
def is_valid(checksum):
    return checksum % 10 == 0

card_n = raw_input("Card number: ")

print "Card number is " + (is_valid(get_luhn_checksum(split_d(card_n))) and "OK" or "Wrong")

# -*- python -*-
