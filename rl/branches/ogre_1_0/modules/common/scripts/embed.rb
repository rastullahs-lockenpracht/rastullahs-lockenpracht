if $0 != "Rastullah" && $0 != "RlCore" && $0 != "libRlCore"
	print <<MSG
Warnung: 
Dies ist eine Skriptdatei des eingebetteten Rastullah-Ruby-Interpreters
Dieses darf nur aus der Rastullah-Applikation ausgefuehrt werden.
MSG
	abort
end
