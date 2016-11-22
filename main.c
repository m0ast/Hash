#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


#define cast(t, i)	((t)(i))
#include <strings.h>
#include <limits.h>
#define hash_(t) (t->_)
#define len(t) (t->len)
#define	key(k) (k->key)
#define cdata(k) (cast(char *, k->data))
#define R(key, n) (cast(unsigned int, (key % n)))
#define Q(key, bis, n) (cast(unsigned int, (((2 * (unsigned int)(key / n)) + 1) * bis) + (4 * (unsigned int)(key / n) * bis)))
#define copyNode(dst, src) (memcpy(dst, src, sizeof(Node)))

typedef int64_t		integer;

typedef struct	header
{
	bool t;
}				header_t;

typedef union	object
{
	integer		i;
	bool		b;
	void		*p;
}				ot;

typedef struct string
{
	char	*data;
	size_t	size;
}				string_t;

unsigned int	hash(char *data, size_t len, unsigned int seed)
{
	unsigned int	hash;
	unsigned int	i;

	hash = seed ^ cast(unsigned int, len);
	i = 0;
	while (i < len)
	{
		hash ^= (data[i] + (hash >> 2) + (hash << 5));
		++i;
	}
//	hash = hash >> 1;
	return (hash);
}

/*
Hash *newHash()
{

}*/

typedef struct
{
	void *data;
	unsigned int key;
}		Node;

typedef struct
{
	size_t len;
	Node	*_;
}		Table;

Node dummy_node = {
	.data = NULL;
	.key = 0;
};

Table	*newTable(size_t len)
{
	Table	*ptr;

	ptr = malloc(sizeof(Table));
	bzero(ptr, sizeof(Table));
	if (ptr == NULL)
		exit(EXIT_FAILURE);
	ptr->_ = malloc(len * sizeof(Node));
	bzero(ptr, sizeof(len * sizeof(Node));
	ptr->len = len;
	return (ptr);
}


int		getPos(Table *t, Node *kv)
{
	int hs;
	
	for (int s = 0;
		 !(hs = (R(key(kv), len(t)) + Q(key(kv), s, len(t))) % len(t), hs == R(key(kv), len(t))) != !s;
		 s++)
	{
		printf("hs:s [%d][%d]\n", hs, s);
		if (hash_(t)[hs] != NULL && strcmp(cdata(hash_(t)[hs]), cdata(kv)) == 0)
			return (hs);
	}
	return (-1);
}

Node	*deleteFromTable(Table *t, Node *kv)
{
	int pos;
	
	if ((pos = getPos(t, kv)) != -1)
	{
		kv = hash_(t)[pos];
		hash_(t)[pos] = NULL;
		return (kv);
	}
	return (NULL);
}

Node	*findIntoTable(Table *t, Node *kv)
{
	int pos;
	
	if ((pos = getPos(t, kv)) != -1)
		return (hash_(t)[pos]);
	return (NULL);
}

unsigned int insertIntoTable(Table *t, Node *kv)
{
	unsigned int s;
	unsigned int hs;

	for (s = 0;
		 !(hs = (R(key(kv), len(t)) + Q(key(kv), s, len(t))) % len(t), hs == R(key(kv), len(t))) != !s;
		 s++)
	{
		printf("hs:s [%d][%d]\n", hs, s);
		if (hash_(t)[hs] == NULL)
			break;
	}
//	printf("ok %d and %d and <%d>and <%d>\n", hs,  R(key(kv), len(t)), Q(key(kv), s, len(t)), s) ;
	if (hash_(t)[hs] != NULL)
		return (-1);
	for (int i = 0; i < ((int)s - 2) ;i++)
	{
		unsigned int hi = (R(key(kv), len(t)) + Q(key(kv), i,len(t))) % len(t);
//		printf("okb <%d>\n", i);
		for (int j = 1 ; i + j < s; j++)
		{
			unsigned int hij = (hi + Q(key(hash_(t)[hi]), j,len(t))) % len(t);
//			printf("okc %d:%d\n", i, j);
//			printf("%d f %d\n", hi, hij);
			if (hash_(t)[hij] == NULL)
			{
				hash_(t)[hij] = hash_(t)[hi];
				hash_(t)[hi] = kv;
				//		printf("%d o %d\n", hi, hij);
				return (hi);
			}
		}
	}
	hash_(t)[hs] = kv;
//	printf("ok\n");
	return (hs);
}

void printTable(Table *t)
{
	for (int i = 0; i < len(t); i++)
	{
		printf("[%d]:[%x][%s]\n", i, (hash_(t)[i]) ? key(hash_(t)[i]) : 0, (hash_(t)[i]) ? cdata(hash_(t)[i]) : "NULL");
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

void delTable(Table *t)
{
	for (int i = 0; i < len(t); i++)
		delNode(hash_(t)[i]);
	free(t);
}

int isFull(Table *t)
{
	for (int i = 0; i < len(t); i++)
		if(hash_(t)[i] == NULL)
			return (1);
	return (0);
}

#include <time.h>

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

void printNode(Node *n)
{
	printf("[%x][%s]\n", key(n), cdata(n));
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
			Node *n = newNode(hash(str, strlen(str), 0), strdup(str));
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
			Node *n = newNode(hash(str, strlen(str), 0), strdup(str));
			printNode(n);
			if (deleteFromTable(t, n) == NULL)
				printf("Not in table, is it OK ?\n");
			printTable(t);
			break ;
		}
		case 'l':
		{
			Node *n, *m;
			n = newNode(hash(str, strlen(str), 0), strdup(str));
			printNode(n);
			m = findIntoTable(t, n);
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
