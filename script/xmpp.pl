#!/usr/bin/perl

use Authen::SASL qw(Perl);
use Net::XMPP;
use Getopt::Long;

use open ':utf8';
use open ':std';

use strict;
use warnings;

$| = 1;

my $msg ="!report";

my $rcpt = shift( @ARGV );

for ( @ARGV )
{
    $msg .= " ";
    $msg .= $_;
}

my @res;

my $host = 'localhost';
my $port = 5222;
my $tls  = 1;
my $ssl  = 0;
my $comp = '';

my $cnx = new Net::XMPP::Client();
@res = $cnx->Connect(
    hostname        => $host,
    port            => $port,
    tls             => $tls,
    ssl             => $ssl,
    connectiontype  => 'tcpip',
);

my $user        = 'zabbix';
my $password    = 'test123';
my $resource    = 'roaXMPP';

@res = $cnx->AuthSend(
    username => $user,
    password => $password,
    resource => $resource
);

my $message_type    = 'chat';
my $subject         = '';
my $body            = $msg;

$cnx->MessageSend(
    'to'        => $rcpt,
    'type'      => $message_type,
    'subject'   => $subject,
    'body'      => $body,
);

sleep( 2 );

$cnx->Disconnect();
