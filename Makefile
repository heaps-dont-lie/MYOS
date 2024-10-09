GPPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o \
		  obj/common/stdio.o \
		  obj/gdt.o \
		  obj/drivers/driver.o \
		  obj/hwcomms/ports.o \
		  obj/hwcomms/interruptstubs.o \
		  obj/hwcomms/interrupts.o \
		  obj/hwcomms/pci.o \
		  obj/drivers/keyboardDriver.o \
		  obj/drivers/mouseDriver.o \
		  obj/drivers/vgaDriver.o \
		  obj/kernel.o

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(GPPPARAMS) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin
	
mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso/
	rm -rf iso

run: mykernel.iso
	(killall qemu-system-i386 && sleep 1) || true
	qemu-system-i386 -boot d -cdrom $<
	
clean: 
	rm -rf obj/
	rm mykernel.*
