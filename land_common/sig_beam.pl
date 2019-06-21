#!/usr/bin/perl
# Usage:  cpp -P /path/to/cfg_sig_beam.hh | ./sig_beam.pl > mapping.hh

use warnings;

for (my $sl = 1; $sl <= 25; $sl++) {
	$type{sprintf("L_SL%02d",$sl)} = sprintf("fastbus.SL%02d",$sl);
}

$type{"L_Q46993"} = "fastbus1.Q46993";
$type{"L_Q47342"} = "fastbus1.Q47342";
$type{"L_Q11111"} = "fastbus1.Q11111";
$type{"L_Q46848"} = "fastbus1.Q46848";
$type{"L_Q47315"} = "fastbus1.Q47315";
$type{"L_Q17320"} = "fastbus1.Q17320";
$type{"L_Q15372"} = "fastbus1.Q15372";
$type{"L_Q46962"} = "fastbus1.Q46962";
$type{"L_Q15358"} = "fastbus1.Q15358";
$type{"L_Q47264"} = "fastbus1.Q47264";
$type{"L_T81855"} = "fastbus1.T81855";
$type{"L_T87044"} = "fastbus1.T87044";
$type{"L_T87024"} = "fastbus1.T87024";
$type{"L_T81800"} = "fastbus1.T81800";
$type{"L_Q75266"} = "fastbus2.Q75266";
$type{"L_T87047"} = "fastbus2.T87047";
$type{"L_T48854"} = "fastbus2.T48854";
$type{"L_T81808"} = "fastbus2.T81808";
$type{"L_T81859"} = "fastbus2.T81859";
$type{"L_T81795"} = "fastbus2.T81795";
$type{"L_T81806"} = "fastbus2.T81806";
$type{"L_T48834"} = "fastbus2.T48834";
$type{"TDC6"} = "fastbus3.TDC6";
$type{"TDC7"} = "fastbus3.TDC7";
$type{"TDC8"} = "fastbus3.TDC8";
$type{"TDC9"} = "fastbus3.TDC9";
$type{"TDC10"} = "fastbus3.TDC10";
$type{"QDC14"} = "fastbus3.QDC14";
$type{"QDC15"} = "fastbus3.QDC15";
$type{"QDC16"} = "fastbus3.QDC16";
$type{"SIA0415"} = "camac.qSIA0415";
$type{"SIA0416"} = "camac.qSIA0416";
$type{"SIA0418"} = "camac.qSIA0418";
$type{"SIA0429"} = "camac.qSIA0429";
$type{"SIA0458"} = "camac.qSIA0458";
$type{"SIA0480"} = "camac.qSIA0480";
$type{"SIA0495"} = "camac.qSIA0495";
$type{"SIA0400"} = "camac.tSIA0400";
$type{"SIA0401"} = "camac.tSIA0401";
$type{"SIA0426"} = "camac.tSIA0426";
$type{"SIA0428"} = "camac.tSIA0428";
$type{"SIA0469"} = "camac.tSIA0469";
$type{"SIA8035"} = "camac.tSIA8035";
$type{"PHI14829"} = "camac.aPHI14829";
$type{"PHI12160"} = "camac.aPHI12160";
$type{"PHI18573"} = "camac.tPHI18573";
$type{"PHI10481"} = "camac.tPHI10481";
$type{"PHI7079"} = "camac.tPHI7079";
$type{"PHI7186"} = "camac.tPHI7186";
$type{"C_FRS120"} = "camac.tC_FRS120";
$type{"CAEN0472"} = "vme.qCAEN0472";
$type{"CAEN0614"} = "vme.tCAEN0614";
$type{"CBL01"} = "vme_cbl.adc[3]";
$type{"CBL02"} = "vme_cbl.adc[2]";
$type{"CBL03"} = "vme_cbl.adc[1]";
$type{"CBL04"} = "vme_cbl.adc[0]";
$type{"CBL05"} = "vme_cbl.tdc";
$type{"MADC32PSP"} = "vme_cbl.madc0_psp";
$type{"CBR01"} = "vme_cbr.adc[3]";
$type{"CBR02"} = "vme_cbr.adc[2]";
$type{"CBR03"} = "vme_cbr.adc[1]";
$type{"CBR04"} = "vme_cbr.adc[0]";
$type{"CBR06"} = "vme_cbr.tdc";

$type{"QDC792POS_ROLU"} = "vme1.qdc792POS_ROLU";
$type{"TDC775POS_ROLU"} = "vme1.tdc775POS_ROLU";

$type{"QDC792TFW1"} = "vme2.qdc792TFW1";
$type{"QDC792TFW2"} = "vme2.qdc792TFW2";
$type{"TDC775TFW1"} = "vme2.tdc775TFW1";
$type{"TDC775TFW2"} = "vme2.tdc775TFW2";
$type{"TDC775NTF"} = "vme2.tdc775NTF";
$type{"QDC792NTF"} = "vme2.qdc792NTF";
$type{"QDC792GFI1"} = "vme2.qdc792GFI1";
$type{"QDC792GFI2"} = "vme2.qdc792GFI2";
$type{"QDC792GFI3"} = "vme2.qdc792GFI3";

#open the input file and copy the informations into an aray
@SigBeam = <STDIN>;

line:
for($i=0;$i<scalar(@SigBeam);$i++) 
{
    if (!($SigBeam[$i] =~ /SIG_(BEAM|LAND|TDET)\s*\((.*)\)/))
    {
	if (!($SigBeam[$i] =~ /\s*/))
	{
	    print STDERR "Unknown line: $SigBeam[$i]\n";
	}
	next line;
    }
    $content = $2;
    $content =~ s/ //g;
    
    @line = split(' *, *',$content);
    
    $name = $line[0];

    if ($name =~ /PDC0|TWA/) 
    {
	next line;
    }
    
    if ($line[7]ne'NONE') 
    {
	$ch_E = $line[8];
	$module = $line[7];
	$type = $type{$module};
	if (!$type) { $type = "/*$module*/UNKNOWN"; }
	print "SIGNAL($name";
	print "_E,$type.data[$ch_E],DATA12);\n";
    }
    
    if ($line[9]ne'NONE') 
    {
	$ch_T = $line[10];
	$module = $line[9];
	$type = $type{$module};
	if (!$type) { $type = "/*$module*/UNKNOWN"; }
	print "SIGNAL($name";
	print "_T,$type.data[$ch_T],DATA12);\n";
    }
}
