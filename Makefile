CC = g++

TAGS = -Wall -g

TARG = mt-collatz

SRC =  mt-collatz.c

LFLAGS = -lpthread

OBJ = $(SRC:.c=.o)

.PHONY: clean

all: $(TARG)
			@echo Compiling Success!

$(TARG): $(OBJ)
			$(CC) $(TAGS) -o $(TARG) $(OBJ) $(LFLAGS)
						@echo Linking Success!

.c.o:
			$(CC) $(TAGS) -c $< -o $@
						@echo Compiling $<

clean:
			rm *.o mt-collatz
					
