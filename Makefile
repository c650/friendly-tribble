CC=g++
CFLAGS= --std=c++17

# LDLIBS= -lcurl -lcrypto -lssl -lpthread -ldl
# LDFLAGS= -L/usr/lib/x86_64-linux-gnu

SRC= ./src/*.cpp
OUT= ./bin/html-parser.out

OPTIMIZE= -Ofast #-Wall -Werror # speed.

build: ${SRC}
	${CC} ${OPTIMIZE} ${CFLAGS} -o ${OUT} ${SRC} #${LDFLAGS} ${LDLIBS}

.PHONY: clean

clean:
	rm ./bin/*.out
