
# sudo apt-get install g++ binutils libc6-dev-i386
# sudo apt-get install xorriso

GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = src/loader.o src/kernel.o


src/tmp/%.s: src/%.cpp
	g++ $(GCCPARAMS) -S -o $@ $<

src/%.o: src/%.cpp
	g++ $(GCCPARAMS) -c -o $@ $<

src/%.o: src/%.s
	as $(ASPARAMS) -o $@ $<

kernel.bin: src/linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)
	rm $(objects)

kernel.iso: kernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp kernel.bin iso/boot/kernel.bin
	echo 'set timeout=0'                     >> iso/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/kernel.bin'      >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=kernel.iso iso
	rm -rf iso
	mv kernel.iso prod/kernel.iso

run: kernel.iso src/tmp/kernel.s
	qemu-system-i386 prod/kernel.iso
