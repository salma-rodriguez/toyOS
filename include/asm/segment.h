// This structure contains the value of one GDT entry.
// We use the attribute 'pacdked' to tell GCC not to change
// any of the aligment in the structure

struct gdt_entry_struct
{
	uint16_t limit_l;	// lower 16 bits of the limit
	uint16_t base_l;	// lower 16 bits of the base
	uint8_t	 base_m;	// middle eight bits of the base
	uint8_t	 access;	// access flags, determines ring
	uint8_t	 granularity;
	uint8_t	 base_h;	// last 8 bits of the base
} __attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

struct gdt_ptr_struct
{
	uint16_t limit;		// upper 16 bits of selector limits
	uint32_t base;		// address of the first gdt_entry_t
}__attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

void init_descriptor_table();
