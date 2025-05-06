#include "getword/getch.c"
#include <ctype.h>
#include <string.h>

#define BUFSIZE	40960 /* arbitrarily picked */
#define WSIZE	30 /* 99.999% for length */

char *zoomwrite(char *bufp, char *bufend, char* endseq);

int main(void)
{
	char buf[BUFSIZE], *bufp, *bufend, w[WSIZE], *wp, *preprocdirp;
	bufp = buf;
	int wstart;
	bufend = buf + BUFSIZE; /* first pointer not included in buf */
	wp = w;
	int c, preprocdir;
	preprocdir = multilncomm = onelncomm = str = def = 0;
	/* < bufend - 1 to leave an extra space to append '\0' on */
	while((c = getch()) != EOF && bufp < bufend - 1)
	{
		if(c == '"')
		{
			/* need to leave room for '\0' possibly hence -1 */
			bufp = zoomwrite(bufp, bufend, "\"");
		}
		else if(c == '/')
		{
			int nxtc;
		        char *endseq;
			nxtc = getch();
			if(nxtc == '*')
				endseq = "*/";
			else if(nxtc == '/')
				endseq = "\n";
			bufp = zoomwrite(bufp, bufend, endseq);
		}
		else if(isspace(c))
			*bufp++ = c;
		else if(preprocdir)
		{
			/* TODO: do some preprocessor definition undef stuff */
		}
		else if(c == '#')
		{
			*bufp++ = c;
			preprocdirp = bufp;
			preprocdir = 1;
		}
	}
}

/* getword: gets word from input, returns length or if overflow, -1 or EOF */
int getword(char *w, int mxlen)
{
	int c, i;
	i = 0;
	while(!isspace(c = getch()) && c != EOF && i < maxlen - 1)
		w[i++] = c;
	w[i] = '\0';
	ungetch(c);
	if(c == EOF)
		return EOF;
	else if(!isspace(c))
		return -1;
	else
		return i;
}

/* addword: adds word to buffer (if possible) */
int addword(){;}

/* addspace: adds all space to buffer */
int addspace(){;}

/* addwo */

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
