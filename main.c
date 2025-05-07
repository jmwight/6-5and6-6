#include "getword/getch.h"
#include "dict.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define BUFSIZE	40960 /* arbitrarily picked */
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
		*bufp++ = c;
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
			switch(nxtc)
			{
				case '*':
					endseq = "*/";
					break;
				case '/':
					endseq = "\n";
					break;
				default:
					ungetch(nxtc);
					nxtc = 0x18; /* ASCII for cancel */
					break;
			}
			if(nxtc != 0x18)
			{
				*bufp++ = nxtc;
				bufp = zoomwrite(bufp, bufend, endseq);
			}
		}
		else
		{
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
					} while(!isspace(*nameend));
					nameendc = *nameend;

					/* get definition and definition end */
					def = startnxtword(name, bufend);
					defend = def;
					while(*++defend != '\n')
						;

					defendc = *defend;
					*defend = '\0';
					*nameend = '\0'; /* to make proper string,
							 we will change back */

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
							printf("Error: out of" 
								"memory\n");
							return NOMEMERR;
						}
					}
					else
					{
						if(strcmp(n->defn, def) != 0)
						{
							printf("Error "
								"redefining"
								"without"
								"undef\n");
							return REDEFERR;
						}
					}

					/* reset characters back from '\0' */
					*nameend = nameendc;
					*defend = defendc;
				}
				else if(strncmp(start, "undef", strlen("undef")) == 0)
				{
					char *name, *nameend, nameendc, defendc;

					/* get name and name end */
					name = startnxtword(start, bufend);

					/* find end and store it */
					nameend = name;
					do
					{
						++nameend;
					} while(!isspace(*nameend));
					nameendc = *nameend;
					*nameend = '\0'; /* to make proper string,
							 we will change back */

					if(undef(name) == NOTFOUND)
						printf("definition not found\n");
					*nameend = nameendc;
				}
			}
			/* start of identifier, turn on identifier state and
			 * record it's starting position */
			else if(state == NONE && (isalpha(c) || c == '_'))
			{
				state = IDENTIFIER;
				start = bufp - 1;
			}
			/* end of identifier, turn off identifier state and
			 * record, lookup if it's in hashtable and if it is
			 * replace with definition inside */
			else if(state == IDENTIFIER && !isalnum(c) && c != '_')
			{
				state = NONE;
				/* find end of identifier and store it */
				char *identend, identendc;
				identend = start;
				while(isalnum(*++identend))
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
					{
						*start = n->defn[i];
					}
					if(start > bufp)
						bufp = start;
					/* if replacement is shorter than
					 * definition we need to move buffer
					 * pointer back to end of replacement*/
					if(start < bufp)
						bufp = start;
					*bufp++ = c;
				}
			}
		}
	}
	*bufp = '\0';

	printf("\nResultant Code:\n%s\n", buf);
}

/* zoomwrite: writes all characters directly from getch into given buffer until
 * ending sequence is reached */
char *zoomwrite(char *bufp, char *bufend, char* endseq)
{
	int endseqlen, c;
	endseqlen = strlen(endseq);
	if(bufp < bufend - 1 && (c = getch()) != EOF)
		*bufp++ = c;
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
	} while(!isspace(*pos) && pos < strend - 1);
	do
	{
		pos++;
	} while(isspace(*pos) && pos < strend - 1);
	return pos;
}
