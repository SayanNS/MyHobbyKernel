
GPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = loader.o kernel.o gdt.o port.o interruptstubs.o interrupts.o keyboard.o

%.o: %.cpp
	g++ $(GPPARAMS) -o $@ -c $<

%.o: %.s
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

run: kernel.iso
	(killall VirtualBoxVM && sleep 1) || true
	VirtualBoxVM --startvm "My Operating System" &

.PHONY: clean
clean:
	rm -f $(objects) kernel.bin kernel.iso
