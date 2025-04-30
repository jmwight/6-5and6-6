

#define HASHSIZE	101

/* table entry */
struct nlist
{
	struct nlist *next; /* next block in linked list */
	char *name; /* defined name */
	char *defn; /* replacement text */
}

static struct nlist *hashtab[HASHSIZE]; /* pointer table */

/* hash: form hash value for string s */
unsigned int hash(char *s)
{
	unsigned int hash;

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
struct nlist *install(char *name; char *defn)
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


