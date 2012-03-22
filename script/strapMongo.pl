#!/usr/bin/perl

use strict;
use warnings;
use MongoDB;
use Data::Dumper;

my $conn = MongoDB::Connection->new(host => 'localhost', port => 27017);
my $db = $conn->zabbix;
my $coll = $db->admins;

my $id = $coll->insert({ name => 'foo'});
