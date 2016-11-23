#!/usr/bin/perl

$OrigPath = @ARGV[0];
$NewPath = "";
#print "$OrigPath" . "\n";

while(true)
{
	$PositionToBeStripped = index($OrigPath, "/../", 0);
	if(0 > $PositionToBeStripped)
	{
		$NewPath = $OrigPath;
		last;
	}
	#print "$PositionToBeStripped" . "\n";
	$NewPath = substr($OrigPath, 0, $PositionToBeStripped);
	#print "$NewPath" . "\n";
	$SnashPos = rindex($NewPath, "/");
	$NewPath = substr($NewPath, 0, $SnashPos);
	#print "$NewPath" . "\n";
	$RemainingPath = substr($OrigPath, $PositionToBeStripped+4);
	#print "$RemainingPath" . "\n";
	if(0 < length($RemainingPath))
	{
		$NewPath = $NewPath . "/" . $RemainingPath;
	}
	last;
}

print "$NewPath" . "\n";
