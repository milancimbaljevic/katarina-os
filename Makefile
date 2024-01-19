FILES = ./build/kernel.asm.o ./build/kernel.o build/memory/heap.o build/string/string.o build/string/StringBuilder.o
FILES += ./build/debug/debugcon.o ./build/math/math.o ./build/utils/float.o
FILES += ./build/memory/memory.o ./build/idt/idt.asm.o ./build/idt/idt.o ./build/idt/IrqHandler.o
FILES += ./build/print.o

INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ./bin/boot.bin ./bin/kernel.bin
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=1048576 count=16 >> ./bin/os.bin

./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-g++ $(FLAGS) -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o

./bin/boot.bin: ./src/boot/boot.asm
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

./build/kernel.asm.o: ./src/kernel.asm
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o

./build/kernel.o: ./src/kernel.cpp
	i686-elf-g++ $(INCLUDES) $(FLAGS) -c ./src/kernel.cpp -o build/kernel.o

./build/memory/heap.o: ./src/memory/heap.cpp
	i686-elf-g++ $(INCLUDES) $(FLAGS) -c ./src/memory/heap.cpp -o build/memory/heap.o

./build/string/string.o: ./src/string/string.cpp
	i686-elf-g++ $(INCLUDES) $(FLAGS) -c ./src/string/string.cpp -o build/string/string.o

./build/string/StringBuilder.o: ./src/string/StringBuilder.cpp
	i686-elf-g++ $(INCLUDES) $(FLAGS) -c ./src/string/StringBuilder.cpp -o build/string/StringBuilder.o

./build/debug/debugcon.o: ./src/debug/debugcon.cpp
	i686-elf-g++ $(INCLUDES) $(FLAGS) -c ./src/debug/debugcon.cpp -o build/debug/debugcon.o

./build/utils/float.o: ./src/utils/float.cpp
	i686-elf-g++ $(INCLUDES) $(FLAGS) -c ./src/utils/float.cpp -o build/utils/float.o

./build/math/math.o: ./src/math/math.cpp
	i686-elf-g++ $(INCLUDES) $(FLAGS) -c ./src/math/math.cpp -o build/math/math.o

./build/memory/memory.o: ./src/memory/memory.cpp
	i686-elf-g++ $(INCLUDES) $(FLAGS) -c ./src/memory/memory.cpp -o build/memory/memory.o

./build/idt/idt.asm.o: ./src/idt/idt.asm
	nasm -f elf -g ./src/idt/idt.asm -o ./build/idt/idt.asm.o

./build/idt/idt.o: ./src/idt/idt.cpp
	i686-elf-g++ $(INCLUDES) $(FLAGS) -c ./src/idt/idt.cpp -o build/idt/idt.o

./build/idt/IrqHandler.o: ./src/idt/IrqHandler.cpp
	i686-elf-g++ $(INCLUDES) $(FLAGS) -c ./src/idt/IrqHandler.cpp -o build/idt/IrqHandler.o

./build/print.o: ./src/print.cpp
	i686-elf-g++ $(INCLUDES) $(FLAGS) -c ./src/print.cpp -o build/print.o

clean:
	rm -rf ./bin/boot.bin
	rm -rf ./bin/kernel.bin
	rm -rf ./bin/os.bin
	rm -rf $(FILES)
	rm -rf ./build/kernelfull.o

run:
	qemu-system-i386 -hda ./bin/os.bin -debugcon stdio