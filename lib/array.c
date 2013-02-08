#include <array.h>
#include <string.h>
#include <kernel/heap.h>

#define DEFAULT_SIZE 100

static void *del(int, struct array_list *);
static void *del_last(struct array_list *);
static void *del_first(struct array_list *);
static void add(int, void *, struct array_list *);
static void add_last(void *, struct array_list *);
static void add_first(void *, struct array_list *);
static int get_index(void *, struct array_list *);
static void copy(struct array_list *, struct array_list *);

static any_t *__set(int size)
{
	any_t *arr;
	arr = kmalloc(size * sizeof(any_t));
	return memset(arr, 0, size * sizeof(any_t));
}

static void __alloc(struct array_list **list)
{
	*list = kmalloc(sizeof(struct array_list));
}

static struct array_list *__init(struct array_list *list, compare_t fun)
{
	__alloc(&list);
	list->count = 0;
	list->size = DEFAULT_SIZE;
	list->array = __set(list->size);
	list->add = add;
	list->add_first = add_first;
	list->add_last = add_last;
	list->del = del;
	list->del_first = del_first;
	list->del_last = del_last;
	list->compare = fun;
	list->copy = copy;
	list->get_index = get_index;
	return list;
}

static void ___copy(any_t *des, any_t *src, int idx, int count)
{
	int i, j;
	for (j = 0, i = idx; j < count; i++, j++)
		des[i] = src[j];
}

static void __copy(any_t *des, any_t *src, int count)
{
	___copy(des, src, 0, count);
}

static void copy(struct array_list *des, struct array_list *src)
{
	___copy(des->array, src->array, des->count, src->count);
}

struct array_list *create(struct array_list *list, compare_t fun)
{
	return __init(list, fun);
}

struct array_list *destroy(struct array_list *list)
{
	// free(list->array);
	list->size = 0;
	list->count = 0;
	list->add = NULL;
	list->add_first = NULL;
	list->add_last = NULL;
	list->del = NULL;
	list->del_first = NULL;
	list->del_last = NULL;
	list->compare = NULL;
	list->copy = NULL;
	list->get_index = NULL;
	return NULL;
}

static int get_index(any_t item, struct array_list *list)
{
	int idx;
	for (idx = 0; idx < list->count; idx++)
		if (!list->compare(list->array[idx], item))
			return idx;
	return -1;
}

static void __add(int idx, void *item, struct array_list *list)
{
	int i;
	for (i = list->count; i > idx; i--)
		list->array[i] = list->array[i-1];
	list->array[idx] = item;
	list->count++;
}

static void __double(struct array_list *list)
{
	any_t *new;
	list->size *= 2;
	new = __set(list->size);
	__copy(new, list->array, list->count);
	// free(list->array);
	list->array = new;
}

static void add(int idx, void *item, struct array_list *list)
{

	if (list->count < list->size)
		goto next;

	__double(list);
next:
	__add(idx, item, list);
}

static void add_first(void *item, struct array_list *list)
{
	add(0, item, list);
}

static void add_last(void *item, struct array_list *list)
{
	add(list->count, item, list);
}

static void *del(int idx, struct array_list *list)
{
	int i;
	any_t item;
	item = list->array[idx];
	for (i = idx; i < list->count-1; i++)
		list->array[i] = list->array[i+1];
	list->array[i] = '\0';
	return item;
}

static void *del_first(struct array_list *list)
{
	return del(0, list);
}

static void *del_last(struct array_list *list)
{
	return del(list->count-1, list);
}
