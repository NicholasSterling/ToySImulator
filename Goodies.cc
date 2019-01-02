/*
 * goodies.c -- Some miscellaneous handy routines.
 */

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include "goodies.hh"

int get_xdigit(std::istream&);

/*
 * find_ms1b() takes a Ulong and returns the index (0..?) of the most
 * significant 1 bit in it.  If the Ulong is 0, a -1 is returned.
 */
int find_ms1b(Ulong val) {
	int   bit_num = -1;
	while (val) {
		val >>= 1;
		bit_num++;
	}
	return bit_num;
}

/*
 * err() is a printf-like routine for dealing with nonfatal errors.
 * fatal() is the fatal equivalent.
 */
void err(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
}

void fatal(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	exit(-1);
}

/*
 * get_byte() returns a byte from the specified stream.  The stream should
 * contain hexdigits and arbitrary whitespace and comments (# to \n).  A
 * return value of -1 indicates end-of-stream.
 */
int get_byte(std::istream& input) {
	int	c1,c2;
	c1 = get_xdigit(input);
	if (c1 == -1)
		return -1;
	c2 = get_xdigit(input);
	if (c2 == -1) {
		err("get_byte: Warning -- unpaired hexdigit in stream\n");
		c2 = 0;
	}
	return (c1 << 4) + c2;
}

int get_xdigit(std::istream& input) {
	char	c;
	Bool	comment = FALSE;
	for (;;) {
//		if (input.eof())
//			return -1;
		input.get(c);
		if (input.eof())
			return -1;
		if (!input)
			fatal("get_xdigit: failed to get char from stream\n");
		if (comment) {
			if (c == '\n')
				comment = FALSE;
		} else {
			if (isxdigit(c))
				break;
			if (c == '#')
				comment = TRUE;
			else if (!isspace(c))
				err("get_xdigit: uncommented non-hexdigit '%c' in stream\n", c);
		}
	}

	// Got a hex digit; return its integer value.
	c  -=  '0';
	if (c > 9) {
		c  -=  'A' - '0' - 10;
		if (c > 0xf)
			c  -=  'a' - 'A';
	}
	return (int)c;
}
