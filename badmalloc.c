/* A horrible dummy malloc */

#include <stdio.h> 
#include <unistd.h>

#define align4(x) ((((x) - 1) >> 2) << 2) + 4)
#define BLOCK_SIZE sizeof(struct s_block)


void *base = NULL;

typedef struct s_block *t_block;

struct s_block {
	size_t		size;
	t_block		next;
	int			free;
	char		data[1];
};



void *malloc(size_t size)
{
	void *p;
	p = sbrk(0); //p points to the current break point in the heap
	if (sbrk(size) == (void*)-1)
	{
		return NULL;
	}
	return p;
}


t_block find_block(t_block *last, size_t size) {
	t_block b = base;
	while (b && !(b -> free && b -> size >= size)) {
		*last = b;
		b = b->next;
	}
	return (b);
}

t_block extend_heap(t_block last, size_t s)
{
	t_block b;
	b = sbrk(0);

	if (sbrk(BLOCK_SIZE + s) == (void*) - 1)
	{
		return NULL;
	}

	b -> size = s;
	b -> next = NULL;

	if (last)
	{
		last -> next = b;
	}

	b -> free = 0;
	return (b);
}

void split_block(t_block b, size_t s)
{
	t_block new;
	new = b -> data + s;
	new -> size = b -> size - s - BLOCK_SIZE;
	new -> next = b -> next;
	new -> free = 1;
	b -> size = s;
	b -> next = new;
}


/*
conceptually, given that we have found a block with sufficient memory for our current needs, we
know how much memory is required for our current block - which I'll call A.

So we want to make a new block that will immediately be A's next. It will have all the memory
which otherwise would b in A's page.

To do this, the start of the new block will be BLOCK_SIZE(

*/
