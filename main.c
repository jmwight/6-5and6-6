#include "getword/getch.c"
#include <ctype.h>
#include <string.h>

#define BUFSIZE	40960 /* arbitrarily picked */
#define WSIZE	30 /* 99.999% for length */
#define NONE		0
#define	PREPROCDIR	1
#define IDENTIFIER	2
#define NOMEMERR	12
#define REDEFERR	20

char *zoomwrite(char *bufp, char *bufend, char* endseq);
char *startnxtword(char *pos, char *strend);

int main(void)
{
	char buf[BUFSIZE], *bufp, *bufend, *start; /* *start is pointer to to
						      beginning of preprocessor
						      directive or identifier*/
	bufp = buf;
	bufend = buf + BUFSIZE; /* first pointer not included in buf */
	int c, state;
	/* states: 0 = none, 1 = preprocessor directive, 2 = identifier */
	state = NONE;
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
		else
		{
			*bufp++ = c;
			/* start of preprocessor directive, turn on state and 
			 * record it's starting position */
			if(c == '#')
			{
				if(isalpha(c = getch()))
				{
					start = bufp;
					state = PREPROCDIR;
				}
				ungetch(c);
			}
			/* end of preprocessor directive, do whatever it is */
			if(state == PREPROCDIR && c == '\n')
			{
				state = NONE;
				/* define statement */
				if(strncmp(start, "define", strlen("define")) == 0)
				{
					char *name, *nameend, *def, *defend;
					char nameendc, defendc;

					/* get name and name end */
					name = startnxtword(start, bufend);
					/* find end and store it */
					nameend = name;
					do
					{
						++nameend;
					} while(!isspace(nameend));
					nameeendc = *nameend;
					*nameend = '\0'; /* to make proper string,
							 we will change back */

					/* get definition and definition end */
					def = startnxtword(name, bufend);
					defend = def;
					while(*++defend != '\n')
						;
					defendc = *defend;
					*defend = '\0';

					/* if it is already defined and the define
					 * defines it differently without an undef
					 * throw an error (ANSI C standards from C
					 * Programming book appendix) */
					
					struct nlist *n = lookup(start);
					//*nameend = endc; /* revert back 


					/* get the definition part and make sure
					 * it's the same as n hashtab if it's 
					 * defined. If it isn't add it. */
					if(n == NULL)
					{
						if(install(name, def) == NULL)
						{
							printf("Error: out of 
								memory\n");
							return NOMEMERR;
						}
					}
					else
					{
						if(strcmp(n->defn, def) != 0)
						{
							printf("Error 
								redefining 
								without 
								undef \n");
							return REDEFERR;
						}
					}

					/* reset characters back from '\0' */
					*nameend = nameendc;
					*defend = defendc;
				}

			}
			/* start of identifier, turn on identifier state and
			 * record it's starting position */
			else if(state == NONE && (isalpha(c) || c == '_'))
			{
				state = IDENTIFIER;
				begin = bufp - 1;
			}
			/* end of identifier, turn off identifier state and
			 * record, lookup if it's in hashtable and if it is
			 * replace with definition inside */
			else if(state == IDENTIFIER && isspace(c))
			{
				state = NONE;
				/* find end of identifier and store it */
				char *identend, identendc;
				while(!isspace(*++identend))
					;
				/* store one past end and replace with '\0' to
				 * make a string we can input into lookup */
				identendc = *identend;
				*identend = '\0';
				struct nlist *n;
				n = lookup(start);
				*identend = identendc;

				if(n != NULL)
				{
					/* if we found a matching definition,
					 * we replace string in buffer */
					int i;
					for(i = 0; n->defn[i] != '\0'; i++,
							start++)
						*start = defn[i];
					/* if replacement is shorter than
					 * definition we need to move buffer
					 * pointer back to end of replacement*/
					if(start < bufp)
						bufp = start;
				}
			}
		}
	}
	*bufp = '\0';
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

/* startnxtword: get position of next word in given string from current spot. 
 * strend is the pointer to one past the end of the array */
char *startnxtword(char *pos, char *strend)
{
	do
	{
		pos++;
	} while(!isspace(name) && name < strend - 1);
	do
	{
		pos++;
	} while(isspace(name) && name < strend - 1);
	return pos;
}
