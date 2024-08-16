#!/usr/bin/perl
print "<html><body><center><h1>Hello from Perl!</h1></center></body></html>";

use strict;
use warnings;

print "\n----- THE SCRIPT COMMAND -----\n\n";
# Call 'ls -la' command
my $output = `ls -la`;

# Print the output
print $output;
print "\n----- THE ENV VARIABLES -----\n";

print "\ncontent-type: $ENV{'SERVER_PROTOCOL'}\n";
print "host-name: $ENV{'HOST_NAME'}\n";
print "path-info: $ENV{'PATH_INFO'}\n";