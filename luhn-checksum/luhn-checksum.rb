#!/usr/bin/env ruby
#
# Checks the validity of an input card number using the Luhn algorithm.
#
# Examples:
# $ ruby luhn-checksum.rb
# Card number: 4561261212345467
# Card number is OK

def split_d(strn)
  strn.split("").map{|c| c.to_i}
end

def get_luhn_checksum(digits)
  digits.each_with_index.map{|d,i| [i,d]}.group_by{|x| x[0] % 2 != 0}.map{|odd,set|
    set = set.map{|x| x[1]}
    (odd ? set : set.map{|x| x*=2; x > 9 ? x - 9 : x}).inject{|sum,x| sum+x}
  }.inject{|sum,x| sum+x}
end

def is_valid(checksum)
  checksum % 10 == 0
end

print "Card number: "
card_n = gets

puts "Card number is " + (is_valid(get_luhn_checksum(split_d(card_n))) ? "OK" : "Wrong")

# -*- ruby -*-
