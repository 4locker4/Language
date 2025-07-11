compiler = g++

flags = -c -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal 			\
		-Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs 				\
		-Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe 					\
		-fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body 					\
		-Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op 							\
		-Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith \
		-Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits 	\
		-Wwrite-strings -Werror=vla

includes = ./inc/Tree.h ./inc/Utils.h ./inc/Asserts.h ./inc/Language.h ./BackEnd/inc/AdditionalFuncs.hpp ./BackEnd/inc/Elf64Gen.hpp \
			./BackEnd/inc/EmittersDSL.hpp ./BackEnd/inc/emitters.hpp

sources = ./src/Language.cpp ./src/Tree.cpp ./src/Utils.cpp ./BackEnd/src/CodeGen.cpp ./BackEnd/src/ElfFiller.cpp ./BackEnd/src/emitters.cpp \
#./BackEnd/src/LinkerPrototype.cpp

objects = $(sources:.cpp=.o)

execute = Language.out

all: $(sources) $(execute)

run:
	@echo -e -en '\E[;32m'"\n\t\t\t\033[1mRunning...\t\t\t\033[0m\n\n"
	rm -rf GraphDump.html
	./Language.out 
	
$(execute): $(objects) 
	$(compiler) $(objects) -o $@

.cpp.o: $(includes)
	@$(compiler) $(flags) $< -o $@

factorial:
	./Language.out Factorial
	dot -Tsvg FactorialTranslation/FactorialDump.dot -o FactorialTranslation/FactorialDump.svg
	mv ./FactorialTranslation/FactorialNasm.txt ./FactorialTranslation/FactorialNasm.asm
	mv ./FactorialTranslation/FactorialBin.txt ./FactorialTranslation/FactorialBin.o
	nasm -f elf64 ./BackEnd/src/Printf.asm -o ./BackEnd/src/Printf.o
	nasm -f elf64 ./FactorialTranslation/FactorialNasm.asm -o ./FactorialTranslation/FactorialNasm.o
	ld -nostdlib -static -o ./FactorialTranslation/FactorialProg.out ./FactorialTranslation/FactorialBin.o ./BackEnd/src/Printf.o

square:
	./Language.out SquareSolver
	dot -Tsvg SquareSolverTranslation/SquareSolverDump.dot -o SquareSolverTranslation/SquareSolverDump.svg
	mv SquareSolverTranslation/SquareSolverNasm.txt SquareSolverTranslation/SquareSolverNasm.asm
	nasm -f elf64 ./BackEnd/src/Printf.asm -o ./BackEnd/src/Printf.o
	nasm -f elf64 SquareSolverTranslation/SquareSolverNasm.asm -o SquareSolverTranslation/SquareSolverNasm.o
	ld -o  SquareSolverTranslation/SquareSolverProg.out SquareSolverTranslation/SquareSolverNasm.o ./BackEnd/src/Printf.o

run_factorial:
	./FactorialTranslation/FactorialProgFromNasm.out

run_square:
	./SquareSolverTranslation/SquareSolverProgFromNasm.out

clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf ./src/*.o
	rm -rf ./BackEnd/src/*.o