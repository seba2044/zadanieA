CC      = gcc
CFLAGS  = -Wall -std=c99 -pedantic -g
LDFLAGS = 

SRC = koder.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = koder

all: $(SRC) $(EXECUTABLE)
$(EXECUTABLE): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS)  -o $@
        
.c.o:
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@
              
.PHONY: clean cleanest
clean:
	rm -f $(OBJ)
                        
cleanest: clean
	rm -f *~
                                