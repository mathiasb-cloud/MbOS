#include "framebuffer.h"
#include "terminal.h"

void kmain(void) {
    framebuffer_clear(0x000000);

    terminal_init();
    terminal_set_color(0xFFFFFF, 0x000000);

    terminal_writeln("MbOS kernel started");
    terminal_writeln("Boot OK");
    terminal_writeln("Framebuffer OK");
    terminal_writeln("Terminal OK");
    terminal_writeln("BIENVENIDOS a  MbOS");

    for (;;) {
        __asm__ volatile ("hlt");
    }
}