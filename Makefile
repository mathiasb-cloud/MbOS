CC      = gcc
LD      = ld

CFLAGS  = -std=gnu11 -ffreestanding -fno-stack-protector -fno-pic -m64 -mcmodel=kernel -mno-red-zone -Wall -Wextra -I./kernel/include -I./limine
LDFLAGS = -nostdlib -z max-page-size=0x1000 -T kernel/linker.ld

BUILD_DIR = build
ISO_ROOT  = $(BUILD_DIR)/iso_root
KERNEL    = $(BUILD_DIR)/mbos.elf
ISO       = $(BUILD_DIR)/mbos.iso

KERNEL_SRCS = \
	kernel/arch/x86_64/boot.c \
	kernel/kernel.c \
	kernel/framebuffer.c \
	kernel/terminal.c

KERNEL_OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(KERNEL_SRCS))

all: $(ISO)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(KERNEL_OBJS) kernel/linker.ld
	$(LD) $(LDFLAGS) $(KERNEL_OBJS) -o $(KERNEL)

$(ISO): $(KERNEL) boot/limine.conf
	rm -rf $(ISO_ROOT)
	mkdir -p $(ISO_ROOT)/boot
	mkdir -p $(ISO_ROOT)/EFI/BOOT

	cp $(KERNEL) $(ISO_ROOT)/boot/mbos.elf
	cp boot/limine.conf $(ISO_ROOT)/limine.conf

	cp limine/limine-bios.sys $(ISO_ROOT)/limine-bios.sys
	cp limine/limine-bios-cd.bin $(ISO_ROOT)/
	cp limine/limine-uefi-cd.bin $(ISO_ROOT)/
	cp limine/BOOTX64.EFI $(ISO_ROOT)/EFI/BOOT/

	xorriso -as mkisofs \
		-b limine-bios-cd.bin \
		-no-emul-boot \
		-boot-load-size 4 \
		-boot-info-table \
		--efi-boot limine-uefi-cd.bin \
		-efi-boot-part \
		--efi-boot-image \
		--protective-msdos-label \
		$(ISO_ROOT) \
		-o $(ISO)

	./limine/limine bios-install $(ISO)

run: $(ISO)
	qemu-system-x86_64 -cdrom $(ISO) -m 512M

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean