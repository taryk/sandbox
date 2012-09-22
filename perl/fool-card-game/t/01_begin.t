#!/usr/bin/env perl

use common::sense;

package Fool::Test {
  use base 'Test::Class';
  use Test::More;

  use common::sense;

  use lib '../';

  sub start : Test(startup) {
  }

  sub before_test : Test(setup) {
  }

  sub test_01 : Test {
  }

  sub after_test : Test(shutdown) {
  }

  sub finish : Test(shutdown) {
  }

}

package main {
  Fool::Test->runtests;
}

__END__
