#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


void free(void *ptr);
void *realloc(void *ptr, size_t size);
void *calloc(size_t, size_t);
void *malloc(size_t);
struct block_meta *find_free_block(struct block_meta **last, size_t);
struct block_meta *request_space(struct block_meta* last, size_t);
struct block_meta *get_block_ptr(void *ptr);

struct block_meta {
	size_t size;
	struct block_meta *next;
	int free;
	int debug;
};

#define META_SIZE sizeof(struct block_meta)

void *global_base = NULL;




struct block_meta *get_block_ptr(void *ptr)
{
	return (struct block_meta*)ptr - 1;
}


//Consider behavior if a block has already been freed
void free(void *ptr)
{
	if (!ptr)
	{
		return;
	}

	struct block_meta *block_ptr = get_block_ptr(ptr);
	assert(block_ptr -> free == 0);
	assert(block_ptr -> debug == 0x77777777 || block_ptr -> debug == 0x12345678);
	block_ptr -> free = 1;
	block_ptr -> debug = 0x55555555;
}

void *realloc(void *ptr, size_t size)
{
	if (!ptr)
	{
		return malloc(size);
	}

	struct block_meta* block_ptr = get_block_ptr(ptr);
	if (block_ptr -> size >= size)
	{
		//this should probably wait until I actually free up unneeded memory	
		//block_ptr -> size = size;
		return ptr;
	}
	
	void *new_block_ptr;
	new_block_ptr = malloc(size);
	if (!new_block_ptr)
	{
		return NULL;
	}
	memcpy(new_block_ptr, ptr, block_ptr -> size);
	free(ptr);
	return new_block_ptr;
}


void *calloc(size_t elems_count, size_t elem_size)
{
	size_t size = elems_count * elem_size;
	void *ptr = malloc(size);
	memset(ptr, 0, size);
	return ptr;
}


void *malloc(size_t size)
{
	struct block_meta *block;

	//Nothing to allocate
	if (size <= 0)
	{
		return NULL;
	}

	//In this case, this is the first malloc
	if (!global_base)
	{
		block = request_space(NULL, size);
		if (!block)
		{
			return NULL;
		}
	}
	else
	{
		struct block_meta *last = global_base;
		block = find_free_block(&last, size);
		if (!block)
		{
			return NULL;
		}
		else
		{
			block -> free = 0;
			block -> debug = 0x77777777;
		}
	}
	return (block+1);
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


