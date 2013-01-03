#ifndef MONITOR_H
#define MONITOR_H

#define BLACK	0
#define BLUE	1
#define GREEN	2
#define CYAN	3
#define RED	4
#define MAGENTA 5
#define BROWN	6
#define LGRAY	7
#define DGRAY	8
#define LBLUE	9
#define LGREEN	10
#define LCYAN	11
#define LRED	12
#define LMGENTA	13
#define LBROWN	14
#define WHITE	15

// Write a character to the screen
void monitor_put(char c);
// Clear the screen
void monitor_clear();
// Output a null-terminated ASCII string to the monitor
void monitor_write(char *c);

#endif
