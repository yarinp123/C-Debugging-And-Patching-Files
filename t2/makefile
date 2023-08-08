# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: virusDetector

# Tool invocations
# Executable "lab1" depends on the files:
virusDetector: task2b.o
	@echo 'Building target: virusDetector'
	$(CC) -m32 task2b.o -o virusDetector
	@echo 'Finished building target: virusDetector'

# Depends on the source file (c)
task2b.o: task2b.c
	$(CC) $(CFLAGS) -m32 -o task2b.o task2b.c 

#tell make that "clean" is not a file name! 
.PHONY: clean 

# Clean the build directory and executable
clean:
	rm -f *.o virusDetector