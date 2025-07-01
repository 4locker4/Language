compiler = g++

flags = -c -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal 			\
		-Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs 				\
		-Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe 					\
		-fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body 					\
		-Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op 							\
		-Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith \
		-Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits 	\
		-Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

includes = ./inc/Tree.h ./inc/Utils.h ./inc/Asserts.h ./inc/Language.h

sources = ./src/Language.cpp ./src/Tree.cpp ./src/Utils.cpp ./BackEnd/src/CodeGen.cpp

objects = $(sources:.cpp=.o)

execute = Language.exe

all: $(sources) $(execute)

run:
	@echo -e -en '\E[;32m'"\n\t\t\t\033[1mRunning...\t\t\t\033[0m\n\n"
	rm -rf GraphDump.html
	../Language/Language.exe
	
$(execute): $(objects) 
	$(compiler) $(objects) -o $@

.cpp.o: $(includes)
	@$(compiler) $(flags) $< -o $@

factorial:
	dot -Tsvg FactorialTranslation/FactorialDump.dot -o FactorialTranslation/FactorialDump.svg
	mv ./FactorialTranslation/FactorialNasm.txt ./FactorialTranslation/FactorialNasm.asm
	nasm -f elf64 -g -F dwarf ./FactorialTranslation/FactorialNasm.asm  -o ./FactorialTranslation/FactorialNasm.o
	gcc -no-pie -g ./FactorialTranslation/FactorialNasm.o -o ./FactorialTranslation/FactorialProg

clean:
	rm -rf ./src/*.o
	rm -rf *.exe