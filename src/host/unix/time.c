# include "dgd.h"
# include <time.h>

/*
 * NAME:	P->time()
 * DESCRIPTION:	return the current time
 */
Uint P_time()
{
    return (Uint) time((time_t *) NULL);
}

/*
 * NAME:	P->ctime()
 * DESCRIPTION:	convert the given time to a string
 */
char *P_ctime(t)
Uint t;
{
    char *buf;
    register int offset;

    offset = 0;
    for (offset = 0; t >= 2147397248L; t -= 883612800L, offset += 28) ;
    buf = ctime((time_t *) &t);
    if (offset != 0) {
	long year;

	year = strtol(buf + 20, (char **) NULL, 10) + offset;
	if (year > 2100 ||
	    (year == 2100 && (buf[4] != 'J' || buf[5] != 'a') &&
	     (buf[4] != 'F' || (buf[8] == '2' && buf[9] == '9')))) {
	    /* 2100 is not a leap year */
	    t -= 378604800L;
	    offset += 12;
	    buf = ctime((time_t *) &t);
	    year = strtol(buf + 20, (char **) NULL, 10) + offset;
	}
	sprintf(buf + 20, "%ld\012", year);
    }
    return buf;
}
