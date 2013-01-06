#include <kernel/types.h>
#include <kernel/common.h>
#include <kernel/monitor.h>

#define BGCOLOR GREEN
#define FGCOLOR WHITE

#define TABSTOP 0x08

// Store cursor position
u8 c_x = 0;
u8 c_y = 0;

// VGA frame buffer begins at 0xB800
u16 *video_memory = (u16 *)0xB8000;

// Update hardware cursor
static void move_cursor()
{
	u16 loc;
	// Working with 80-character screen
	// Determin location of cursor on the screen
	loc = c_y * 80 + c_x;
	// Send command to VGA board for high cursor byte
	outb(0x3D4, 14);
	// Send the high cursor byte, right shifted by 8
	outb(0x3D5, loc >> 8);
	// Prepare VGA for setting the low cursor byte
	outb(0x3D4, 15);
	// Send the low cursor byte
	outb(0x3D5, loc);
}

// Scroll the text on the screen up by one line
static void scroll()
{
	int area;
	u16 blank;
	u8 attribyte;
	
	area = 24*80;
	attribyte = (BGCOLOR << 4) | (FGCOLOR & 0x0F);
	blank = 0x20 | (attribyte << 8);

	if (c_y >= 25)
	{
		// Move screen text back in the buffer by a line
		int i;
		for (i = 0*80; i < area; i++)
			video_memory[i] = video_memory[i+80];

		// Last line should be blank.
		// Write 80 spaces to it.
		for (i = area; i < area + 80; i++)
			video_memory[i] = blank;
		c_y = 24;
	}
}

// Clear the screen by copying spaces to the framebuffer.
void monitor_clear()
{
	int area;
	u16 blank;
	u8 attribyte;

	area = 24*80;
	// Make an attribute byte for the default colors
	attribyte = (BGCOLOR << 4) | (FGCOLOR & 0x0F);
	blank = 0x20 | (attribyte << 8);

	int i;
	for (i = 0; i < area + 80; i++)
		video_memory[i] = blank;

	// Move hardware cursor back to the start
	c_x = 0;
	c_y = 0;
	move_cursor();
}

// Write a single character out to the screen.
void monitor_put(char c)
{
	u8 fg_color;
	u8 bg_color;
	u8 attribyte;
	u16 attribute, *location;

	bg_color = BGCOLOR;
	fg_color = FGCOLOR;
	
	// The attribute byte is made up of two nibbles,
	// lower being forground color and upper background color
	attribyte = (bg_color << 4) | (fg_color & 0x0F);

	attribute = attribyte << 8;

	// Backspace is handled by moving cursor back one space.
	if (c == 0x08 && c_x)
		c_x--;
	/* 
	 * Tab is handled by moving cursor by TABSTOP bytes,
	 * where TABSTOP is a multiple of 8.
	 */
	else if (c == 0x09) 
		c_x = (c_x+TABSTOP) & ~(TABSTOP-1);
	// Handle a carriage return
	else if (c == '\r')
		c_x = 0;
	/*
	 * Handle newline by moving cursor left and increasing
	 * the row count
	 */
	else if (c == '\n') {
		c_x = 0;
		c_y++;
	}
	// Handle any other printable character.
	else if (c >= ' ') {
		location = video_memory + (c_y*80 + c_x);
		*location = c | attribute;
		c_x++;
	}

	/*
	 * Check for new line at the end of the screen
	 */
	if (c_x >= 80) {
		c_x = 0;
		c_y++;
	}

	// Scroll screen if necessary
	scroll();
	// Move the hardware cursor.
	move_cursor();
}

// Output a null-terminated ASCII string to the monitor.
void monitor_write(char *c)
{
	int i = 0;
	while(c[i])
		monitor_put(c[i++]);
}
