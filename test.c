#include <stdio.h>
#include "getword/getch.h"
#include <string.h>

#define BUFSIZE	100

char *zoomwrite(char *, char *, char *);

int main(void)
{
	char *s, *endseq, buf[BUFSIZE], *bufp, *bufend;
        s = "This is a test */";
	endseq = "*/";
	bufp = buf;
	bufend = buf + BUFSIZE; /* first address not in buffer */

	printf("\nbufp = 0x%x\n", bufp);
	bufp = zoomwrite(bufp, bufend, endseq);
	printf("bufp = 0x%x\n", bufp);
	*bufp++ = '\0';
	printf("Buffer: %s\n", buf);
	
	return 0;
}

/* zoomwrite: writes all characters directly from getch into given buffer until
 * ending sequence is reached */
char *zoomwrite(char *bufp, char *bufend, char* endseq)
{
	int endseqlen, c;
	endseqlen = strlen(endseq);
	while(bufp < bufend - 1 && (c = getch()) != EOF && 
			strcmp(endseq, bufp - endseqlen) != 0)
		*bufp++ = c;
	return bufp;
}
