#!/usr/bin/perl
#
# Call this script via:    nohup ./DST_massive_submitter.pl process.sh &
# 
# This script checks the number of reprocess jobs pending/running;
# If the number of jobs  under a certain threshold, a new process is submitted.
#
# Based on previous script of SD.
# Revised: Alessandra Re (August 2013)
# 

use warnings;
use strict;

my $process = shift;
die "\n\t Usage: ./DST_massive_submitter.pl input_file.sh\n\n" if(!$process);

open INPUT, $process or die $!;
my @commands = <INPUT>;
close INPUT;

my $sleep_time = 3;
my $submitted = 0;
my $cpuload_limit = 600;
my $total_time = 0;
my $current_time = int(`date +%s`);
my $current_time2 = int(`date +%s`);

while ($#commands >= 0){
#       my $running = int(`bjobs | grep -c RUN`);
#       my $pending = int(`bjobs | grep -c PEND`);
        my $cpuload = int(`ps -A -o pcpu | tail -n+2 | paste -sd+ | bc`);
        open(my $limitfile, '<:encoding(UTF-8)',"wunderwaffe.cfg") or die "Could not open file 'wunderwaffe.cfg' $!";
        while (my $row = <$limitfile>) {
          chomp $row;
          $cpuload_limit = $row;
        }
        close($limitfile);
        $current_time2 = int(`date +%s`);
        $total_time = $total_time - $current_time + $current_time2;
        $current_time = int(`date +%s`);

        if(($cpuload < $cpuload_limit)) {
                my $new_process = shift(@commands);
                print "\t Submitting $new_process";
                system("$new_process");
                sleep 2;
        } else{
                my $still = $#commands+1;
                print "\t LOAD: $cpuload(limit: $cpuload_limit), submitted: $submitted; total time: $total_time s, still $still process to submit.\n";
                sleep $sleep_time;
        }
}


