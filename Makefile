SVN_VER	= $(shell svnversion | sed 's,[^0-9]*,,g' | sed 's,^$$,0,g')

CC	= g++
CFLAGS	= -O2 -Wall -Werror -DSVN_VER=\""${SVN_VER}"\"

TARGET	= fqtools
MODULES	= fqtools merge split filter

.PHONY: all clean

all: ${TARGET}

clean:
	rm -fv ${TARGET} ${MODULES:%=%.o}

fqtools: ${MODULES:%=%.o}
	${CC} ${CFLAGS} -o $@ $^

%.o: %.cpp
	${CC} -c ${CFLAGS} -o $@ $<
