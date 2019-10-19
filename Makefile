CC = g++

TAGS = -Wall -g

TARG = myshell

SRC =  myshell.cpp\
	   parse.cpp\
	   param.cpp

LFLAGS = -pthread

OBJ = $(SRC:.c=.o)

.PHONY: clean

all: $(TARG)
			@echo Compiling Success!

$(TARG): $(OBJ)
			$(CC) $(TAGS) -o $(TARG) $(OBJ)
						@echo Linking Success!

.c.o:
			$(CC) $(TAGS) -c $< -o $@ $(LFLAGS)
						@echo Compiling $<

clean:
			rm *.o
					
