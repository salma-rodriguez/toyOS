#ifndef _SEGMENT_H_
#define _SEGMENT_H_


#define SEG_SAVL(X)      ((X) << 0x04) 
#define SEG_LONG(X)      ((X) << 0x05) 
#define SEG_SIZE(X)      ((X) << 0x06) 
#define SEG_GRAN(X)      ((X) << 0x07) 
#define SEG_PRES(X)      ((X) << 0x07) 
#define SEG_PRIV(X)     (((X) &  0x03) << 0x05)
#define SEG_DESCTYPE(X)  ((X) << 0x04)
	 
#define SEG_DATA_RD        0x00 
#define SEG_DATA_RDA       0x01 
#define SEG_DATA_RDWR      0x02 
#define SEG_DATA_RDWRA     0x03 
#define SEG_DATA_RDEXPD    0x04 
#define SEG_DATA_RDEXPDA   0x05 
#define SEG_DATA_RDWREXPD  0x06 
#define SEG_DATA_RDWREXPDA 0x07 
#define SEG_CODE_EX        0x08 
#define SEG_CODE_EXA       0x09 
#define SEG_CODE_EXRD      0x0A 
#define SEG_CODE_EXRDA     0x0B 
#define SEG_CODE_EXC       0x0C 
#define SEG_CODE_EXCA      0x0D 
#define SEG_CODE_EXRDC     0x0E 
#define SEG_CODE_EXRDCA    0x0F 
 
#define GDT_CODE_PL(X) SEG_PRES(1) 	| SEG_PRIV(X) | \
		   	SEG_DESCTYPE(1) | SEG_CODE_EXRD
		       
#define GDT_DATA_PL(X) SEG_PRES(1) 	| SEG_PRIV(X) | \
		   	SEG_DESCTYPE(1) | SEG_DATA_RDWR

#define GRANULARITY(L) SEG_GRAN(1) 	| SEG_SIZE(1) | \
			SEG_LONG(0)	| SEG_SAVL(0) | L

#endif /* _SEGMENT_H_ */
