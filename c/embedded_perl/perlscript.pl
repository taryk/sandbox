sub perl_sub_one {
  my $string = shift;
  # just reverse a string
  return scalar reverse $string;
}

sub perl_sub_two {
  my $string = shift;
  my $len = length($string) - 1;
  # reverse a second part of string
  for (my $i = $len; $i > $len/2; $i--) {
    substr($string, $i, 1, substr($string, $len-$i, 1));
  }
  return $string;
}

sub perl_sub_three {
  my $string = shift;
  my $len = length($string) - 1;
  # reverse a first part of string
  for (my $i = 0; $i < $len/2; $i++) {
    substr($string, $i, 1, substr($string, $len-$i, 1));
  }
  return $string;
}
