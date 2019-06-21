#!/usr/bin/perl
# Usage:  cpp -P /path/to/cfg_elecloc.hh | ./elecloc.pl
# generates files fastbus_c{1,2}.hh and camac_c{2,3}.hh

use warnings;

open(INPUT,"cfg_elecloc.hh");
@Elecloc = <STDIN>;
close(INPUT);

for($i=0;$i<scalar(@Elecloc);$i++) {chomp($Elecloc[$i]);}

open(OUTPUT1,">fastbus_c1.hh");
open(OUTPUT2,">fastbus_c2.hh");
open(OUTPUT3,">camac_c3.hh");
open(OUTPUT4,">camac_c2.hh");

my %XDC;
$XDC{"QDC_F"} = "FASTBUS_LECROY_1885" ;
$XDC{"TDC_F"} = "FASTBUS_LECROY_1875" ;
$XDC{"QDC4418Q"} = "CAMAC_SILENA_4418Q";
$XDC{"QDC4418Q_4CH"} = "CAMAC_SILENA_4418Q";
$XDC{"QDC4418Q_6CH"} = "CAMAC_SILENA_4418Q";
$XDC{"TDCS4418"} = "CAMAC_SILENA_4418T";
$XDC{"TDCS4418_2CH"} = "CAMAC_SILENA_4418T";
$XDC{"TDCS4418_4CH"} = "CAMAC_SILENA_4418T";
$XDC{"TDCPHI16"} = "CAMAC_PHILLIPS_7186H";
$XDC{"TDCPHI16_8CH"} = "CAMAC_PHILLIPS_7186H";
$XDC{"TDC_8"} = "CAMAC_SILENA_4418T";
$XDC{"TDC_8_6CH"} = "CAMAC_SILENA_4418T";
$XDC{"ADCPHI16"} = "CAMAC_PHILLIPS_7164H";
$XDC{"ADCPHI14"} = "CAMAC_PHILLIPS_7164H";
my %channels;
$channels{"QDC_F"} = 96 ;
$channels{"TDC_F"} = 64 ;
$channels{"QDC4418Q"} = 8 ;
$channels{"QDC4418Q_4CH"} = 4;
$channels{"QDC4418Q_6CH"} = 6;
$channels{"TDCS4418"} = 8 ;
$channels{"TDCS4418_2CH"} = 2 ;
$channels{"TDCS4418_4CH"} = 4 ;
$channels{"TDCPHI16"} = 16 ;
$channels{"TDCPHI16_8CH"} = 8 ;
$channels{"TDC_8"} = 8 ;
$channels{"TDC_8_6CH"} = 6 ;
$channels{"ADCPHI16"} = 16;
$channels{"ADCPHI14"} = 14;
my %mark_channels;
$mark_channels{"ADCPHI16"} = 1;
$mark_channels{"ADCPHI14"} = 1;
$mark_channels{"TDCPHI16"} = 1;
$mark_channels{"TDCPHI16_8CH"} = 1;
$mark_channels{"QDC4418Q"} = 0;
$mark_channels{"QDC4418Q_4CH"} = 0;
$mark_channels{"QDC4418Q_6CH"} = 0;
$mark_channels{"TDCS4418"} = 0;
$mark_channels{"TDCS4418_2CH"} = 0;
$mark_channels{"TDCS4418_4CH"} = 0;
$mark_channels{"TDC_8"} = 0;
$mark_channels{"TDC_8_6CH"} = 0;

$dim=13;
$j2=0;
$j3=0;

for($i=0;$i<scalar(@Elecloc);$i++) 
  {
    @line = split(' ',$Elecloc[$i]);
    $len = scalar(@line);
    #    print "\n\n\n\n";
    $type = $line[11];

    #print "$len\n";

    if(($len==$dim) && ($line[0]eq'ELECLOC(') && ($type ne 'OFF'))
      {
	$type     = $line[11];       #fastbus or camac
	$model    = $XDC{$line[3]};  #type of the module
	$name     = $line[1];         #different for each module
	$crate    = $line[5];         #on which crate the module is located
	$slot     = $line[7];         #on which slot the module is plugged
	$channels = $channels{$line[3]};

	if($type eq 'FASTBUS')
	  {
	    #crate 1 = FASTBUS
	    if ($crate==1)
	      {
		if($name =~ /(.+)_(.+)/) {$name=$2;}
		if($line[3] eq QDC_F) {print OUTPUT1 "$name = $model(geom=$slot,channels=$channels);\n";}
		if($line[3] eq TDC_F) {print OUTPUT1 "$name = $model(geom=$slot,channels=$channels);\n";}
		if(($line[3] ne TDC_F) && ($line[3] ne QDC_F)) {print "FASTBUS - CRATE 1 - no QDC_F neither TDC_F but $line[3]\n";}
	      }
	    
	    #crate 2 = FASTBUS
	    if ($crate==2)
	      {
		if($name =~ /(.+)_(.+)/) {$name=$2;}
		if($line[3] eq QDC_F) {print OUTPUT2 "$name = $model(geom=$slot,channels=$channels);\n";}
		if($line[3] eq TDC_F) {print OUTPUT2 "$name = $model(geom=$slot,channels=$channels);\n";}
		if(($line[3] ne TDC_F) && ($line[3] ne QDC_F)) {print "FASTBUS - CRATE 2 - no QDC_F neither TDC_F but $line[3]\n";}
	      }
	  }

	if($type eq 'CAMAC')
	  {
	    #crate 2 = CAMAC
	    if ($crate==2)
	      {
		$mark_channel_no = $mark_channels{$line[3]};
		$geom1[$j2]=$slot;
		if($line[3] =~ /QDC(.+)/) 
		  {
		    $sort1[$j2]= "q$name = $model(channels=$channels,mark_channel_no=$mark_channel_no);";
		  }
		if($line[3] =~ /ADC(.+)/) 
		  { 
		    $sort1[$j2]= "a$name = $model(channels=$channels,mark_channel_no=$mark_channel_no);";
		  }
		if($line[3] =~ /TDC(.+)/) 
		  { 
		    $sort1[$j2]= "t$name = $model(channels=$channels,mark_channel_no=$mark_channel_no);";
		  }
		$j2++;
	      }

	    #crate 3 = CAMAC
	    if ($crate==3)
	      {
		$mark_channel_no = $mark_channels{$line[3]};
		$geom2[$j3]=$slot;
		if($line[3] =~ /QDC(.+)/) 
		  {
		    $sort2[$j3]= "q$name = $model(channels=$channels,mark_channel_no=$mark_channel_no);";
		  }
		if($line[3] =~ /ADC(.+)/) 
		  {
		    $sort2[$j3]= "a$name = $model(channels=$channels,mark_channel_no=$mark_channel_no);";
		  }
		if($line[3] =~ /TDC(.+)/) 
		  { 
		    $sort2[$j3]= "t$name = $model(channels=$channels,mark_channel_no=$mark_channel_no);";
		  }
		$j3++;
	      }
	  }
	#	for($j=0;$j<$len;$j++)
	#	  {
	#	    print "line $i: part $j = $line[$j]\n";
	#	  }
      }
  }

for($j=0;$j<scalar(@geom1);$j++)
  {
    $min = $geom1[$j];
    $dum = $sort1[$j];
    $l = $j;
    for($k=$j;$k<scalar(@geom1);$k++)
      {
	if($geom1[$k]<$min)
	  {
	    $min = $geom1[$k];
	    $dum = $sort1[$k];
	    $l=$k;
	  }
      }
    $geom1[$l] = $geom1[$j];
    $sort1[$l] = $sort1[$j];
    $geom1[$j] = $min ;
    $sort1[$j] = $dum ;
    print OUTPUT4 "$sort1[$j]\n";
  }

for($j=0;$j<scalar(@geom2);$j++)
  {
    $min = $geom2[$j];
    $dum = $sort2[$j];
    $l = $j;
    for($k=$j;$k<scalar(@geom2);$k++)
      {
	if($geom2[$k]<$min)
	  {
	    $min = $geom2[$k];
	    $dum = $sort2[$k];
	    $l=$k;
	  }
      }
    $geom2[$l] = $geom2[$j];
    $sort2[$l] = $sort2[$j];
    $geom2[$j] = $min ;
    $sort2[$j] = $dum ;
    print OUTPUT3 "$sort2[$j]\n";
  }


close(OUTPUT1);
close(OUTPUT2);
close(OUTPUT3);
close(OUTPUT4);
