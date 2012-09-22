#!/usr/bin/env perl

package Fool::Deck {
  use common::sense;

  use List::Util qw[ shuffle ];

  use Data::Printer;

  use constant {
    CARDTYPE  => { D => 'DIAMONDS',
                   C => 'CLUBS'   ,
                   S => 'SPADES'  ,
                   H => 'HEARTS'  },
    # CARDRANGE => [ '6', '7', '8', '9', '10', 'JACK', 'QUEEN', 'KING', 'ACE' ],

    CARDRANGE => [ '6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A' ],
  };

  sub new {
    my $class = shift;
    my $self  = bless { @_ } => $class;
    $self->{card_deck}  = [ shuffle $self->make_card_deck ];
    $self->{trump_suit} = $self->{card_deck}[0][0];
    $self
  }

  sub make_card_deck {
    my $self = shift;
    my @cards;
    for my $card_type ( keys %{ +CARDTYPE } ) {
      for my $card_range (@{ +CARDRANGE }) {
        push @cards, [ $card_type => $card_range ];
      }
    }
    @cards
  }

  sub card_deck {
    my $self = shift;
    $self->{card_deck}
  }

  sub trump_suit {
    my $self = shift;
    $self->{trump_suit}
  }

  sub give_card {
    my $self   = shift;
    pop $self->{card_deck};
  }

  sub give_cards($$) {
    my $self   = shift;
    my $number = shift || 1;
    my $cards;
    for (1 .. $number) {
      push @$cards => pop $self->{card_deck};
    }
    $cards
  }

  sub is_card_trump {
    my $self   = shift;
    my $card   = shift;
    ($card->[0] eq $self->trump_suit)
  }

};

package Fool::Player {
  use common::sense;

  use Data::Printer;

  use constant {
    MINCARDS => 6,
  };

  my %card_order_map = map { Fool::Deck::CARDRANGE->[$_] => $_ }
                           0 .. $#{ +Fool::Deck::CARDRANGE };

  sub new {
    my $class = shift;
    my $self  = bless { @_ } => $class;
    $self->{cards} = [];
    $self
  }

  sub take_card {
    my $self = shift;
    my $card = shift;
    push @{ $self->{cards} } => $card;
  }

  sub need_card {
    my $self = shift;
    scalar @{ $self->{cards} } < MINCARDS
  }

  # sub sort_cards {
  #   my $self = shift;
  #   $self->{cards} = [ sort {
  #     $a->[0] cmp $b->[0]
  #     ||
  #     $card_order_map{$a->[1]} <=> $card_order_map{$b->[1]}
  #   } @{ $self->{cards} } ];
  # }

  sub sort_cards {
    my $self = shift;
    $self->{cards} = [ sort {
      $a->[0] cmp $b->[0]
      ||
      $card_order_map{$a->[1]} <=> $card_order_map{$b->[1]}
    } @{ $self->{cards} } ];
  }

  sub empty {
    my $self = shift;
    0 >= scalar @{ $self->{cards} };
  }

  sub get_lowest_card {
    my $self = shift;
    # @TODO implement
    $self->{cards}[0]
  }

  sub get_highest_card {
    my $self = shift;
    # @TODO implement
  }

  sub attack {
    my $self = shift;
    my $desk = shift;
    my $throwed_card;
    unless ($desk->is_empty) {
      $throwed_card = $self->throw_in($desk)
    }
    else {
      $throwed_card = $self->get_lowest_card();
    }
    $desk->push_attack( $self->{id} => $throwed_card )
      if $throwed_card;
  }

  sub defence {
    my $self = shift;
    my $desk = shift;
    # @TODO implement
  }

  sub throw_in {
    my $self = shift;
    my $desk = shift;
    for my $slot ( @{ $desk->{desk} } ) {
      for my $pslot (@$slot) {
        for my $mycard ( @{ $self->{cards} } ) {
          # @TODO check for the trump suit
          return $mycard if $mycard->[1] eq $pslot->[1][1];
        }
      }
    }
  }

  sub can_attack {
    my $self = shift;
    my $desk = shift;
    ($desk->is_empty or
     defined $desk->get_last_defence)
  }

};

package Fool::Desk {
  use common::sense;

  use Data::Printer;

  sub new {
    my $class = shift;
    my $self  = bless {
      @_
    } => $class;
    $self->{desk} = [];
    $self
  }

  #
  # [
  #  [[1 => ["D",6]], [2 => ["D", 7 ]]],
  #  [[1 => ["S",6]], [2 => ["S","J"]]],
  # ]
  #
  sub push_attack {
    my $self   = shift;
    my $player = shift;
    my $card   = shift;
    push @{ $self->{desk} } => [[$player => $card]];
    $self
  }

  sub push_defence {
    my $self   = shift;
    my $player = shift;
    my $card   = shift;
    # @TODO check
    push @{ $self->{desk}[-1] } => [$player => $card];
    $self
  }

  sub get_last_slot {
    my $self   = shift;
    return if $self->is_empty;
    $self->{desk}[-1]
  }

  sub get_last_attack {
    my $self   = shift;
    $self->get_last_slot->[0]
  }

  sub get_last_defence {
    my $self   = shift;
    $self->get_last_slot->[1]
  }

  sub is_empty {
    my $self = shift;
    0 >= scalar @{ $self->{desk} }
  }

  sub clear {
    my $self = shift;
    $self->{desk} = [];
    $self
  }

};

package Fool {
  use common::sense;

  use Data::Printer;

  my $gamers = 2;
  my $act = 0;

  sub new {
    my $class = shift;
    my $self  = bless {
      @_
    } => $class;
    for my $player (@{ $self->{player} }) {
      $self->deal_cards($player);
      $player->sort_cards;
    }
    $self
  }

  sub deal_cards {
    my $self   = shift;
    my $player = shift;
    while ($player->need_card) {
      $player->take_card( $self->{deck}->give_card );
    }
  }

  sub curr_player {
    my $self = shift;
    $act
  }

  sub next_player {
    my $self = shift;
    return ++$act > $#{ $self->{player} } ? 0 : $act;
  }

  sub act {
    my $self = shift;
    my $curr_player = shift || $self->curr_player;
    my $next_player = shift || $self->next_player;
    while ($self->{player}[$curr_player]->can_attack($self->{desk}) and
           $self->{player}[$curr_player]->attack($self->{desk}))
    {
      $self->{player}[$next_player]->defence($self->{desk});
    }
    $self->{desk}->clear();
  }

  sub finish {
    1 == scalar grep { ! $_->empty } @{ $self->{player} };
  }

  sub start {
    my $self = shift;
    while ( ! $self->finish ) {
      $self->act;
      sleep 1;
    }
  }

};

package main {
  use common::sense;

  use Data::Printer;

  my $fool = new Fool(
    deck   => Fool::Deck->new(),
    desk   => Fool::Desk->new(),
    player => [ Fool::Player->new(id => 0) ,
                Fool::Player->new(id => 1) ],
  );
  p($fool->{deck}->trump_suit);
  p($fool->{player}[0]);

  # $fool->start;
};

__END__
