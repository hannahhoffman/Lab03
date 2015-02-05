AutomatedMakefile = am
CC = g++

FILES = Password.o PasswordDriver.o
EXECUTABLE = Password.exe

PROJECT_PATH = $(PROJECT_DIR)

INC_DIRS = -I$(PROJECT_PATH)/CSC2110/
LIB_DIRS = -L$(PROJECT_PATH)/CSC2110/
LIBS = (-lCSC2110 for libCSC2110.a)

COMPILE = $(CC) $(INC_DIRS) -c
LINK = $(CC) $(LIB_DIRS) -o

Password.o:		Password.h Password.cpp
				$(COMPILE) Password.cpp
all: Project

Project: 		$(FILES)
			$(LINK) $(EXECUTABLE) $(FILES) $(LIBS)