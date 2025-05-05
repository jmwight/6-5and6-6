#include "getword/getch.c"
#include <ctype.h>
#include <string.h>

#define BUFSIZE	40960 /* arbitrarily picked */
#define WSIZE	30 /* 99.999% for length */

int main(void)
{
	char buf[BUFSIZE], *bufp, w[WSIZE], *wp;
	bufp = buf;
	wp = w;
	int c, multilncomm, onelncomm, str, def, preprocdir;
	preprocdir = multilncomm = onelncomm = str = def = 0;
	while((c = getch()) != EOF)
	{
		/* need to detect comments, strings, and leadning '#' */
		/* turn on or off string state */
		if(c == '"' && comm == 0)
			str = 1;
		else if(c == '"' && comm = 1)
			str = 0;
		/* turn off comment state */
		else if(c == '/')
		{
			int nxtc = getch();
			if(nxtc == EOF)
			{
				ungetch(nxtc);
				break;
			}
			else if(nxtc == '*')
			       multilncomm = 1;
			else if(nxtc == '/')
				onelncomm = 1;
			ungetch(nxtc);
		}
		/* turn on preprocessor directive state if not inside string or comment */
		else if(c == '#' && !onelncomm && !multilncomm && !str)
			preprocdir = 1;
		/* turn off preprocessor directive or one line comment */
		else if(c == '\n' && (preprocdir || onelncomm))
			preprocdir = onelncomm = 0;
		
		/* check to make sure this is safe in buffer overflow condition */
		*bufp = c;

		/* read preprocessor directive if we are on one and do what's necessary */
		if(preprocdir)
		{
			/* get directive first */
			while(!isspace(c = getch) && c != EOF)
			{
				*w++ = c;
				*bufp++ = c;
			}
			if(strcmp(w, "define"))
				; /* TODO: finish this */
			else if(strcmp(w, "undef"))
				;  /* TODO: finish this */

		}
	}
}
/* zoomwrite: writes all characters directly from getch into given buffer until
 * ending sequence is reached */
char *zoomwrite(char *bufp, char* endseq)
{
	int endseqlen, c;
	endseqlen = strlen(endseq);
	while((c = getch()) != EOF && strcmp(endseq, bufp[endseqlen * -1]) != 0)
		*bufp++ = c;
	return bufp;
}
