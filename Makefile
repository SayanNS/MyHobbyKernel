
GPPARAMS = -m32 -Iinclude -fno-pie -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = 	obj/loader.o \
			obj/kernel.o \
			obj/gdt.o \
			obj/hardwarecommunication/port.o \
			obj/hardwarecommunication/interruptstubs.o \
			obj/hardwarecommunication/interrupts.o \
			obj/drivers/keyboard.o \
			obj/drivers/mouse.o \
			obj/drivers/ata.o \
			obj/drivers/vga.o \
			obj/common/print.o 

run: kernel.iso
	(killall VirtualBoxVM && sleep 1) || true
	VirtualBoxVM --startvm "My Operating System" &

obj/%.o: src/%.cpp
	mkdir -p ${@D}
	g++ $(GPPARAMS) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p ${@D}
	as $(ASPARAMS) -o $@ $<

kernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: kernel.bin
	sudo cp $< /boot/$<

kernel.iso: kernel.bin
	mkdir iso
	mkdir iso/boot
	cp $< iso/boot
	mkdir iso/boot/grub
	cp grub.cfg iso/boot/grub/
	grub-mkrescue --output=$@ iso
	rm -rf iso

.PHONY: clean
clean:
	rm -rf obj kernel.bin kernel.iso
