
all:  bootloader kernel userland image test

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean

test:
	chmod -R g+wx /root/

.PHONY: bootloader image collections kernel userland all clean
