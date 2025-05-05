/* table entry */
struct nlist
{
	struct nlist *next; /* next block in linked list */
	char *name; /* defined name */
	char *defn; /* replacement text */
}

unsigned int hash(char *s);
struct nlist *lookup(char *s);
struct nlist *install(char *name; char *defn);
int undef(char *name);
