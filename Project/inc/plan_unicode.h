#ifndef PLAN_UNICODE_H
#define PLAN_UNICODE_H

#include "plan_include.h"


#define MERGE2(a, b) a ## b
#define CVTBL(tbl, cp) MERGE2(tbl, cp)
#define FF_CODE_PAGE 437



WCHAR uni2oem (	/* Returns OEM code character, zero on error */
	WCHAR	uni,	/* Unicode character to be converted */
	WORD	cp		/* Code page for the conversion */
);

WCHAR oem2uni (	/* Returns Unicode character, zero on error */
	WCHAR	oem,	/* OEM code to be converted */
	WORD	cp		/* Code page for the conversion */
);



#endif //PLAN_UNICODE_H