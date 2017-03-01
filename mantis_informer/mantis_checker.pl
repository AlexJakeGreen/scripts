#!/usr/bin/perl
use strict;
use warnings;
#BEGIN {
#    $ENV{PERL_LWP_SSL_VERIFY_HOSTNAME} = 0;
#};

###############################################################
###
### TO::MantisWSDL
###
###############################################################

package TO::MantisWSDL;
use common::sense;
use Carp;
use IO::Socket::SSL qw( SSL_VERIFY_NONE );
use SOAP::Lite; # +trace => 'all';
use Data::Dumper;

sub new {
    my $class = shift;
    my $self  = {
        proxy_url =>
          "https://localhost/api/soap/mantisconnect.php",
        user_name       => "",
        user_pass       => "",
        project_id      => 0,
        filter_id       => undef,
        page_number     => 1,
        issues_per_page => 10,
    };
    bless $self, $class;

    $self->init_soap_lite();

    return $self;
}

sub init_soap_lite {
    my $self = shift;
    my $soap = SOAP::Lite->new();
    
    $soap->proxy( $self->{proxy_url} );

    $soap->transport->ssl_opts(
        SSL_verify_mode => IO::Socket::SSL::SSL_VERIFY_NONE,
        verify_hostname => 0 );

    $self->{soap} = $soap;
}

sub mc_filter_get_issue_headers {
    my $self = shift;
    my $soap = $self->{soap};
    my $som  = $soap->call(
        'mc_filter_get_issue_headers', $self->{user_name},
        $self->{user_pass},            $self->{project_id},
        $self->{filter_id},            $self->{page_number},
        $self->{issues_per_page}
    );

    die( $som->faultstring() ) if ( $som->fault() );
    my (@result) = $som->result() or die('No result');

    return \@result;
}

###############################################################
###
### TO::MantisInformer
###
###############################################################

package TO::MantisInformer;
use common::sense;
use Carp;
use Gtk2;
use Gtk2::TrayIcon;
use Data::Dumper;

sub new {
    my $class = shift;
    my $self  = shift;
    bless $self, $class;

    my $wd = $self->{work_dir};

    my $img_pri = {
        20 => $wd . "/priority_low_1.gif",
        30 => $wd . "/priority_normal.gif",
        40 => $wd . "/priority_1.gif",
        50 => $wd . "/priority_2.gif",
        60 => $wd . "/priority_3.gif",
    };

    $self->{COLOR_BG}     = 'None';
    $self->{COLOR_FG}     = '#303030';
    $self->{COLOR_BORDER} = 'None';

    $self->{log_level4} = 1;

#<<< do not let perltidy touch this
    $self->{nums}         = {
        '-w' => 6,
        '-h' => 15,
        '0'  => [
            ' oooo ',
            'o oo o',
            'oo  oo',
            'oo  oo',
            'oo  oo',
            'oo  oo',
            'o    o',
            '      ',
            'o    o',
            'oo  oo',
            'oo  oo',
            'oo  oo',
            'oo  oo',
            'o oo o',
            ' oooo '
        ],
        '1' => [
            '      ',
            '     o',
            '    oo',
            '    oo',
            '    oo',
            '    oo',
            '     o',
            '      ',
            '     o',
            '    oo',
            '    oo',
            '    oo',
            '    oo',
            '     o',
            '      '
        ],
        '2' => [
            ' oooo ',
            '  oo o',
            '    oo',
            '    oo',
            '    oo',
            '    oo',
            '  oo o',
            ' oooo ',
            'o oo  ',
            'oo    ',
            'oo    ',
            'oo    ',
            'oo    ',
            'o oo  ',
            ' oooo '
        ],
        '3' => [
            ' oooo ',
            '  oo o',
            '    oo',
            '    oo',
            '    oo',
            '    oo',
            '  oo o',
            ' oooo ',
            '  oo o',
            '    oo',
            '    oo',
            '    oo',
            '    oo',
            '  oo o',
            ' oooo '
        ],
        '4' => [
            '      ',
            'o    o',
            'oo  oo',
            'oo  oo',
            'oo  oo',
            'oo  oo',
            'o oo o',
            ' oooo ',
            '  oo o',
            '    oo',
            '    oo',
            '    oo',
            '    oo',
            '     o',
            '      '
        ],
        '5' => [
            ' oooo ',
            'o oo  ',
            'oo    ',
            'oo    ',
            'oo    ',
            'oo    ',
            'o oo  ',
            ' oooo ',
            '  oo o',
            '    oo',
            '    oo',
            '    oo',
            '    oo',
            '  oo o',
            ' oooo '
        ],
        '6' => [
            ' oooo ',
            'o oo  ',
            'oo    ',
            'oo    ',
            'oo    ',
            'oo    ',
            'o oo  ',
            ' oooo ',
            'o oo o',
            'oo  oo',
            'oo  oo',
            'oo  oo',
            'oo  oo',
            'o oo o',
            ' oooo '
        ],
        '7' => [
            ' oooo ',
            '  oo o',
            '    oo',
            '    oo',
            '    oo',
            '    oo',
            '     o',
            '      ',
            '     o',
            '    oo',
            '    oo',
            '    oo',
            '    oo',
            '     o',
            '      '
        ],
        '8' => [
            ' oooo ',
            'o oo o',
            'oo  oo',
            'oo  oo',
            'oo  oo',
            'oo  oo',
            'o oo o',
            ' oooo ',
            'o oo o',
            'oo  oo',
            'oo  oo',
            'oo  oo',
            'oo  oo',
            'o oo o',
            ' oooo '
        ],
        '9' => [
            ' oooo ',
            'o oo o',
            'oo  oo',
            'oo  oo',
            'oo  oo',
            'oo  oo',
            'o oo o',
            ' oooo ',
            '  oo o',
            '    oo',
            '    oo',
            '    oo',
            '    oo',
            '  oo o',
            ' oooo '
        ],
        '-deg' => [
            ' oo ',
            'o  o',
            'o  o',
            ' oo ',
            '    ',
            '    ',
            '    ',
            '    ',
            '    ',
            '    ',
            '    ',
            '    ',
            '    ',
            '    '
        ],
        '-logo' => [
            'ooo       o',
            '  oo oo oo ',
            '   oo  oo  ',
            '  oo    oo ',
            '  o o  o o ',
            '  oo     o ',
            '   oo   oo ',
            '    oo   o ',
            '    o oo  o',
            '  o  o o   ',
            ' o o  o o  ',
            ' o  o o   o',
            ' oo  oo   o',
            ' o o      o',
            '  o o   oo '
        ]
    };

#>>>

    $self->{img_pri} = $img_pri;

    $self->{wsdl} = TO::MantisWSDL->new();

    $self->{wsdl}->{user_name} = $self->{user_name};
    $self->{wsdl}->{user_pass} = $self->{user_pass};
    $self->{wsdl}->{filter_id} = $self->{filter_id};
    $self->{wsdl}->{issues_per_page} = $self->{issues_per_page};

    return $self;
}

sub logger {
    my $self      = shift;
    my $message   = shift;
    
=head
### debug messages
log_level1 = 0 # all user input (clicks, gestures, text input)
log_level2 = 0 # "i'm here" from significant subroutines, plus any queries that get executed
log_level3 = 0 # diagnostic info from significant algorithms, plus summarized results of any queries
log_level4 = 1 # other messages
=cut
    my $log_level = shift || 4;
    return unless $self->{'log_level' . $log_level};

    my $timestamp = $self->get_logging_time();

    open my $fh, ">>", $self->{work_dir} . "/" . $self->{log_file}
      or die "Cant access log file $!\n";
    print $fh $timestamp . " " . $message . "\n";
    close $fh;
}

sub run {
    my $self = shift;

    Gtk2->init();

    $self->{icon}     = Gtk2::TrayIcon->new("MI count applet");
    $self->{eventbox} = Gtk2::EventBox->new();

    #    $self->{label}    = Gtk2::Label->new("0");
    $self->{img} = Gtk2::Image->new();

    #    $self->{eventbox}->add( $self->{label} );
    $self->{eventbox}->add( $self->{img} );
    $self->{icon}->add( $self->{eventbox} );
    $self->{tt}   = Gtk2::Tooltips->new();
    $self->{menu} = Gtk2::Menu->new();

    $self->{eventbox}
      ->signal_connect( 'button-press-event' => sub { $self->click(@_); } );

    $self->label_refresh();

    Glib::Timeout->add( $self->{timeout}, sub { $self->label_refresh(); 1; } );

    $self->{icon}->show_all();
    Gtk2->main();
    exit $?;
}

sub set_tooltip {
    my $self = shift;
    my $text = "Impl_Ops queue: " . $self->{count} . "\n";
    for ( my $i = 0 ; $i < $self->{count} ; $i++ ) {
        my $it = $self->{rp}->[$i];
        $text .= "\n" . $it->{id} . " " . $it->{summary};
    }
    $self->{tt}->set_tip( $self->{eventbox}, $text );
}

sub get_data {
    my $self = shift;

    my $w = $self->{wsdl};
    $self->logger("performing wsdl call for headers of tickets...");
    my $result = $w->mc_filter_get_issue_headers();
    $self->logger("wsdl call performed");
    $self->{old_rp} = $self->{rp};
    $self->{rp}     = $result->[0];

    $self->check_changes();

    return $result;
}

sub label_refresh {
    my $self = shift;
    $self->get_data();
    my $data = $self->{rp};
    my $cnt  = scalar @{$data};
    $self->{count} = $cnt;
    $self->logger( "Label refresh " . $cnt );

    #$self->{label}->set_label( "Tickets: " . $cnt );
    $self->icon_refresh($cnt);
    $self->set_tooltip();

}

sub icon_refresh {
    my $self = shift;
    my $val = int( shift() || 0 );

    #  return if ($old_temp == $val);
    #  $old_temp = $val;
    my $numbers = $self->{nums};
    my $w       = 48;
    my $h       = 24;
    my ( $i, $j, $k ) = ( 0, 0, 0 );
    my $pixbuf = $self->{img}->get_pixbuf();
    my @data   = (
        sprintf( '%d %d 4 1', $w, $h ),
        '# c None',
        '  c ' . $self->{COLOR_BG},
        'o c ' . $self->{COLOR_FG},
        '. c ' . $self->{COLOR_BORDER}
    );

    push( @data, '#' x $w );
    push( @data, '##' . ( '.' x ( $w - 4 ) ) . '##' );
    $j++;
    my $h_top = int( ( $h - $numbers->{'-h'} ) / 2 + 0.5 );
    while ( $j <= $h_top - 2 ) {
        push( @data, '#.' . ( ' ' x ( $w - 4 ) ) . '.#' );
        $j++;
    }
    my @nums = split( //, int($val) . "" );
    my $w_left = int( ( $w - ( $numbers->{'-w'} * ( scalar(@nums) - 1 ) + ( scalar(@nums) - 1 ) ) ) / 2 ) - length( $numbers->{'-deg'}->[0]) - length( $numbers->{'-logo'}->[0] );
    push( @nums, '-deg' );
    push( @nums, '-logo');
    $k = 0;
    while ( $k < $numbers->{'-h'} ) {
        my $ln = '#.' . ( ' ' x ( $w_left - 2 ) );
        foreach (@nums) {
            $ln .= $numbers->{$_}->[$k] || ( ' ' x ( $numbers->{'-w'} - 2 ) );
            $ln .= ' ';
        }
        $k++;
        $ln .= ( ' ' x ( $w - length($ln) - 2 ) ) . '.#';
        push( @data, $ln );
        $j++;
    }
    while ( $j < $h - 3 ) {
        push( @data, '#.' . ( ' ' x ( $w - 4 ) ) . '.#' );
        $j++;
    }
    push( @data, '##' . ( '.' x ( $w - 4 ) ) . '##' );
    push( @data, '#' x $w );
    
#    print "$_\n" for(@data);
    
    my $tmp = Gtk2::Gdk::Pixbuf->new_from_xpm_data(@data);
    $self->{img}->set_from_pixbuf($tmp);
}

sub check_changes {
    my $self        = shift;
    my $old_tickets = {};
    foreach my $i ( @{ $self->{old_rp} } ) { $old_tickets->{ $i->{id} } = 1; }

    my @new_tickets;
    foreach my $i ( @{ $self->{rp} } ) {
        push @new_tickets, $i if not defined( $old_tickets->{ $i->{id} } );
    }

    $self->notify_send( \@new_tickets ) if @new_tickets;
}

sub notify_send {
    my $self = shift;
    my $data = shift;
    my $text = "";
    foreach my $i (@$data) {
        my $id      = $i->{id};
        my $summary = $i->{summary};

        $summary =~
          s;[^a-zA-Z_0-9:]; ;g;    ### dont pass special characters to dbus
        $summary =~ s;^(.{30}).+$;$1...;;

        $text .= $id . " " . $summary . "\n";
    }
    system("notify-send 'New ticket(s) assigned' '$text'");
}

sub click {
    my $self = shift;
    if ( $_[1]->button == 1 ) {
        ###
        $self->logger("LMK pressed", 1);
        $self->show_menu_tickets();
    }
    elsif ( $_[1]->button == 2 ) {
        ###
        $self->logger("MMK pressed", 1);
    }
    elsif ( $_[1]->button == 3 ) {
        ###
        $self->logger("RMK pressed", 1);
        $self->config_it();
    }
    return 1;
}

sub show_menu_tickets {
    my $self = shift;
    $self->logger("show_tickets called");
    my $menu = Gtk2::Menu->new();
    my @entries;

    my $img = $self->{img_pri};

    for ( my $i = 0 ; $i < $self->{count} ; $i++ ) {
        my $it = $self->{rp}->[$i];

        ### Create label
        $entries[$i] = Gtk2::ImageMenuItem->new_with_label( $it->{summary} );

        ### Add priority icon
        $entries[$i]
          ->set_image( Gtk2::Image->new_from_file( $img->{ $it->{priority} } ) )
          if defined( $img->{ $it->{priority} } );

        $entries[$i]->signal_connect(
            activate => sub {
                system( "firefox '" . $self->{ticket_url} . $it->{id} . "'" );
                1;
            }
        );
        $menu->add( $entries[$i] );
    }
    $menu->show_all;
    $menu->popup( undef, undef, undef, undef, 0, 0 );
}

sub config_it {
    my $self = shift;
    $self->logger("Config menu called");
    my $menu = Gtk2::Menu->new();

    ###Preferences
    my $menu_pref = Gtk2::ImageMenuItem->new_with_label("Configure");
    $menu_pref->signal_connect( activate => \&configure_it );
    $menu_pref->set_image(
        Gtk2::Image->new_from_stock( 'gtk-preferences', 'menu' ) );
    $menu->add($menu_pref);

    ### Separator
    my $menu_sep = Gtk2::SeparatorMenuItem->new();

    ###Quit
    my $menu_quit = Gtk2::ImageMenuItem->new_with_label("Quit");
    $menu_quit->signal_connect( activate => \&exit_it );
    $menu_quit->set_image( Gtk2::Image->new_from_stock( 'gtk-quit', 'menu' ) );
    $menu->add($menu_quit);

    ###Show menu
    $menu->show_all;
    $menu->popup( undef, undef, undef, undef, 0, 0 );
    return 1;
}

sub configure_it {
    return 1;
}

sub exit_it {
    exit 0;
}

sub get_logging_time {

    my ( $sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst ) =
      localtime(time);
    my $timestamp = sprintf(
        "[%04d-%02d-%02d %02d:%02d:%02d]",
        $year + 1900,
        $mon + 1, $mday, $hour, $min, $sec
    );
    return $timestamp;
}

###############################################################
###
### MAIN
###
###############################################################

package main;
use common::sense;
use Cwd "abs_path";
use Data::Dumper;

### check config should be here!

my $work_dir = abs_path($0);
$work_dir =~ s;/[^/]+$;;;

my $config_file = $work_dir . "/" . "mantis_checker.cfg";

my $config = load_config($config_file);
$config->{work_dir} = $work_dir;

daemonize() if $config->{daemonize};

my $i = TO::MantisInformer->new($config);
$i->run();

sub load_config {
    my $file = shift;
    my $c    = {};
    open my $fh, "<", $file or return undef;
    while (<$fh>) {
        s;^\s*#.+$;;;
        next if m;^$;;
        my ( $key, $val ) = (m;^\s*([^\s]+)\s*=\s*([^\s]+)\s*$;);
        $c->{$key} = $val;
    }
    close $fh;

    $c->{user_name} = _ask_user( "Mantis login: ", 0 ) unless $c->{user_name};
    $c->{user_pass} = _ask_user( "Mantis password: ", 1 )
      unless $c->{user_pass};

    return $c;
}

sub _ask_user {
    my $question = shift;
    my $hide_input = shift || 1;
    my $input;
    print $question;
    system( 'stty', '-echo' ) if $hide_input;
    chop( $input = <STDIN> );
    system( 'stty', 'echo' ) if $hide_input;
    print "\n";
    return $input;
}

sub daemonize {
    use POSIX;
    POSIX::setsid or die "setsid: $!\n";

    #    warn "trying to pid \n";
    my $pid = fork();

    #    warn "pid: $pid";
    if    ( $pid < 0 ) { die "fork: $!"; }
    elsif ($pid)       { exit 0; }
    chdir "/";
    umask 0;

#    warn "after umask";
#    foreach (0 .. (POSIX::sysconf (&POSIX::_SC_OPEN_MAX) || 1024)) { POSIX::close $_; }

    open( STDIN,  "</dev/null" );
    open( STDOUT, ">/dev/null" );
    open( STDERR, ">/dev/null" );
}

__END__


11  log levels
10  Logo added
9   filter_id moved to cfg file
8   Label replaced with Icon
7   SOAP::WSDL
