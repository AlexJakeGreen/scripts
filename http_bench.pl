#!/usr/bin/perl
use strict;
use warnings;
use AnyEvent::HTTP;
use POSIX;
use Getopt::Long;
use Pod::Usage;

#use AnyEvent::CacheDNS ':register';

# Our target
my $url = undef;

# verbosity
# 0 - stats only
# 1 - show what the script does
# 2 - show responce from remote server
my $verbose = 1;

# The time-out to use for various stages
#   each connect attempt will reset the timeout,
#   as will read or write activity,
#   i.e. this is not an overall timeout.
my $timeout = 20;

# How many requests should be performed
my $count = 5000;

# How many requests we send at one time,
# i.e. how many our processes on web server
my $concurrency = 60;

# method to connect. GET, POST..
my $method = 'GET';

# How many redirects to follow
my $max_recurse = 10;

# user agent string. Webserver may block us after several request by this string
my $useragent = 'Mozilla/5.0 (compatible; U; AnyEvent::HTTP)';

my $help;
my $man;

my $result = GetOptions(
    'help|?'        => \$help,
    'man'           => \$man,
    'url=s'         => \$url,
    'verbose=i'     => \$verbose,
    'timeout=i'     => \$timeout,
    'count=i'       => \$count,
    'concurrency=i' => \$concurrency,
    'method=s'      => \$method,
    'recurse=i'     => \$max_recurse,
    'useragent=s'   => \$useragent,
) or pod2usage(2);

pod2usage(1) if $help;
pod2usage( -exitstatus => 0, -verbose => 2 ) if $man;

unless ($url) {
    pod2usage(
        -verbose => 0,
        -message => "$0: Mandatory argument '--url http://hostname' is missing"
    );
    exit;
}


############################################################################
### DONT EDIT
############################################################################

my $done = 0;

my @reqs_time;

$AnyEvent::HTTP::MAX_PER_HOST = $concurrency;
$AnyEvent::HTTP::USERAGENT    = $useragent;

$SIG{INT} = \&end_bench;

my $cv = AnyEvent->condvar;

my $start_time = AnyEvent->time();
print "Started at ", format_time($start_time), "\n";

for ( 1 .. $concurrency ) {
    add_request( $_, $url );
}

$cv->recv;

end_bench();

############################################################################
### SUBS
############################################################################

sub add_request {
    my ( $id, $url ) = @_;
    my $req_time = AnyEvent->time;
    http_request $method => $url,
      timeout            => $timeout,
      cookie_jar         => {},
      recurse            => $max_recurse,
      sub {
        my $completed = AnyEvent->time;
        my $req_time  = format_seconds( $completed - $req_time );
        print "Got answer in $req_time seconds." if $verbose;
        push @reqs_time, $req_time;
        $done++;

        if ( $verbose >= 2 ) {
            print "=========== HTTP RESPONSE ===========\n";
            print $_[0];
        }
        my $hdr = $_[1];
        if ( $hdr->{Status} =~ /^2/ ) {
            print " Done $done\n" if $verbose;
        }
        else {
            print STDERR
"\nOops we get problem in  request  . $done  . ($hdr->{Status}) . ($hdr->{Reason}) \n";
        }

        return add_request( $done, $url ) if $done < $count;
        $cv->send;
      }
}

sub end_bench {
    my $end_time     = AnyEvent->time;
    my $overall_time = format_seconds( $end_time - $start_time );
    print "\n\nDONE\n

INITIAL PARAMETERS:
\tURL: $url
\tTimeout: $timeout
\tTotal requests: $count
\tConcurrency: $concurrency
\tHTTP method: $method
\tMAX recursion of redirects: $max_recurse,
\tUser agent string: $useragent
";

    print "\nRESULTS:\n\tTotal time: $overall_time seconds\n";
    my $sum;

    print "\n\tRequests per second: "
      . sprintf( "%.2f", $count / $overall_time ) . "\n";
    @reqs_time = sort { $a <=> $b } (@reqs_time);

    for my $i ( 0 .. scalar(@reqs_time) ) {
        $sum += $reqs_time[$i] if defined( $reqs_time[$i] );
    }

    print "\n\tShortest is :  $reqs_time[0]  sec. \n";
    print "\tAverage time is : " . format_seconds( $sum / $count ) . " sec. \n";
    print "\tLongest is :  $reqs_time[scalar(@reqs_time)-1] sec. \n\n";

    exit;
}

sub format_time {
    my ( $microsec, $seconds ) = modf(shift);
    my ( $sec, $min, $hour ) = localtime($seconds);
    return sprintf "%02d:%02d:%02d.%04d", $hour, $min, $sec,
      int( $microsec * 10000 );
}

sub format_seconds {
    return sprintf "%.4f", shift;
}

__END__

=head1 NAME

Simple benchmark tool

=head1 SYNOPSIS

./script.pl --url http://hostname [options]

=head1 OPTIONS

=over 8

=item B<--url> <http://host...>

Our target URL. Required.

=item B<--count> N

How many requests should be performed at all

=item B<--concurrency> N

How many requests we send at one time,

=item B<--method> GET

method to connect. GET, POST..

=item B<--recurse> N

How many redirects to follow. Login portal needs 2

=item B<--useragent> "Hello from martians"

user agent string. Webserver may block us after several request by this string
Default 'Mozilla/5.0 (compatible; U; AnyEvent::HTTP)';

=item B<--verbose> N

How much information print to stdout
0 - stats only
1 - show what the script does
2 - show responce from remote server

=item B<--timeout> N

The time-out to use for various stages
   each connect attempt will reset the timeout,
   as will read or write activity,
   i.e. this is not an overall timeout.

=item B<--help>

brief help message

=item B<--man>

full documentation

=back

=head1 DESCRIPTION

B<script.pl> sends http requests using non blocking connections

=head1 TODO

Implement AnyEvent::CacheDNS

Fix error AnyEvent::Util: Runtime error in AnyEvent::guard callback: Can't call method "_put_session" on an undefined value at /usr/lib64/perl5/vendor_perl/5.12.4/x86_64-linux/AnyEvent/Handle.pm line 2163 during global destruction

Implement random user agent string

Implement urls from file

=head1 AUTHORS

    Alexander Litvinov greenulito@gmail.com

=cut

