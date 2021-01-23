
# sudo apt-get install g++ binutils libc6-dev-i386
# sudo apt-get install xorriso

GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -I include
ASPARAMS = --32
LDPARAMS = -melf_i386
SRC = src
BUILD = build
TMP = tmp

build_directories = $(BUILD) $(BUILD)/drivers $(BUILD)/filesystem $(BUILD)/hardware $(BUILD)/lib $(BUILD)/memory $(BUILD)/sys
sw_build_directories = $(BUILD)/simpletext
kernel_main = $(BUILD)/kernel.o
as_objects = $(BUILD)/loader.o $(BUILD)/interrupt_stubs.o $(BUILD)/virtual_memory_switch.o
cpp_objects = $(patsubst $(SRC)/%.cpp, $(BUILD)/%.o, $(wildcard $(SRC)/*/*.cpp))

setup_build:
	mkdir $(build_directories)
	mkdir $(sw_build_directories)

$(TMP)%.s: %.cpp
	g++ $(GCCPARAMS) -S -o $@ $<

$(kernel_main): kernel.cpp
	g++ $(GCCPARAMS) -c -o $@ $<

$(BUILD)/%.o: $(SRC)/%.cpp
	g++ $(GCCPARAMS) -c -o $@ $<

$(BUILD)/%.o: %.s
	as $(ASPARAMS) -o $@ $<

kernel.bin: linker.ld $(as_objects) $(cpp_objects) $(kernel_main)
	ld $(LDPARAMS) -T $< -o $@ $(as_objects) $(cpp_objects) $(kernel_main)

kernel.iso: kernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp kernel.bin iso/boot/kernel.bin
	echo 'set timeout=0'                     >> iso/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "OS-ONE" {'              >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/kernel.bin'      >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=kernel.iso iso
	mv kernel.iso prod/kernel.iso
	rm -rf iso
	rm kernel.bin

run: kernel.iso
	qemu-system-i386 prod/kernel.iso

clean:
	rm -rf -f build
	make setup_build
