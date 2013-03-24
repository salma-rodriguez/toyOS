#include <array.h>
#include <assert.h>
#include <string.h>
#include <kernel/heap.h>

static void insert(any_t, struct ordered_array *);
static void remove(uint32_t, struct ordered_array *);
static any_t lookup(uint32_t, struct ordered_array *);

struct ordered_array create_ordered_array(size_t size, compare_t fun)
{
	struct ordered_array to_ret;
	to_ret.array = (void *)kmalloc(size * sizeof(any_t));
	memset(to_ret.array, 0, size * sizeof(any_t));
	to_ret.count = 0;
	to_ret.compare = fun;
	to_ret.insert = insert;
	to_ret.remove = remove;
	to_ret.lookup = lookup;
	return to_ret;
}

struct ordered_array place_ordered_array(void *addr, size_t size, compare_t fun)
{
	struct ordered_array to_ret;
	to_ret.array = (any_t *)addr;
	memset(to_ret.array, 0, size * sizeof(any_t));
	to_ret.count = 0;
	to_ret.compare = fun;
	to_ret.remove = remove;
	to_ret.lookup = lookup;
	to_ret.insert = insert;
	return to_ret;
}

void destroy_ordered_array(struct ordered_array *arr)
{
	kfree(arr->array);
}

static void insert(any_t item, struct ordered_array *arr)
{
	ASSERT(arr->compare);
	any_t t1, t2;
	uint32_t i = 0;
	while(i < arr->count && arr->compare(arr->array[i], item))
		i++;
	if (i == arr->count)
		arr->array[arr->count++] = item;
	else {
		t1 = arr->array[i];
		arr->array[i] = item;
		while (i < arr->count)
		{
			i++;
			t2 = arr->array[i];
			arr->array[i] = t1;
			t1 = t2;
		}
		arr->count++;
	}
}

static any_t lookup(uint32_t i, struct ordered_array *arr)
{
	ASSERT(i < arr->count);
	return arr->array[i];
}

static void remove(uint32_t i, struct ordered_array *arr)
{
	while (i < arr->count)
	{
		arr->array[i] = arr->array[i+1];
		i++;
	}
	arr->count--;
}
