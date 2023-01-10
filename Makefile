# This is a Makefile that make uses to compile and install my C program.
# Please install gcc and g++ before running this file.
# To install my software, run `make` and then `make install`.

# Please specify the installation location.
# InstallPath = usr/local/naspi_indicator
InstallPath = ./test

CC = gcc
CFLAGS = -O3 -Wall
CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17
executable = testmain


.PHONY: all
all: $(executable)

testmain: testmain.o display.o
	@echo $@
	$(LINK.cpp) -o $@ $^

testmain.o: testmain.cpp display.h
	$(COMPILE.cpp) $<

display.o: display.c display.h
	$(COMPILE.c) $<

so1602aw.o: so1602aw.c so1602aw.h
	$(COMPILE.c) $<

# Copy executable files to InstallPath and enable dependend daemons.
.PHONY: install
install: makedir
	@echo Install to $(InstallPath)
	sudo cp $(executable) $(InstallPath)/
	@echo Making daemon file for starting automatically.
	@echo Finish installation. You can remove this directory.

# Making install directory defined on line 6 if it doesn't exist.
.PHONY: makedir
makedir:
	@if [ ! -d $(InstallPath) ]; then \
        sudo mkdir $(InstallPath); \
    fi

# Remove all object files and executable files.
.PHONY: clean
clean:
	$(RM) *.o $(executable)