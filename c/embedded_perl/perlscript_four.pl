use Crypt::PBKDF2;

sub perl_sub {
  my $string = shift;
  my $pbkdf2 = Crypt::PBKDF2->new(
    hash_class => 'HMACSHA1',
    iterations => 1000,
    output_len => 20,
    salt_len   => 4,
  );
  return $pbkdf2->PBKDF2_hex('', $string);
}
