#ifndef __ARRAY_H
#define __ARRAY_H

#include <kernel/types.h>

struct array_list;
struct ordered_array;

typedef void *any_t;
typedef int (*compare_t)(any_t, any_t);
typedef void *(*delete_t)(struct array_list *);
typedef void *(*gendel_t)(int, struct array_list *);
typedef void (*add_t)(void *, struct array_list *);
typedef void (*genadd_t)(int, void *, struct array_list *);
typedef int (*getidx_t)(void *, struct array_list *);
typedef any_t (*lookup_t)(int, struct array_list *);
typedef void (*copy_t)(struct array_list *, struct array_list *);
typedef struct ordered_array *(*place_t)(void *, size_t, compare_t);
typedef void (*insert_t)(any_t, struct ordered_array *);
typedef void (*remove_t)(uint32_t, struct ordered_array *);
typedef any_t (*getitm_t)(uint32_t, struct ordered_array *);

struct array_list {
	size_t size;
	size_t count;
	any_t *array;
	genadd_t add;
	add_t add_first;
	add_t add_last;
	gendel_t del;
	delete_t del_first;
	delete_t del_last;
	compare_t compare;
	copy_t copy;
	getidx_t get_index;
};

struct ordered_array {
	size_t count;
	any_t *array;
	insert_t insert;
	getitm_t lookup;
	remove_t remove;
	compare_t compare;
};

struct array_list *create(struct array_list *, compare_t);
struct array_list *destroy(struct array_list *);

struct ordered_array create_ordered_array(size_t, compare_t);
struct ordered_array place_ordered_array(void *, size_t, compare_t);
void destroy_ordered_array(struct ordered_array *);

#endif /* __ARRAY_H */
