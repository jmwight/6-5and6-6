#include "dict.h"
#include <stdlib.h>
#include <string.h>

#define HASHSIZE	101

static struct nlist *hashtab[HASHSIZE]; /* pointer table */

/* hash: form hash value for string s */
unsigned int hash(char *s)
{
	unsigned int hashval;

	for(hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % HASHSIZE;
}

/* lookup: look for s in hashtab */
struct nlist *lookup(char *s)
{
	struct nlist *np;

	/* standard idiom for walking along a linked list */
	for(np = hashtab[hash(s)]; np != NULL; np = np->next)
		if(strcmp(s, np->name) == 0)
			return np; /* found */
	return NULL; /* not found */
}

/* install: put (name, defn) in hashtab */
struct nlist *install(char *name, char *defn)
{
	struct nlist *np;
	unsigned int hashval;

	/* not found */
	if((np = lookup(name)) == NULL)
	{
		/* allocate size of *np and cast to correct type of pointer */
		np = (struct nlist *) malloc(sizeof(*np)); 
		if(np == NULL || (np->name = strdup(name)) == NULL)
			return NULL;
		hashval = hash(name);
		/* put newly created block at the beginning of the linked list */
		/* make new block next pointer point to current first block in
		 * linked list */
		np->next = hashtab[hashval]; 
		/* make hashtable point to newly created block */
		hashtab[hashval] = np;
	}
	/* already there */
	else
		free((void *) np->defn);
	if((np->defn = strdup(defn)) == NULL)
		return NULL;
	return np;
}

/* undef: undefines name from hashtab returns 1 if one undefined and 0 if
 * nothing found */
int undef(char *name)
{
	struct nlist *npcur, *npprev;
	unsigned int hashval;
	hashval = hash(name);
	npcur = hashtab[hashval];

	/* check if hashtab is null or if first one matches first */
	if(npcur == NULL)
		return 0; /* not found */
	/* we found match in first one we can just update hashtab[hashval] */
	else if(strcmp(name, npcur->name) == 0)
	{
		hashtab[hashval] = npcur->next;
		free((void *) npcur);
		return 1; /* found */
	}
	/* we need to look through several, lets setup to do so */
	else
	{
		npprev = npcur;
		npcur = npcur->next;
	}

	/* walking along a linked list comparing with previous */
	for( ; npcur != NULL; npcur = npcur->next)
	{
		if(strcmp(name, npcur->name) == 0)
		{
			/* make previous block point to block of after the
			 * current one */
			npprev->next = npcur->next;
			free((void *) npcur);
			return 1; /* found */
		}
		npprev = npcur;
	}
	return 0; /* not found */
}
