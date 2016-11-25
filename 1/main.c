#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <strings.h>
#include <stdint.h>

#define cast(t, v)	((t)(v))
#define hash(t)	(t->vec)
#define key(n)	(n->key)
#define cdata(n)	(n->data)
#define len(t)	(t->len)
#define	getPos(k, t)	(key(k) % len(t))

typedef	uint32_t Natural;

typedef struct Node{
	Natural key;
	void	*data;
	SLIST_ENTRY(Node)	sibl;
}		Node;

typedef struct Table{
	SLIST_HEAD(coalescedEntries, Node)	*vec;
	size_t	len;
	size_t	cnt;
}	Table;

Table *newTable(size_t len)
{
	Table *t;

	t = malloc(sizeof(Table));
	bzero(t, sizeof(Table));
	t->len = len;
	t->cnt = 0;
	hash(t) = malloc(len * sizeof(*hash(t)));
	bzero(hash(t), len(t) * sizeof(*hash(t)));
	return (t);
}

Natural Hash(char *str, size_t len, Natural seed)
{
	Natural hash = seed;

	for (int i = 0; i < len; i++)
		hash ^= str[i] + (hash << 5 ^ hash >> 2);
	return (hash);
}

Natural	insertIntoTable(Table *t, Node *kv)
{
	Natural	h;

	h = getPos(kv, t);
	SLIST_INSERT_HEAD(hash(t) + h, kv, sibl);
	return (h);
}

Node *lookupIntoTable(Table *t, Node *kv)
{
	Node *k;
	Natural	h;

	h = getPos(kv, t);
	SLIST_FOREACH(k, hash(t) + h, sibl)
		if (strcmp(cdata(kv), cdata(k)) == 0)
			return (k);
	return (NULL);		
}

Node *deleteFromTable(Table *t, Node *kv)
{
	Node *k;
	Natural	h;

	k = lookupIntoTable(t, kv);
	h = getPos(kv, t);
	if (k)
	{
		SLIST_REMOVE(hash(t) + h, k, Node, sibl);
		return (k);
	}
	return (NULL);
}


void printNode(Node *n)
{
	printf("\t[%x][%s]\n", key(n), cdata(n));
}

void printTable(Table *t)
{
	Node *n;
	
	for (int i = 0; i < len(t); i++)
	{
		printf("[%d]:[%p]\n", i, hash(t) + i);
		SLIST_FOREACH(n, hash(t) + i, sibl)
			printNode(n);
	}
}

Node *newNode(unsigned int key, void *data)
{
	Node *n;

	n = malloc(sizeof(Node));
	n->key = key;
	n->data = data;
	return (n);
}

void delNode(Node *n)
{
	free(n->data);
	free(n);
}

//void delTable(Table *t)
//{
//	for (int i = 0; i < len(t); i++)
//		delNode(hash_(t)[i]);
//	free(t);
//}

void usage()
{
	printf("\tUsage:\n\n"
		"\t\t[i]-[keys] : Insert\n"
		"\t\t[l]-[keys] : Lookup\n"
		"\t\t[d]-[keys] : Delete\n"
		"\t\t[u] : usage\n"
		"\t\t[p] : print table\n"
		"\t\t[x] : exit\n");
}

int main(int argc, char **argv, char **envp)
{
	Table *t;
	char c=0;
	char str[256];
	int len;

	printf("Choose table size:\n");
	scanf("%d",&len);
	t = newTable(len);
	while (1)
	{
		usage();
		scanf("\n%c-%s",&c, str);
		printf("c:<%c>\n", c);
		switch (c)
		{
		case 'i':
		{
			Node *n = newNode(Hash(str, strlen(str), 0), strdup(str));
			printNode(n);
			if (insertIntoTable(t, n) == -1)
			{
				printf("Table is full ?\n");
				exit(EXIT_FAILURE);
			}
			printTable(t);
			break ;
		}
		case 'd':
		{
			Node *n = newNode(Hash(str, strlen(str), 0), strdup(str));
			printNode(n);
			if (deleteFromTable(t, n) == NULL)
				printf("Not in table, is it OK ?\n");
			printTable(t);
			break ;
		}
		case 'l':
		{
			Node *n, *m;
			n = newNode(Hash(str, strlen(str), 0), strdup(str));
			printNode(n);
			m = lookupIntoTable(t, n);
			if (m == NULL)
				printf("Not in table, is it OK ?\n");
			else
				printNode(m);
			break ;
		}
		case 'u':
		{
			usage();
			break ;
		}
		case 'p':
		{
			printTable(t);
			break;
		}
		case 'x': exit(0);
		default: {usage();exit(0);}
		}
	}
	return (0);
}

