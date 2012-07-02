#!/usr/bin/env perl

package Foo {
  use common::sense;

  sub new {
    my $class = shift;
    bless {}, $class;
  }

  sub private_method {
    my $self = shift;
    die "private method "
      unless (caller)[0]->isa( ref $self );

    print "test\n";
  }

  sub public_method {
    my $self = shift;
    $self->private_method();
  }

};

package main {
  use common::sense;

  my $bar = Foo->new();

  $bar->public_method;

  $bar->private_method;

};
