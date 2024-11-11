#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


struct block_meta {
	size_t size;
	struct block_meta *next;
	int free;
	int debug;
};

#define META_SIZE sizeof(struct block_meta)

void *global_base = NULL;

void *malloc(size_t size)
{
	void *p = sbrk(0);
	void *request = sbrk(size);

	if (request == (void*) -1)
	{
		return NULL;
	}
	else 
	{
		assert(p == request);
		return p;
	}
}

struct block_meta *find_free_block(struct block_meta **last, size_t size)
{
	struct block_meta *current = global_base;
	while (current && !(current -> free && current -> size >= size))
	{
		*last = current;
		current = current -> next;
	}
	return current;
}

struct block_meta *request_space(struct block_meta* last, size_t size)
{
	struct block_meta *block;
	block = sbrk(0);
	void *request = sbrk(size + META_SIZE);
	assert((void*)block == request); //Not thread safe

	if (request == (void*) -1)
	{
		return NULL;
	}

	//If there is already at least one block_meta, we need to update it's link
	if (last)
	{
		last -> next = block;
	}

	block -> size = size;
	block -> next = NULL;
	block -> free = 0;
	block -> debug = 0x12345678;
	return block;
}


