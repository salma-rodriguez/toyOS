#ifndef _STDARG_H_
#define _STDARG_H_

#ifndef va_arg

#ifndef _VALIST
#define _VALIST
typedef char * va_list;
#endif

#define _UPPERBND		(sizeof (int) - 1)
#define _LOWERBND		(sizeof (int) - 1)
#define _bound(X, BND)		(((sizeof (X)) + (BND)) & (~(BND)))

#define va_arg(AP, TYPE)	(*(TYPE *)(((AP) += (_bound (TYPE, _UPPERBND))) - (_bound (TYPE, _LOWERBND))))
#define va_start(AP, LAST)	(void) ((AP) = (((char *) &(LAST)) + (_bound (LAST, _UPPERBND))))
#define va_end(AP)		(void) 0

#endif /* va_arg */

#endif /* _STDARG_H_*/
