#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>

void terminal_init(void);
void terminal_set_color(uint32_t fg, uint32_t bg);
void terminal_write(const char *str);
void terminal_writeln(const char *str);

#endif