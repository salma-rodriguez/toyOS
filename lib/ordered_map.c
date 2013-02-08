#include <array.h>

static void insert(any_t item, struct ordered_array *array);

struct ordered_array *create_ordered_array(size_t size, compare_t fun)
{
	struct ordered_array to_ret;
	to_ret.array = (void *)kmalloc(size*sizeof*(any_t));
	memset(to_ret.array, 0, size*sizeof(any_t));
	to_ret.count = 0;
	to_ret.size = size;
	to_ret.compare = fun;
	to_ret.insert = insert;
	return &to_ret;
}

struct ordered_array *place_ordered_array(void *addr, size_t size, compare_t fun)
{
	struct ordered_array to_ret;
	to_ret.array = (type_t *)addr;
	memset(to_ret.array, 0, max_size*sizeof(any_t));
	to_ret.count = 0;
	to_ret.size = size;
	to_ret.compare = fun;
}

struct ordered_array *destroy_ordered(ordered_array_t *arr)
{
	kfree(arr->array);
	return NULL;
}

static void insert_ordered_array(any_t item, ordered_array *arr)
{
	// ASSERT(array->compare);
	any_t t1, t2;
	uint32_t i = 0;
	while(i < arr->count && array->compare(arr->array[i], item) < 0)
		i++;
	if (i == arr->count)
		arr->array[arr->count++] = item;
	else {
		t1 = arr->array[i];
		arr->array[i] = item;
		while (i < arr->size)
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
	// ASSERT(i < arr->size);
	return arr->array[i];
}

void remove_ordered_array(uint32_t i, struct ordered_array *arr)
{
	while (i < arr->count)
	{
		arr->array[i] = arr->array[i+1];
		i++;
	}
	arr->count--;
}
