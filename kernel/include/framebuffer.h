#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include <stddef.h>
#include "limine.h"

void framebuffer_init(struct limine_framebuffer *fb);
void framebuffer_put_pixel(uint32_t x, uint32_t y, uint32_t color);
void framebuffer_clear(uint32_t color);

uint32_t framebuffer_width(void);
uint32_t framebuffer_height(void);

#endif