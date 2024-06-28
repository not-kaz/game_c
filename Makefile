# TODO: Modify makefile so it's relatively crossplatform. "-lmingw32, -lGL" etc.
CC = gcc 
CF = -Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes \
     -Wdeclaration-after-statement -Wmissing-declarations \
     -Wimplicit-function-declaration -std=c99 -pedantic \
     -Wshadow -Wdouble-promotion -Wconversion -Wformat \
     -Wformat-signedness -Wformat-extra-args \
     -Wpointer-arith -Wcast-qual
LF = -lSDL2main -lSDL2 -lSDL2_image -lm -lcglm -lGL -Llib/glad -lglad
DF = -DDEBUG_MODE
INC = -Iinclude -Ilib
SRC = $(wildcard src/*.c)
 
all: $(SC)
	$(CC) -g3 $(CF) $(INC) $(SRC) $(DF) $(LF)
