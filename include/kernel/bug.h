#ifndef _BUG_H_
#define _BUG_H_

extern void bug(const char *, const char *, unsigned int);

#define BUG(message) bug(message, __FILE__, __LINE__)

#endif /* _BUG_H_ */
