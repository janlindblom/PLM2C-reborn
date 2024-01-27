#include "defs.h"
#include "cvt.h"
#include "struct.h"

/*
 *	PL/M function equivalents
 */
CVT_ID cvt_functions[] = {
	"size",			"sizeof",
	"abs",			"fabs",
	"iabs",			"abs",
	"input",		"inportb",
	"inword",		"inport",
	"setinterrupt",		"signal",
	"initrealmathunit",	"_fpreset",
	"",		""
};

/*
 *	PL/M identifier equivalents
 */
CVT_ID cvt_identifiers[] = {
	"getrealerror",		"_status87()",
	"",			""
};

