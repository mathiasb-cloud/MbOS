#include "include/framebuffer.h"

static struct limine_framebuffer *g_fb = 0;

void framebuffer_init(struct limine_framebuffer *fb) {
    g_fb = fb;
}

void framebuffer_put_pixel(uint32_t x, uint32_t y, uint32_t color) {
    if (g_fb == 0) {
        return;
    }

    if (x >= g_fb->width || y >= g_fb->height) {
        return;
    }

    uint8_t *pixel = (uint8_t *)g_fb->address + y * g_fb->pitch + x * 4;
    *(uint32_t *)pixel = color;
}

void framebuffer_clear(uint32_t color) {
    if (g_fb == 0) {
        return;
    }

    for (uint32_t y = 0; y < g_fb->height; y++) {
        for (uint32_t x = 0; x < g_fb->width; x++) {
            framebuffer_put_pixel(x, y, color);
        }
    }
}

uint32_t framebuffer_width(void) {
    return g_fb ? g_fb->width : 0;
}

uint32_t framebuffer_height(void) {
    return g_fb ? g_fb->height : 0;
}