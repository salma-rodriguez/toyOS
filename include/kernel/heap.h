#ifndef _KHEAP_H_
#define _KHEAP_H_

uint32_t kmalloc(size_t siz);
uint32_t kmalloc_a(size_t siz);
uint32_t kmalloc_p(size_t siz, uint32_t *phys);
uint32_t kmalloc_ap(size_t siz, uint32_t *phys);

#endif /* _KHEAP_H_ */
