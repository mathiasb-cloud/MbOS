#include "terminal.h"
#include "framebuffer.h"
#include "font8x8.h"

#include <stddef.h>
#include <stdint.h>

static uint32_t term_col = 0;
static uint32_t term_row = 0;
static uint32_t fg_color = 0xFFFFFF;
static uint32_t bg_color = 0x000000;

static const uint32_t CHAR_W = 8;
static const uint32_t CHAR_H = 8;
static const uint32_t CHAR_SPACING_X = 1;
static const uint32_t CHAR_SPACING_Y = 2;
static const uint32_t MARGIN_X = 10;
static const uint32_t MARGIN_Y = 10;

static uint32_t terminal_columns(void) {
    uint32_t usable = framebuffer_width() - (MARGIN_X * 2);
    return usable / (CHAR_W + CHAR_SPACING_X);
}

static uint32_t terminal_rows(void) {
    uint32_t usable = framebuffer_height() - (MARGIN_Y * 2);
    return usable / (CHAR_H + CHAR_SPACING_Y);
}

static void terminal_newline(void) {
    term_col = 0;
    term_row++;

    if (term_row >= terminal_rows()) {
        term_row = 0;
        framebuffer_clear(bg_color);
    }
}

static void draw_char_at(char c, uint32_t px, uint32_t py) {
    unsigned char uc = (unsigned char)c;
    const uint8_t *glyph = font8x8_basic[uc];

    for (uint32_t y = 0; y < CHAR_H; y++) {
        uint8_t row = glyph[y];

        for (uint32_t x = 0; x < CHAR_W; x++) {
            uint32_t color = (row & (1u << (7 - x))) ? fg_color : bg_color;
            framebuffer_put_pixel(px + x, py + y, color);
        }
    }
}

void terminal_init(void) {
    term_col = 0;
    term_row = 0;
    fg_color = 0xFFFFFF;
    bg_color = 0x000000;
}

void terminal_set_color(uint32_t fg, uint32_t bg) {
    fg_color = fg;
    bg_color = bg;
}

void terminal_write(const char *str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        char c = str[i];

        if (c == '\n') {
            terminal_newline();
            continue;
        }

        uint32_t px = MARGIN_X + term_col * (CHAR_W + CHAR_SPACING_X);
        uint32_t py = MARGIN_Y + term_row * (CHAR_H + CHAR_SPACING_Y);

        draw_char_at(c, px, py);

        term_col++;
        if (term_col >= terminal_columns()) {
            terminal_newline();
        }
    }
}

void terminal_writeln(const char *str) {
    terminal_write(str);
    terminal_write("\n");
}