#include "multiboot.h"

int kmain(struct multiboot_info *mbd, unsigned int magic) {
	volatile unsigned char *videoram = (unsigned char *)0xB8000;
	videoram[0] = 65;
	videoram[1] = 0x07; 
	return 0;
}
