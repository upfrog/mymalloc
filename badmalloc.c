/* A horrible dummy malloc */

#include <stdio.h> 
#include <unistd.h>

#define align4(x) ((((x) - 1) >> 2) << 2) + 4)


typedef struct s_block *t_block;

struct s_block {
	size_t		size;
	t_block		next;
	int			free;
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
