sub perl_sub {
  my $string = shift;
  my $len = length($string) - 1;
  # reverse a first part of string
  for (my $i = 0; $i < $len/2; $i++) {
    substr($string, $i, 1, substr($string, $len-$i, 1));
  }
  return $string;
}
