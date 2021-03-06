/* bigint_io.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2006-2015 Daniel Otte (bg@nerilex.org)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "hexdigit_tab.h"
#include "bigint.h"
#include <stdlib.h>
#include <string.h>

#ifndef ONLY_ALGORITHM
#include "cli.h"
#endif // ONLY_ALGORITHM

void bigint_print_hex(const bigint_t *a) {
	if (a->length_W == 0) {
#ifndef ONLY_ALGORITHM
		cli_putc('0');
#endif // ONLY_ALGORITHM
		return;
	}
	if (a->info&BIGINT_NEG_MASK) {
#ifndef ONLY_ALGORITHM
		cli_putc('-');
#endif // ONLY_ALGORITHM
	}
	size_t idx;
	uint8_t print_zero = 0;
	uint8_t *p, x, y;
	p = (uint8_t*)&(a->wordv[a->length_W - 1]) + sizeof(bigint_word_t) - 1;
	for (idx = a->length_W * sizeof(bigint_word_t); idx > 0; --idx) {
		x = *p >> 4;
		y = *p & 0xf;
		if (x != 0 || print_zero != 0) {
#ifndef ONLY_ALGORITHM
			cli_putc(pgm_read_byte(&hexdigit_tab_lc_P[x]));
#endif // ONLY_ALGORITHM
		}
		if (x) {
			print_zero = 1;
		}
		if (y != 0 || print_zero != 0) {
#ifndef ONLY_ALGORITHM
			cli_putc(pgm_read_byte(&hexdigit_tab_lc_P[y]));
#endif // ONLY_ALGORITHM
		}
		if (y) {
			print_zero = 1;
		}
		--p;
	}
}

#ifndef ONLY_ALGORITHM

#define BLOCKSIZE 32

static uint8_t char2nibble(char c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
	}
	c |= 'A' ^ 'a'; /* to lower case */
	if ( c>= 'a' && c <= 'f') {
		return c - 'a' + 10;
	}
	return 0xff;
}

static uint16_t read_byte(void) {
	uint8_t t1, t2;
	char c;
	c = cli_getc_cecho();
	if (c == '-') {
		return 0x0500;
	}
	t1 = char2nibble(c);
	if (t1 == 0xff) {
		return 0x0100;
	}
	c = cli_getc_cecho();
	t2 = char2nibble(c);
	if (t2 == 0xff) {
		return 0x0200|t1;
	}
	return (t1 << 4)|t2;
}

uint8_t bigint_read_hex_echo(bigint_t *a) {
	uint16_t allocated = 0;
	uint8_t  shift4 = 0;
	uint16_t  t, idx = 0;
	uint8_t *buf = NULL;
	a->length_W = 0;
	a->wordv = NULL;
	a->info = 0;
	for (;;) {
		if (allocated - idx < 1) {
			uint8_t *p;
			p = realloc(buf, (allocated += BLOCKSIZE) * sizeof(bigint_word_t));
			if (p == NULL) {
				cli_putstr("\r\nERROR: Out of memory!");
				free(buf);
				return 0xff;
			}
			memset((uint8_t*)p + (allocated - BLOCKSIZE) * sizeof(bigint_word_t), 0, BLOCKSIZE * sizeof(bigint_word_t));
			buf = p;
		}
		t = read_byte();
		if (idx == 0) {
			if (t & 0x0400) {
				/* got minus */
				a->info |= BIGINT_NEG_MASK;
				continue;
			} else {
				if (t == 0x0100) {
					free(a->wordv);
					a->wordv = NULL;
					return 1;
				}
			}
		}
		if (t <= 0x00ff) {
			buf[idx++] = (uint8_t)t;
		} else {
			if (t & 0x0200) {
				shift4 = 1;
				buf[idx++] = (uint8_t)((t & 0x0f) << 4);
			}
			break;
		}
	}
	/* we have to reverse the byte array */
	uint8_t tmp;
	uint8_t *p, *q;
	a->length_W = (idx + sizeof(bigint_word_t) - 1) / sizeof(bigint_word_t);
	p = buf;
	q = buf + idx - 1;
	while (q > p) {
		tmp = *p;
		*p = *q;
		*q = tmp;
		p++; q--;
	}
	a->wordv = (bigint_word_t*)buf;
	bigint_adjust(a);
	if (shift4) {
		bigint_shiftright(a, 4);
	}
	if(a->length_W == 1 && a->wordv[0] == 0){
	    a->length_W = 0;
	    a->info = 0;
	}
	return 0;
}

#endif // ONLY_ALGORITHM
